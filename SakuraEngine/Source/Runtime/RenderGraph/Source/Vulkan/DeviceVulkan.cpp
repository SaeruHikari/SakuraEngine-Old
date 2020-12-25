#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

bool checkValidationLayerSupport(const sakura::span<const char* const>);
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger_, const VkAllocationCallbacks* pAllocator);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::set<std::string> requiredExtensions);
std::vector<const char*> getRequiredDeviceExtensions(bool asMainDevice);
VkSurfaceKHR createSurface(sakura::Window window, VkInstance instance);
VkCommandPool createCommandPoolForQueue(uint32_t family_index, VkDevice logical_device, bool resetFlag = true);
VkFence createFenceForQueue(VkDevice device, bool signalOnCreate = false);

sakura::graphics::vk::RenderDevice::RenderDevice(const DeviceConfiguration& config)
	:name_(config.name)
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "SakuraEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// cn: 创建VkInstance.
	// en: Create VkInstance.
	// jp: VkInstanceを作る.
	{
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		const auto basic_exts = basic_extentions();
		createInfo.enabledExtensionCount = basic_exts.size();
		createInfo.ppEnabledExtensionNames = basic_exts.data();
		if (bEnableValidationLayers && !checkValidationLayerSupport(validationLayers))
		{
			sakura::error("validation layers requested, but not available!");
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (bEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}
		if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
		{
			sakura::error("Vulkan: failed to create instance!");
		}
	}

	// cn: 拾取PhysicalDevice.
	// en: Pick physical device.
	// jp: 物理デバイスを選択する.
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

		if (deviceCount == 0) {
			sakura::error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			VulkanDeviceSet dev = findQueueFamilies(device, config.window_handle);
			// cn: 筛选掉特性不足的设备.
			// en: Filter out devices_ with insufficient features.
			// jp: 機能が不十分なデバイスを除外する
			if (checkDeviceExtensionSupport(device, std::set<std::string>(basic_device_exts.begin(), basic_device_exts.end())))
			{
				dev.device = device;
				// Collect Device Properties & Features.
				vkGetPhysicalDeviceProperties(device, &dev.properties);
				vkGetPhysicalDeviceFeatures(device, &dev.features);
				device_sets_.emplace_back(dev);

				// TODO:
				// cn: 选择最合适的一个设备作为主设备.
				// en: Choose the most suitable device as the master_device.
				// jp: マスターデバイスとして最適なデバイスを選択する.
				if (checkDeviceExtensionSupport(device, std::set<std::string>(main_device_exts.begin(), main_device_exts.end())))
				{
					master_device_index_ = device_sets_.size() - 1;
				}
			}
		}

		if (device_sets_.empty())
			sakura::error("failed to find a suitable GPU!");
	}

	// cn: 创建逻辑设备.
	// en: Create Logical Device.
	// jp: ロジカル-デバイスを作る.
	{
		bool findGfxQueue = false;
		bool findCmptQueue = false;
		bool findTransferQueue = false;
		bool findPresentQueue = false;
		for (auto i = 0u; i < device_sets_.size(); i++)
		{
			auto& phy_dev = device_sets_[i];
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::vector<float> queuePriority; queuePriority.resize(100, 1.f);

			for(auto f = 0u; f < phy_dev.queue_families.size(); f++)
			{
				if ( (phy_dev.queue_families[f].queueFlags & VK_QUEUE_GRAPHICS_BIT) || 
					(phy_dev.queue_families[f].queueFlags & VK_QUEUE_COMPUTE_BIT) ||
					(phy_dev.queue_families[f].queueFlags & VK_QUEUE_TRANSFER_BIT) )
				{
					VkDeviceQueueCreateInfo queueCreateInfo{};
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = f;
					queueCreateInfo.queueCount = phy_dev.queue_families[f].queueCount;
					queueCreateInfo.pQueuePriorities = queuePriority.data();
					queueCreateInfos.push_back(queueCreateInfo);
				}
			}
			VkPhysicalDeviceFeatures deviceFeatures{};
			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.queueCreateInfoCount = queueCreateInfos.size();
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			// Features
			createInfo.pEnabledFeatures = &deviceFeatures;
			// Extensions
			auto devExts = getRequiredDeviceExtensions(master_device_index_ == i);
			createInfo.enabledExtensionCount = devExts.size();
			createInfo.ppEnabledExtensionNames = devExts.data();

			// Layers
			if (bEnableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			}
			else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(phy_dev.device, &createInfo, nullptr, &phy_dev.logical_device) != VK_SUCCESS)
			{
				sakura::error("[DeviceVulkan]: failed to create logical device!");
			}
			else
			{
				// TODO : Refactor this.
				phy_dev.command_pools.resize(phy_dev.queue_families.size());
				for (auto f = 0u; f < phy_dev.queue_families.size(); f++)
				{
					phy_dev.command_pools[f] = createCommandPoolForQueue(f, phy_dev.logical_device);
				}

				// cn: 为创建好的设备创建queues.
				for (auto gfxs = 0u; gfxs < phy_dev.graphics_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.graphics_queues[gfxs].family_index, gfxs, &q);
					phy_dev.graphics_queues[gfxs].queue = q;
					phy_dev.graphics_queues[gfxs].block_fence = createFenceForQueue(phy_dev.logical_device);
					findGfxQueue = true;

					// cn: 尝试检查此queue对present的支持.
					// en: Try to check the present support for this queue.
					// jp: このキューのプレゼントへのサポートを確認してみます.
					if (!findPresentQueue)
					{
						for (const auto pf : phy_dev.present_families)
						{
							if (pf == phy_dev.graphics_queues[gfxs].family_index)
							{
								findPresentQueue = true;
								phy_dev.master_queue = phy_dev.graphics_queues[gfxs];
							}
						}
					}
				}
				for (auto cs = 0u; cs < phy_dev.compute_queues.size(); cs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.compute_queues[cs].family_index, cs, &q);
					phy_dev.compute_queues[cs].queue = q;
					phy_dev.compute_queues[cs].block_fence = createFenceForQueue(phy_dev.logical_device);
					findCmptQueue = true;
				}
				for (auto transfers = 0u; transfers < phy_dev.transfer_queues.size(); transfers++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.transfer_queues[transfers].family_index, transfers, &q);
					phy_dev.transfer_queues[transfers].queue = q;
					phy_dev.transfer_queues[transfers].block_fence = createFenceForQueue(phy_dev.logical_device);
					findTransferQueue = true;
				}

				if (!findPresentQueue)
				{
					sakura::error("Failed to find a proper master_queue! Maybe your graphics queues have no support for present, and currently it's not supported by engine!");
					phy_dev.master_queue = phy_dev.graphics_queues[0];
				}
			}
		}
	}
}

