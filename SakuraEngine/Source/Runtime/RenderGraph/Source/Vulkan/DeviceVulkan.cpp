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
			// en: Filter out devices with insufficient features.
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
			// Create All Queues.
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
				for (auto gfxs = 0u; gfxs < phy_dev.graphics_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.graphics_queues[gfxs].family_index, gfxs, &q);
					phy_dev.graphics_queues[gfxs].queue = q;
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
				for (auto gfxs = 0u; gfxs < phy_dev.compute_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.compute_queues[gfxs].family_index, gfxs, &q);
					phy_dev.compute_queues[gfxs].queue = q;
					findCmptQueue = true;
				}
				for (auto gfxs = 0u; gfxs < phy_dev.transfer_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.transfer_queues[gfxs].family_index, gfxs, &q);
					phy_dev.transfer_queues[gfxs].queue = q;
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

EBackend RenderDevice::backend() const
{
	return EBackend::Vulkan;
}

bool sakura::graphics::vk::RenderDevice::valid(const RenderShaderHandle shader) const
{
	return false;
}

void sakura::graphics::vk::RenderDevice::destroy_resource(const RenderShaderHandle to_destroy)
{
	return;
}

sakura::string_view sakura::graphics::vk::RenderDevice::get_name() const
{
	return name_;
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

sakura::graphics::RenderShaderHandle sakura::graphics::vk::RenderDevice::create_shader(
	const RenderShaderHandle handle, const ShaderDesc& config)
{
	return _create_resouce_impl<vk::GPUShader, RenderShaderHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, 
		config);
}

sakura::graphics::RenderBufferHandle sakura::graphics::vk::RenderDevice::create_buffer(
	const RenderBufferHandle handle, const BufferDesc& config)
{
	return _create_resouce_impl<vk::GPUBuffer, RenderBufferHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, master_device().device,
		config);
}

sakura::graphics::RenderAttachmentHandle sakura::graphics::vk::RenderDevice::create_render_attachment(
	const RenderAttachmentHandle handle, const Attachment& config)
{

	return handle;
}

sakura::graphics::FenceHandle sakura::graphics::vk::RenderDevice::create_fence(const FenceHandle handle, const FenceDesc& desc)
{

	return handle;
}

sakura::graphics::SwapChainHandle sakura::graphics::vk::RenderDevice::create_swap_chain(
	const SwapChainHandle handle, const SwapChainDesc& desc)
{
	return _create_object_impl<SwapChain, SwapChainHandle>(handle, *this, desc);
}

sakura::graphics::RenderPipelineHandle sakura::graphics::vk::RenderDevice::create_render_pipeline(
	const RenderPipelineHandle handle, const RenderPipelineDesc& desc)
{

	return _create_object_impl<vk::RenderPipeline, RenderPipelineHandle>(handle, *this,
		// TODO: mGPU Support.
		master_device().logical_device, 
		desc);
}

sakura::graphics::RenderBufferHandle sakura::graphics::vk::RenderDevice::update_buffer(
	const RenderBufferHandle handle, size_t offset, void* data, size_t size)
{
	auto buf = static_cast<GPUBuffer*>(get(handle));
	void* data_ptr;
	vkMapMemory(buf->owned_device_, buf->buffer_memory_, 0, size, 0, &data_ptr);
		memcpy(data_ptr, data, size);
	vkUnmapMemory(buf->owned_device_, buf->buffer_memory_);
	return handle;
}


IGPUBuffer* RenderDevice::get(const RenderBufferHandle handle) const
{
	return _get_resouce_impl<false, IGPUBuffer>(handle);
}

IGPUShader* RenderDevice::get(const RenderShaderHandle handle) const
{
	return _get_resouce_impl<false, IGPUShader>(handle);
}

IGPUBuffer* RenderDevice::optional(const RenderBufferHandle handle) const
{
	return _get_resouce_impl<true, IGPUBuffer>(handle);
}

IGPUShader* RenderDevice::optional(const RenderShaderHandle handle) const
{
	return _get_resouce_impl<true, IGPUShader>(handle);
}

IRenderPipeline* RenderDevice::optional(const RenderPipelineHandle handle) const
{
	return _get_object_impl<true, IRenderPipeline, RenderPipelineHandle>(handle);
}

ISwapChain* RenderDevice::optional(const SwapChainHandle handle) const
{
	return _get_object_impl<true, ISwapChain, SwapChainHandle>(handle);
}

