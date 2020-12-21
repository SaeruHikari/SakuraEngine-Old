#pragma once
#include "RenderGraph/IRenderDevice.h"
#include "System/Log.h"
#include "vulkan/vulkan.h"
#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "vulkan/vulkan_win32.h"
#endif
#include "Containers/flat_map.hpp"

namespace sakura::graphics::vk
{
	const bool bEnableValidationLayers = false;
	class RenderPipeline;

	// cn: 启动引擎所需要的最小扩展集合.
	// en: The minimum set of extensions required to start the engine.
	// jp: エンジンをランチするために必要なエクステンション-セット.
	FORCEINLINE const std::vector<const char*> basic_extentions()
	{
		std::vector<const char*> res = {
#ifdef SAKURA_TARGET_PLATFORM_WIN
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
				VK_KHR_SURFACE_EXTENSION_NAME
		};
		if (bEnableValidationLayers)
		{
			res.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return res;
	}

	// cn: 拉起 physics device 对象所要求的最小设备扩展集合.
	// en: The minimum set of device extensions required by the physics device object.
	// jp: 物理デバイスオブジェクトによって必要とされるデバイス-エクステンションの最小集合.
	const std::vector<const char*> basic_device_exts =
	{
		
	};

	// cn: 拉起 main device 对象所要求的最小设备扩展集合.
	// en: The minimum set of device extensions required by the main device object.
	// jp: メイン-デバイスオブジェクトによって必要とされるデバイス-エクステンションの最小集合.
	const std::vector<const char*> main_device_exts =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	struct VulkanQueue
	{
		uint32_t family_index = 0;
		VkQueue queue = VK_NULL_HANDLE;
	};

	struct VulkanDeviceSet
	{
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;

		VkPhysicalDevice device;
		VkDevice logical_device;
		std::vector<VkQueueFamilyProperties> queue_families;

		std::vector<uint32_t> present_families;
		std::vector<uint32_t> graphics_families;
		std::vector<uint32_t> compute_families;
		std::vector<uint32_t> transfer_families;

		std::vector<VulkanQueue> graphics_queues;
		std::vector<VulkanQueue> compute_queues;
		std::vector<VulkanQueue> transfer_queues;

		// cn: graphics queues中一条支持GFX和present的queue.
		//	   其index尽可能小，一般期望它是0.
		// en: A queue in graphics_queues that supports GFX and Present.
		//	   Its index is as small as possible, and is generally expected to be 0.
		// jp: グラフィックスとプレゼントの両方をサポートするグラフィックスキューのキュー.
		//     そのインデックスはできるだけ小さい、一般に0であることになっています.
		VulkanQueue master_queue;
	};
	


    class RenderGraphVulkanAPI RenderDevice final : public IRenderDevice
    {
	public:
		RenderDevice(const DeviceConfiguration& config);
		~RenderDevice();

		virtual EBackend backend() const override;

		bool valid(const RenderShaderHandle shader) const override;
		void destroy_resource(const RenderShaderHandle to_destroy) override;

		sakura::string_view get_name() const override;
		bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) override;
		bool execute(const RenderGraph& graph_to_execute) override;
		bool present(const SwapChainHandle handle) override;
		void terminate() override;

		RenderShaderHandle create_shader(const RenderShaderHandle handle, const ShaderDesc& config) override;
		RenderBufferHandle create_buffer(const RenderBufferHandle handle, const BufferDesc& config) override;
		RenderAttachmentHandle create_render_attachment(const RenderAttachmentHandle handle, const Attachment& config) override;

		FenceHandle create_fence(const FenceHandle handle, const FenceDesc& desc) override;
		SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc) override;
		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) override;

		RenderBufferHandle update_buffer(const RenderBufferHandle handle, size_t offset, void* data, size_t size) override;

		IGPUMemoryResource* get_unsafe(const RenderResourceHandle handle) const override;
		IGPUMemoryResource* optional_unsafe(const RenderResourceHandle handle) const override;
		IGPUObject* get_unsafe(const RenderGraphHandle handle) const override;
		IGPUObject* optional_unsafe(const RenderGraphHandle handle) const override;


		sakura::vector<sakura::pair<IGPUMemoryResource*, RenderGraphId::uhalf_t>> created_resources_;
		sakura::vector<sakura::pair<IGPUObject*, RenderGraphId::uhalf_t>> created_objects_;

		// vulkan-specific
		
		FORCEINLINE const VulkanDeviceSet& master_device() const
		{
			return device_sets_[master_device_index_];
		}
		
		VkInstance instance_;
		VkDebugUtilsMessengerEXT debug_messenger_;
		uint32_t master_device_index_ = 0;
		sakura::vector<VulkanDeviceSet> device_sets_;
		VkSurfaceKHR create_and_validate_surface(Window window) const;
		bool validate_surface(VkSurfaceKHR surface) const;

		struct PassCacheFrame
		{
			VkRenderPassCreateInfo pass_info_ = {};
			VkRenderPass pass_ = VK_NULL_HANDLE;
			
			// ?
			VkCommandPool command_pool_ = VK_NULL_HANDLE;
			
			RenderPipeline* pipeline = nullptr;

			// 易变: !!!Do Something!!!
			VkDescriptorPool descripter_pool_;
			VkFramebuffer frame_buffer_;
			std::vector<VkDescriptorSet> binding_sets_;
			VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;

			VkFence self_execution_ = VK_NULL_HANDLE;
			SwapChainHandle toScreen = GenerationalId::UNINITIALIZED;
		};
		sakura::vector<PassCacheFrame> pass_caches_;
		

	protected:
		void processCommand(PassCacheFrame& cache, const RenderCommand* command) const;
		void processCommandUpdateBinding(PassCacheFrame& cache, const RenderCommandUpdateBinding& command) const;
		void processCommandUpdateBinding(PassCacheFrame& cache, const sakura::graphics::Binding& binder) const;
		void processCommandDrawInstancedWithArgs(
			PassCacheFrame& cache, const RenderCommandDrawInstancedWithArgs& command) const;
		void processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command) const;
		void processCommandDrawIndirect(PassCacheFrame& cache, const RenderCommandDrawIndirect& command) const;
		RenderPipeline* processCommandBeginRenderPass(
			VkDevice device, PassCacheFrame& cache, const RenderCommandBeginRenderPass& command) const;
		void processCommandEndRenderPass(PassCacheFrame& cache, const RenderCommandEndRenderPass& command) const;
		void processCommandSetScissorRect(
			PassCacheFrame& cache, const RenderCommandSetScissorRect& command) const;

	protected:
		sakura::string name_;
		template <typename ResourceType, typename Handle, typename... Args>
		Handle _create_resouce_impl(const Handle handle, Args&&... args) noexcept;
		template <typename ObjectType, typename Handle, typename... Args>
		Handle _create_object_impl(const Handle handle, Args&&... args) noexcept;
	protected:
		VulkanDeviceSet findQueueFamilies(VkPhysicalDevice device, sakura::Window wind);
    };
}