sakura::graphics::vk::RenderDevice::~RenderDevice()
{

}

void sakura::graphics::vk::RenderDevice::terminate()
{
	for (auto& phy_dev : device_sets_)
	{
		vkDestroyDevice(phy_dev.logical_device, nullptr);
	}
	if (bEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
	}
	vkDestroyInstance(instance_, nullptr);
	return;
}

EBackend RenderDevice::backend() const
{
	return EBackend::Vulkan;
}

bool sakura::graphics::vk::RenderDevice::valid(const RenderResourceHandle handle) const
{
	return optional_unsafe(handle);
}

void sakura::graphics::vk::RenderDevice::destroy(const RenderResourceHandle to_destroy)
{
	return;
}

sakura::string_view sakura::graphics::vk::RenderDevice::get_name() const
{
	return name_;
}

QueueIndex RenderDevice::request_copy_queue() const
{
	assert(0 && "Unimplemented!");
	return QueueIndex(-1);
}

bool RenderDevice::execute_let_fly(const QueueIndex queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
	return false;
}

bool RenderDevice::execute_let_fly(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
	return false;
}

bool RenderDevice::execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
	return false;
}

FenceHandle RenderDevice::execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
	return GenerationalId::UNINITIALIZED;
}

FenceHandle RenderDevice::execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
	return GenerationalId::UNINITIALIZED;
}

void RenderDevice::wait_idle()
{
	assert(0 && "Unimplemented!");
	return;
}

void RenderDevice::write_texture(GpuTextureHandle texture, void const* data, size_t data_size,
	const TextureSlice& slice, const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index)
{
	//assert(0 && "Unimplemented!");
}