IFence* RenderDevice::optional(const FenceHandle handle) const
{
	return _get_object_impl<true, IFence, FenceHandle>(handle);
}

IRenderPipeline* RenderDevice::get(const RenderPipelineHandle handle) const
{
	return _get_object_impl<false, IRenderPipeline, RenderPipelineHandle>(handle);
}

ISwapChain* RenderDevice::get(const SwapChainHandle handle) const
{
	return _get_object_impl<false, ISwapChain, SwapChainHandle>(handle);
}

IFence* RenderDevice::get(const FenceHandle handle) const
{
	return _get_object_impl<false, IFence, FenceHandle>(handle);
}


bool sakura::graphics::vk::RenderDevice::execute(const RenderGraph& graph_to_execute)
{
	return false;
}


bool sakura::graphics::vk::RenderDevice::present(const SwapChainHandle handle)
{
	if (auto swapChain = static_cast<SwapChain*>(get(handle)); swapChain)
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
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = master_device().master_queue.family_index;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			if (vkCreateCommandPool(master_device().logical_device, &poolInfo, nullptr, &cacheFrame.command_pool_) != VK_SUCCESS)
			{
				sakura::error("failed to create command pool!");
			}
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

	// Evaluate.
	for (auto& cmd : cmdBuffer)
	{
		processCommand(cacheFrame, cmd);
	}

	if (cmdBuffer.begin() != cmdBuffer.end())
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cacheFrame.command_buffer_;

		if (cacheFrame.toScreen.id() != GenerationalId::UNINITIALIZED)
		{
			if (auto native_chain = get(cacheFrame.toScreen); native_chain)
			{
				auto vkChain = static_cast<vk::SwapChain*>(native_chain);
				VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
				// Semaphore(s) to wait upon before the submitted command buffer starts executing
				submitInfo.pWaitSemaphores = &vkChain->presentCompleteSemaphore;
				 // One wait semaphore
				submitInfo.waitSemaphoreCount = 1;                          
				// Semaphore(s) to be signaled when command buffers have completed
				submitInfo.pSignalSemaphores = &vkChain->render_to_screen_semaphores_[0];
				submitInfo.signalSemaphoreCount = 1;
				submitInfo.pWaitDstStageMask = waitStages;

				vkAcquireNextImageKHR(master_device().logical_device, vkChain->swap_chain_,
					UINT64_MAX, vkChain->presentCompleteSemaphore, VK_NULL_HANDLE, &vkChain->imageIndex);
			}
		}
		vkQueueSubmit(master_device().master_queue.queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(master_device().master_queue.queue);
	}
	// TODO: Reset on Finish.
	vkResetCommandBuffer(cacheFrame.command_buffer_, 0);
	return true;
}