namespace sakura::graphics::vk
{
	template<typename Handle>
	struct RenderGraphVulkanAPI handle_error
	{
		static void not_find(const Handle handle)
		{
			sakura::error("RenderResource/RenderObject With Handle {} Not found in Vulkan Resources!", handle);
		}
		static void generation_dismatch(const Handle handle)
		{
			sakura::error("Generation Error : RenderResourceHandle {} has a different generation with Vulkan Resource!", handle);
		}
		static void create_on_existed(const Handle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: Resource with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<RenderBufferHandle>
	{
		static void not_find(const RenderBufferHandle handle)
		{
			sakura::error("RenderBuffer With Handle {} Not found in Vulkan Resources!", handle);
		}
		static void generation_dismatch(const RenderBufferHandle handle)
		{
			sakura::error("Generation Error : RenderBufferHandle{} has a different generation with Vulkan Resource!", handle);
		}
		static void create_on_existed(const RenderBufferHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: RenderBuffer with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<RenderShaderHandle>
	{
		static void not_find(const RenderShaderHandle handle)
		{
			sakura::error("RenderShader With Handle {} Not found in Vulkan Resources!", handle);
		}
		static void generation_dismatch(const RenderShaderHandle handle)
		{
			sakura::error("Generation Error : RenderShaderHandle{} has a different generation with Vulkan Resource!", handle);
		}
		static void create_on_existed(const RenderShaderHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: RenderShader with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<SwapChainHandle>
	{
		static void not_find(const SwapChainHandle handle)
		{
			sakura::error("RenderSwapChain With Handle {} Not found in Vulkan Objects!", handle);
		}
		static void generation_dismatch(const SwapChainHandle handle)
		{
			sakura::error("Generation Error : RenderSwapChainHandle{} has a different generation with Vulkan Objects!", handle);
		}
		static void create_on_existed(const SwapChainHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: RenderSwapChain with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<RenderPipelineHandle>
	{
		static void not_find(const RenderPipelineHandle handle)
		{
			sakura::error("RenderPipeline With Handle {} Not found in Vulkan Objects!", handle);
		}
		static void generation_dismatch(const RenderPipelineHandle handle)
		{
			sakura::error("Generation Error : RenderPipelineHandle{} has a different generation with Vulkan Objects!", handle);
		}
		static void create_on_existed(const RenderPipelineHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: RenderPipeline with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};

	template <typename ResourceType, typename Handle, typename ... Args>
	Handle RenderDevice::_create_resouce_impl(const Handle handle, Args&&... args) noexcept
	{
		static_assert(std::is_base_of_v<IGPUMemoryResource, ResourceType>, "[DeviceVulkan::_create_resouce_impl]: ResourceType must be derived from IGPUMemoryResource!");
		static_assert(std::is_base_of_v<RenderResourceHandle, Handle>, "[DeviceVulkan::_create_resouce_impl]: Handle must be derived from RenderResourceHandle!");
		static_assert(std::is_base_of_v<typename Handle::ResourceType, ResourceType>, "[DeviceVulkan::_create_resouce_impl]: Handle must match to it's ResourceType!");
		if (this->optional<ResourceType>(handle))
		{
			handle_error<Handle>::create_on_existed(handle);
		}
		else
		{
			if (created_resources_.size() < handle.id().index() + 1)
				created_resources_.resize(handle.id().index() + 1);
			auto newRes = new ResourceType(handle, std::forward<Args>(args)...);
			created_resources_[handle.id().index()] = sakura::make_pair(newRes, handle.id().generation());
		}
		return handle;
	}

	template <typename ObjectType, typename Handle, typename ... Args>
	Handle RenderDevice::_create_object_impl(const Handle handle, Args&&... args) noexcept
	{
		static_assert(std::is_base_of_v<IGPUObject, ObjectType>, "[DeviceVulkan::_create_object_impl]: ResourceType must be derived from IGPUObject!");
		static_assert(std::is_base_of_v<RenderGraphHandle, Handle>, "[DeviceVulkan::_create_object_impl]: Handle must be derived from RenderObjectHandle!");
		static_assert(std::is_base_of_v<typename Handle::ObjectType, ObjectType>, "[DeviceVulkan::_create_object_impl]: Handle must match to it's ObjectType!");
		if (this->optional<ObjectType>(handle))
		{
			handle_error<Handle>::create_on_existed(handle);
		}
		else
		{
			if (created_objects_.size() < handle.id().index() + 1)
				created_objects_.resize(handle.id().index() + 1);
			auto newRes = new ObjectType(handle, std::forward<Args>(args)...);
			created_objects_[handle.id().index()] = sakura::make_pair(newRes, handle.id().generation());
		}
		return handle;
	}
}