sakura::graphics::GpuShaderHandle sakura::graphics::vk::RenderDevice::create_shader(
	const GpuShaderHandle handle, const ShaderDescriptor& config)
{
	return _create_resouce_impl<vk::GpuShader, GpuShaderHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, 
		config);
}

sakura::graphics::GpuBufferHandle sakura::graphics::vk::RenderDevice::create_buffer(
	const GpuBufferHandle handle, const BufferDescriptor& config)
{
	return _create_resouce_impl<vk::GpuBuffer, GpuBufferHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, master_device().device,
		config);
}

GpuTextureHandle RenderDevice::create_texture(const GpuTextureHandle handle, const TextureDescriptor& desc)
{
	return handle;
}

GpuSamplerHandle RenderDevice::create_sampler(const GpuSamplerHandle handle, const SamplerDescriptor& desc)
{
	return _create_resouce_impl<vk::GpuSampler, GpuSamplerHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, master_device().device,
		desc);
}

ComputePipelineHandle RenderDevice::create_compute_pipeline(const ComputePipelineHandle handle,
	const ComputePipelineDescriptor& desc)
{
	sakura::error("UNIMPLEMENTED!");
	return GenerationalId::UNINITIALIZED;
}


sakura::graphics::FenceHandle sakura::graphics::vk::RenderDevice::create_fence(const FenceHandle handle, const FenceDescriptor& desc)
{

	return handle;
}

sakura::graphics::SwapChainHandle sakura::graphics::vk::RenderDevice::create_swap_chain(
	const SwapChainHandle handle, const SwapChainDescriptor& desc)
{
	return _create_object_impl<SwapChain, SwapChainHandle>(handle, *this, desc);
}

sakura::graphics::RenderPipelineHandle sakura::graphics::vk::RenderDevice::create_render_pipeline(
	const RenderPipelineHandle handle, const RenderPipelineDescriptor& desc)
{

	return _create_object_impl<vk::RenderPipeline, RenderPipelineHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, 
		desc);
}

sakura::graphics::GpuBufferHandle sakura::graphics::vk::RenderDevice::update_buffer(
	const GpuBufferHandle handle, size_t offset, void* data, size_t size)
{
	auto buf = get<GpuBuffer>(handle);
	void* data_ptr;
	vkMapMemory(buf->owned_device_, buf->buffer_memory_, 0, size, 0, &data_ptr);
		memcpy(data_ptr, data, size);
	vkUnmapMemory(buf->owned_device_, buf->buffer_memory_);
	return handle;
}

sakura::graphics::IGpuMemoryResource* sakura::graphics::vk::RenderDevice::get_unsafe(const RenderResourceHandle handle) const
{
	if (created_resources_.size() < handle.id().index() + 1)
	{
		handle_error<RenderResourceHandle>::not_find(handle);
		return nullptr;
	}
	const auto& resource = created_resources_[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		handle_error<RenderResourceHandle>::generation_dismatch(handle);
		return nullptr;
	}
}

sakura::graphics::IGpuMemoryResource* sakura::graphics::vk::RenderDevice::optional_unsafe(const RenderResourceHandle handle) const
{
	if (created_resources_.size() < handle.id().index() + 1)
	{
		return nullptr;
	}
	const auto& resource = created_resources_[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		return nullptr;
	}
}

sakura::graphics::IGpuObject* sakura::graphics::vk::RenderDevice::get_unsafe(const RenderObjectHandle handle) const
{
	if (created_objects_.size() < handle.id().index() + 1)
	{
		handle_error<RenderObjectHandle>::not_find(handle);
		return nullptr;
	}
	const auto& resource = created_objects_[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		handle_error<RenderObjectHandle>::generation_dismatch(handle);
		return nullptr;
	}
}

sakura::graphics::IGpuObject* sakura::graphics::vk::RenderDevice::optional_unsafe(const RenderObjectHandle handle) const
{
	if (created_objects_.size() < handle.id().index() + 1)
	{
		return nullptr;
	}
	const auto& resource = created_objects_[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		return nullptr;
	}
}



bool sakura::graphics::vk::RenderDevice::execute(const RenderGraph& graph_to_execute)
{
	return false;
}