// vk-command processors:
using PassCacheFrame = RenderDevice::PassCacheFrame;
void sakura::graphics::vk::RenderDevice::processCommand(PassCacheFrame& cacheFrame, const RenderCommand* command) const
{
	switch (command->type())
	{
	case ERenderCommandType::begin_render_pass:
	{
		auto& cmd = *static_cast<const RenderCommandBeginRenderPass*>(command);
		cacheFrame.pipeline = processCommandBeginRenderPass(master_device().logical_device, cacheFrame, cmd);
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

void sakura::graphics::vk::RenderDevice::processCommandUpdateBinding(PassCacheFrame& cache, const sakura::graphics::Binding& binder) const
{
	if (cache.binding_sets_.empty())
	{
		if (!cache.descripter_pool_)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; //
			poolSize.descriptorCount = static_cast<uint32_t>(cache.pipeline->binding_layouts_.size());

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32_t>(cache.pipeline->binding_layouts_.size());

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
		allocInfo.descriptorSetCount = static_cast<uint32_t>(cache.pipeline->binding_layouts_.size());
		allocInfo.pSetLayouts = cache.pipeline->binding_layouts_.data();

		cache.binding_sets_.resize(cache.pipeline->binding_layouts_.size());
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
		bufferInfos.reserve(writeSize);
		for (auto i = 0u; i < binder.sets.size(); i++) // Sets
		{
			auto& set = binder.sets[i];
			for (auto j = 0; j < set.slots.size(); j++) // Slots
			{
				auto& slot = set.slots[j];
				VkDescriptorBufferInfo bufferInfo = {};
				if (auto buf = static_cast<GPUBuffer*>(get(slot.buffer)); buf)
					bufferInfo.buffer = buf->buffer_;
				else
				{
					assert(0 && "Buffer Not Found!");
				}
				bufferInfo.offset = slot.offset;
				bufferInfo.range = slot.size;
				bufferInfos.emplace_back(bufferInfo);

				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //
				descriptorWrite.dstSet = cache.binding_sets_[i]; // Set
				descriptorWrite.dstBinding = slot.slot_index;
				descriptorWrite.dstArrayElement = 0;
				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pBufferInfo = &bufferInfos[bufferInfos.size() - 1];

				vkUpdateDescriptorSets(
					// TODO: mGPU
					master_device().logical_device,
					1, &descriptorWrite, 0, nullptr);
			}
		}
	}

	vkCmdBindDescriptorSets(cache.command_buffer_,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		cache.pipeline->pipeline_layout_, 0, binder.sets.size(), cache.binding_sets_.data(),
			binder.dynamic_offsets.size(), binder.dynamic_offsets.data());
}

void sakura::graphics::vk::RenderDevice::processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command) const
{
	if (command.instance_draw)
		goto DRAW_INSTANCE;
	{
		const auto& vb_src = command.vb;
		const auto& ib_src = command.ib;
		if (auto vb = static_cast<GPUBuffer*>(get(vb_src.vertex_buffer)); vb)
		{
			VkBuffer vertexBuffers[] = { vb->buffer_ };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(cacheFrame.command_buffer_, 0, 1, vertexBuffers, offsets);
		}
		else
		{
			assert(0 && "VB NOT FOUND");
		}

		if (auto ib = static_cast<GPUBuffer*>(get(ib_src.index_buffer)); ib)
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

sakura::graphics::vk::RenderPipeline* sakura::graphics::vk::RenderDevice::processCommandBeginRenderPass(
	VkDevice device, PassCacheFrame& cache, const RenderCommandBeginRenderPass& cmd) const
{
	sakura::vector<VkAttachmentDescription> attachDescs(cmd.attachments.slots.size());
	sakura::vector<VkImageView> imageViews(cmd.attachments.slots.size());
	extent2d extent = {};
	for (size_t i = 0u; i < cmd.attachments.slots.size(); i++)
	{
		auto& attachDesc = attachDescs[i];
		auto& attachment_slot = cmd.attachments.slots[i];
		auto& slot_var = cmd.attachments.slots[i].attachment;

		if (auto swapChain
			= std::get_if<SwapChainHandle>(&slot_var); swapChain)
		{
			cache.toScreen = *swapChain;
			if (auto native_chain = get(*swapChain); native_chain)
			{
				auto vkChain = static_cast<vk::SwapChain*>(native_chain);
				attachDesc.format = translate(vkChain->render_format());
				attachDesc.samples = VK_SAMPLE_COUNT_1_BIT;
				attachDesc.loadOp = translate(attachment_slot.load_op);
				attachDesc.storeOp = translate(attachment_slot.store_op);
				// TODO: ?
				attachDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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
			= std::get_if<RenderTextureHandle>(&slot_var); attachment)
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

	cache.pipeline = (RenderPipeline*)get(cmd.pipeline);

	// !!!!!! REFACTOR THIS !!!!!!
	if (cache.frame_buffer_)
		vkDestroyFramebuffer(device, cache.frame_buffer_, nullptr);
	if (!cache.pass_)
	{
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		// NO IMPLEMENTATION.
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32>(cmd.attachments.slots.size());
		renderPassInfo.pAttachments = attachDescs.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		cache.pass_info_ = renderPassInfo;
		
		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &cache.pass_) != VK_SUCCESS)
		{
			sakura::error("failed to create render pass!");
		}
		cache.pipeline->start(cache.pass_);
	}
	{
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = cache.pass_;
		framebufferInfo.attachmentCount = imageViews.size();
		framebufferInfo.pAttachments = imageViews.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &cache.frame_buffer_) != VK_SUCCESS) {
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
	renderPassBeginInfo.renderPass = cache.pass_;
	renderPassBeginInfo.framebuffer = cache.frame_buffer_;
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = { extent.width, extent.height };

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cache.command_buffer_, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(cache.command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, cache.pipeline->pipeline_);

	return (RenderPipeline*)get(cmd.pipeline);
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