bool sakura::graphics::vk::RenderDevice::present(const SwapChainHandle handle)
{
	if (auto swapChain = get<SwapChain>(handle); swapChain)
	{
		return swapChain->present();
	}
	else
	{
		assert(0 && "SwapChain not found!");
	}
	return true;
}

bool sakura::graphics::vk::RenderDevice::execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl)
{
	// TODO: Move These to Constuction Phase.
	if (hdl.id().index() + 1 > pass_caches_.size())
		pass_caches_.resize(hdl.id().index() + 1); // Create New Cache
	// TODO: Generation Check & Validate
	auto&& cacheFrame = pass_caches_[hdl.id().index()];
	if(!cacheFrame.command_buffer_ && cmdBuffer.begin() != cmdBuffer.end())
	{// create pass cache objects.
		//cacheFrame.pass_info_.
		if (!cacheFrame.command_pool_)
		{
			VkCommandPoolCreateInfo poolInfo{};
			cacheFrame.command_pool_ = 
				createCommandPoolForQueue(master_device().master_queue.family_index, master_device().logical_device);
		}
		// Alloc Per Frame Now.
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = cacheFrame.command_pool_;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		if (vkAllocateCommandBuffers(master_device().logical_device, &allocInfo, &cacheFrame.command_buffer_) != VK_SUCCESS) 
		{
			sakura::error("failed to allocate command buffers!");
		}
	}
	if (!cacheFrame.self_execution_)
	{
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		vkCreateFence(master_device().logical_device, &fenceCreateInfo, nullptr, &cacheFrame.self_execution_);
	}

	// Evaluate.
	for (auto& cmd : cmdBuffer)
	{
		processCommand(cacheFrame, cmd);
	}

	if (cmdBuffer.begin() != cmdBuffer.end())
	{
		VkSubmitInfo submitInfo{};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cacheFrame.command_buffer_;

		if (cacheFrame.toScreen.id() != GenerationalId::UNINITIALIZED)
		{
			if (auto vkChain = get<vk::SwapChain>(cacheFrame.toScreen); vkChain)
			{
				// Semaphore(s) to wait upon before the submitted command buffer starts executing
				submitInfo.pWaitSemaphores = &vkChain->present_complete_semaphore;
				 // One wait semaphore
				submitInfo.waitSemaphoreCount = 1;                          
				// Semaphore(s) to be signaled when command buffers have completed
				submitInfo.pSignalSemaphores = &vkChain->render_to_screen_semaphores[0];
				submitInfo.signalSemaphoreCount = 1;
				submitInfo.pWaitDstStageMask = waitStages;

				vkAcquireNextImageKHR(master_device().logical_device, vkChain->swap_chain,
					UINT64_MAX, vkChain->present_complete_semaphore, VK_NULL_HANDLE, &vkChain->image_index_);
			}
		}
		// Use a fence to wait until the command buffer has finished execution before using it again
		vkWaitForFences(master_device().logical_device, 1, &cacheFrame.self_execution_, VK_TRUE, UINT64_MAX);
		vkResetFences(master_device().logical_device, 1, &cacheFrame.self_execution_);

		vkQueueSubmit(master_device().master_queue.queue, 1, &submitInfo, cacheFrame.self_execution_);
		vkQueueWaitIdle(master_device().master_queue.queue);
	}
	// TODO: Reset on Finish.
	if (cacheFrame.command_buffer_)
		return vkResetCommandBuffer(cacheFrame.command_buffer_, 0);
	else
		return false;
}

// vk-command processors:
using PassCacheFrame = RenderDevice::PassCacheFrame;

bool RenderDevice::execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer)
{
	switch (queue)
	{
	case ERenderQueueType::Copy:
	{
		auto& queueInfo = master_device().transfer_queues[0];
		auto commandPool = master_device().command_pools[queueInfo.family_index];
		VkCommandBuffer commandBuffer = nullptr;
		// Alloc Per Frame Now.
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		if (vkAllocateCommandBuffers(master_device().logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
		{
			sakura::error("failed to allocate command buffers!");
		}
		for (auto& cmd : command_buffer)
		{
			compileCommand(commandBuffer, cmd);
		}
		VkSubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vkQueueSubmit(queueInfo.queue, 1, &submitInfo, queueInfo.block_fence);
		vkWaitForFences(
			master_device().logical_device, 1, &queueInfo.block_fence, VK_TRUE, UINT64_MAX);
		return true;
	}break;

	default:
		assert(0 && "Unimplemented!");
		return false;
	}
	return false;
}

void RenderDevice::compileCommand(VkCommandBuffer commandBuffer, const RenderCommand* command)
{
	switch (command->type())
	{
	case ERenderCommandType::copy_buffer_to_texture:
	{
		auto& cmd = *static_cast<const RenderCommandCopyBufferToTexture*>(command);
		compileCopyBufferToTexture(commandBuffer, cmd);
	}break;
	default:
		sakura::error("NOT IMPLEMENTED!");
		return;
	}
}

void RenderDevice::compileCopyBufferToTexture(VkCommandBuffer commandBuffer,
	const RenderCommandCopyBufferToTexture& command)
{
	
}

void sakura::graphics::vk::RenderDevice::processCommand(PassCacheFrame& cacheFrame, const RenderCommand* command) const
{
	switch (command->type())
	{
	case ERenderCommandType::begin_render_pass:
	{
		auto& cmd = *static_cast<const RenderCommandBeginRenderPass*>(command);
		processCommandBeginRenderPass(master_device().logical_device, cacheFrame, cmd);
	}break;
	case ERenderCommandType::set_scissor_rect:
	{
		auto& cmd = *static_cast<const RenderCommandSetScissorRect*>(command);
		processCommandSetScissorRect(cacheFrame, cmd);
	}break;
	case ERenderCommandType::draw:
	{
		auto& cmd = *static_cast<const RenderCommandDraw*>(command);
		processCommandDraw(cacheFrame, cmd);
	}break;
	case ERenderCommandType::end_render_pass:
	{
		auto& cmd = *static_cast<const RenderCommandEndRenderPass*>(command);
		processCommandEndRenderPass(cacheFrame, cmd);
	}break;
	case ERenderCommandType::update_binding:
	{
		auto& cmd = *static_cast<const RenderCommandUpdateBinding*>(command);
		processCommandUpdateBinding(cacheFrame, cmd);
	}break;
	case ERenderCommandType::draw_instanced_with_args:
	{
		auto& cmd = *static_cast<const RenderCommandDrawInstancedWithArgs*>(command);
		processCommandDrawInstancedWithArgs(cacheFrame, cmd);
	}break;
	default:break;
	}
}

void sakura::graphics::vk::RenderDevice::processCommandUpdateBinding(PassCacheFrame& cache, const RenderCommandUpdateBinding& command) const
{
	processCommandUpdateBinding(cache, command.binder);
}

void sakura::graphics::vk::RenderDevice::processCommandUpdateBinding(
	PassCacheFrame& cache, const sakura::graphics::Binding& binder) const
{
	const auto pipeline = get<RenderPipeline>(cache.pipeline);
	if (cache.binding_sets_.empty())
	{
		if (!cache.descripter_pool_)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; //
			poolSize.descriptorCount = static_cast<uint32_t>(pipeline->binding_layouts.size());

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32_t>(pipeline->binding_layouts.size());

			if (vkCreateDescriptorPool(
				// TODO: mGPU
				master_device().logical_device,
				&poolInfo, nullptr, &cache.descripter_pool_) != VK_SUCCESS)
			{
				sakura::error("failed to create descriptor pool!");
			}
		}
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = cache.descripter_pool_;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(pipeline->binding_layouts.size());
		allocInfo.pSetLayouts = pipeline->binding_layouts.data();

		cache.binding_sets_.resize(pipeline->binding_layouts.size());
		if (vkAllocateDescriptorSets(
			// TODO: mGPU
			master_device().logical_device,
			&allocInfo, cache.binding_sets_.data()) != VK_SUCCESS)
		{
			sakura::error("failed to allocate descriptor sets!");
		}

		// Update all binding sets.
		// Tables
		size_t writeSize = 0u;
		for (auto i = 0u; i < binder.sets.size(); i++) // Sets
		{
			auto& set = binder.sets[i];
			for (auto j = 0; j < set.slots.size(); j++) // Slots
			{
				writeSize++;
			}
		}
		std::vector<VkDescriptorBufferInfo> bufferInfos;
		std::vector<VkDescriptorImageInfo> imageInfos;
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		bufferInfos.reserve(writeSize);
		imageInfos.reserve(writeSize);
		writeDescriptorSets.reserve(writeSize);
		for (auto i = 0u; i < binder.sets.size(); i++) // Sets
		{
			auto& set = binder.sets[i];
			for (auto j = 0; j < set.slots.size(); j++) // Slots
			{
				auto& slot = set.slots[j];
				if (auto bufSet = slot.as_buffer(); bufSet)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					if (auto buf = get<GpuBuffer>(bufSet->buffer); buf)
						bufferInfo.buffer = buf->buffer_;
					else
					{
						assert(0 && "Buffer Not Found!");
					}
					bufferInfo.offset = bufSet->offset;
					bufferInfo.range = bufSet->size;
					bufferInfos.emplace_back(bufferInfo);

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //
					descriptorWrite.dstSet = cache.binding_sets_[i]; // Set
					descriptorWrite.dstBinding = bufSet->slot_index;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.pBufferInfo = &bufferInfos[bufferInfos.size() - 1];
					writeDescriptorSets.emplace_back(descriptorWrite);
				}
				else if (auto samplerSet = slot.as_sampler();samplerSet)
				{
					VkDescriptorImageInfo imageInfo = {};
					if (auto sampler = get<GpuSampler>(samplerSet->sampler); sampler)
						imageInfo.sampler = sampler->sampler;
					imageInfos.emplace_back(imageInfo);
					
					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.dstBinding = samplerSet->slot_index;
					descriptorWrite.dstSet = cache.binding_sets_[i];
					descriptorWrite.pImageInfo= &imageInfos[imageInfos.size() - 1];
					writeDescriptorSets.emplace_back(descriptorWrite);
				}
				else if (auto texSet = slot.as_sampled_texture(); texSet)
				{
					VkDescriptorImageInfo imageInfo = {};
					if (auto texture = get<GpuTexture>(texSet->texture); texture)
						imageInfo.imageView = texture->image_view;
					imageInfos.emplace_back(imageInfo);

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.dstBinding = texSet->slot_index;
					descriptorWrite.dstSet = cache.binding_sets_[i];
					descriptorWrite.pImageInfo = &imageInfos[imageInfos.size() - 1];
					writeDescriptorSets.emplace_back(descriptorWrite);
				}
				else
				{
					assert(0 && "Unimplemented!");
				}
			} // end foreach slots

			vkUpdateDescriptorSets(
				// TODO: mGPU
				master_device().logical_device,
				writeDescriptorSets.size(), writeDescriptorSets.data(),
				0, nullptr
			);
		}
	}

	for (auto i = 0u; i < binder.sets.size(); i++) // Sets
	{
		auto& set = binder.sets[i];
		vkCmdBindDescriptorSets(cache.command_buffer_,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline->pipeline_layout, i, 1, &cache.binding_sets_[i],
			set.slots.size(), set.dynamic_offsets.data());
	}
}

void sakura::graphics::vk::RenderDevice::processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command) const
{
	if (command.instance_draw)
		goto DRAW_INSTANCE;
	{
		const auto& vb_src = command.vb;
		const auto& ib_src = command.ib;
		if (auto vb = get<GpuBuffer>(vb_src.vertex_buffer); vb)
		{
			VkBuffer vertexBuffers[] = { vb->buffer_ };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(cacheFrame.command_buffer_, 0, 1, vertexBuffers, offsets);
		}
		else
		{
			assert(0 && "VB NOT FOUND");
		}

		if (auto ib = get<GpuBuffer>(ib_src.index_buffer); ib)
		{
			VkIndexType indType = VkIndexType::VK_INDEX_TYPE_UINT16;
			if (ib_src.stride == 32)
				indType = VkIndexType::VK_INDEX_TYPE_UINT32;
			if (ib_src.stride == 8)
				indType = VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
			vkCmdBindIndexBuffer(cacheFrame.command_buffer_, ib->buffer_, ib_src.offset, indType);
		}
		else
		{
			assert(0 && "IB NOT FOUND");
		}
	}
DRAW_INSTANCE:
	vkCmdDrawIndexed(cacheFrame.command_buffer_, command.ib.index_count, command.instance_count,
		command.first_index, command.vb.offset, command.first_instance);
}

void sakura::graphics::vk::RenderDevice::processCommandDrawIndirect(PassCacheFrame& cache, const RenderCommandDrawIndirect& command) const
{

}
void sakura::graphics::vk::RenderDevice::processCommandDrawInstancedWithArgs(PassCacheFrame& cache, const RenderCommandDrawInstancedWithArgs& command) const
{
	if (command.binder)
		processCommandUpdateBinding(cache, *command.binder);
	vkCmdDrawIndexed(cache.command_buffer_, command.index_count, command.instance_count,
		command.first_index, command.base_vertex, command.first_instance);
}

void sakura::graphics::vk::RenderDevice::processCommandBeginRenderPass(
	VkDevice device, PassCacheFrame& cache, const RenderCommandBeginRenderPass& cmd) const
{
	sakura::vector<VkImageView> imageViews(cmd.attachments.slots.size());
	
	cache.pipeline = cmd.pipeline;
	const auto pipeline = get<RenderPipeline>(cmd.pipeline);
	extent2d extent = {};
	for (size_t i = 0u; i < cmd.attachments.slots.size(); i++)
	{
		auto& attachment_slot = cmd.attachments.slots[i];
		auto& slot_var = cmd.attachments.slots[i].attachment;

		if (auto swapChain
			= std::get_if<SwapChainHandle>(&slot_var); swapChain)
		{
			cache.toScreen = *swapChain;
			if (auto vkChain = get<vk::SwapChain>(*swapChain); vkChain)
			{
				imageViews[i] = vkChain->back_buffer();
				extent = vkChain->extent();
			}
			else
			{
				sakura::warn("SwapChain Not Found!");
				assert(0 && "INVALID SwapChain Handle");
			}
		}
		else if (auto attachment
			= std::get_if<GpuTextureHandle>(&slot_var); attachment)
		{
			auto native_res = created_resources_[attachment->id().index()];
			// TODO: Support This & Generation Check & Diff Check.
			assert(0 && "Unimplemented");
		}
		else
		{
			assert(0 && "Unknown Handle Bound.");
		}
	}

	
	// !!!!!! REFACTOR THIS !!!!!!
	if (cache.frame_buffer_)
	{
		vkDestroyFramebuffer(device, cache.frame_buffer_, nullptr);
		cache.frame_buffer_ = NULL;
	}
	if (!cache.frame_buffer_)
	{
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = pipeline->pass;
		framebufferInfo.attachmentCount = imageViews.size();
		framebufferInfo.pAttachments = imageViews.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &cache.frame_buffer_) != VK_SUCCESS)
		{
			sakura::error("[DeviceVulkan]: failed to create framebuffer!");
		}
	}
	// !!!!!! REFACTOR THIS !!!!!!


	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(cache.command_buffer_, &beginInfo) != VK_SUCCESS) 
	{
		sakura::error("failed to begin recording command buffer!");
	}
	
	VkRenderPassBeginInfo renderPassBeginInfo{};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = pipeline->pass;
	renderPassBeginInfo.framebuffer = cache.frame_buffer_;
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = { extent.width, extent.height };
	VkClearValue clearColor = { 0.f, 0.0f, 0.0f, 1.0f };
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cache.command_buffer_, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(cache.command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

	if(!imageViews.empty())
	{
		auto& imageView = imageViews[0];
		VkRect2D rect = {};
		rect.extent = { extent.width, extent.height };
		rect.offset = { static_cast<int32>(0), static_cast<int32>(0) };
		vkCmdSetScissor(cache.command_buffer_, 0, 1, &rect);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)extent.width;
		viewport.height = (float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cache.command_buffer_, 0, 1, &viewport);
	}

}

void sakura::graphics::vk::RenderDevice::processCommandEndRenderPass(PassCacheFrame& cache, const RenderCommandEndRenderPass& command) const
{
	vkCmdEndRenderPass(cache.command_buffer_);

	if (vkEndCommandBuffer(cache.command_buffer_) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void sakura::graphics::vk::RenderDevice::processCommandSetScissorRect(PassCacheFrame& cache, const RenderCommandSetScissorRect& command) const
{
	VkRect2D rect = {};
	rect.extent = { command.width, command.height };
	rect.offset = { static_cast<int32>(command.x), static_cast<int32>(command.y) };
	vkCmdSetScissor(cache.command_buffer_, 0, 1, &rect);
}

// vk-specified:

bool sakura::graphics::vk::RenderDevice::validate_surface(VkSurfaceKHR surface) const
{
	// To Make Vulkan Happy.
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(master_device().device, master_device().master_queue.family_index, surface, &presentSupport);
	if (!presentSupport)
	{
		sakura::error("master queue does not support present of this window surface!");
	}
	return presentSupport;
}

VkSurfaceKHR sakura::graphics::vk::RenderDevice::create_and_validate_surface(Window window) const
{
	VkSurfaceKHR surface = createSurface(window, instance_);
	bool sfcValid = validate_surface(surface);
	assert(sfcValid && "Surface Not Supported!");
	return surface;
}



// vk-implementation:
VkFence createFenceForQueue(VkDevice device, bool signalOnCreate)
{
	VkFence fence = nullptr;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	if (signalOnCreate)
		fenceInfo.flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT;
	
	if(vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS)
	{
		sakura::error("[RenderGraphVulkan]: Create Fence Failed!");
	}
	return fence;
}

VkCommandPool createCommandPoolForQueue(uint32_t family_index, VkDevice logical_device, bool resetFlag)
{
	VkCommandPoolCreateInfo poolInfo{};
	VkCommandPool pool = nullptr;
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = family_index;
	if (resetFlag)
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	else
		poolInfo.flags = 0;
	
	if (vkCreateCommandPool(logical_device, &poolInfo, nullptr, &pool) != VK_SUCCESS)
	{
		sakura::error("failed to create command pool!");
	}
	return pool;
}

std::vector<const char*> getRequiredDeviceExtensions(bool asMainDevice)
{
	uint32_t glfwExtensionCount = 0;

	std::vector<const char*> extensions = basic_device_exts;

	if (asMainDevice)
	{
		extensions.insert(extensions.end(), main_device_exts.begin(), main_device_exts.end());
	}

	return extensions;
}

// cn: 检查设备扩展的支持.
// en: Check device extensions support.
// jp: デバイス-エクステンションのチェック.
bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::set<std::string> requiredExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

VulkanDeviceSet RenderDevice::findQueueFamilies(VkPhysicalDevice device, sakura::Window wind)
{
	VulkanDeviceSet pd;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	pd.queue_families.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, pd.queue_families.data());

	VkSurfaceKHR surface = createSurface(wind, instance_);
	uint32 i = 0u;
	bool findPresentQueue = false;
	for (const auto& queueFamily : pd.queue_families)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			pd.graphics_families.emplace_back(i);
			pd.graphics_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });

		}
		else if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			pd.compute_families.emplace_back(i);
			pd.compute_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });
		}
		else if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			pd.transfer_families.emplace_back(i);
			pd.transfer_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			findPresentQueue = true;
			pd.present_families.emplace_back(i);
		}

		i++;
	}
	if (!findPresentQueue)
	{
		sakura::error("Failed to find a present queue!");
	}
	vkDestroySurfaceKHR(instance_, surface, nullptr);
	return pd;
}

VkSurfaceKHR createSurface(sakura::Window window, VkInstance instance)
{
	VkSurfaceKHR surface;
#ifdef SAKURA_TARGET_PLATFORM_WIN
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = static_cast<HWND>(window.handle());
	createInfo.hinstance = GetModuleHandle(nullptr);
	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
	{
		sakura::error("failed to create window surface!");
	}
#else
	static_assert(0, "Implement This!");
#endif
	return surface;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger_, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debug_messenger_, pAllocator);
	}
}

bool checkValidationLayerSupport(const sakura::span<const char* const> layers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	for (const char* layerName : layers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			return false;
		}
	}
	return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageType && VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
	{
		sakura::info("[VkGeneralMessage]: {}", pCallbackData->pMessage);
	}
	else
	{
		sakura::error("[VkValidation/Performance]: {}", pCallbackData->pMessage);
	}
	return VK_FALSE;
}