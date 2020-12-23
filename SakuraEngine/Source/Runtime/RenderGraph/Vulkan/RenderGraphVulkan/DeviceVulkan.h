#pragma once
#include "RenderGraph/RenderDevice.h"
#include "System/Log.h"
#include "vulkan/vulkan.h"
#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "vulkan/vulkan_win32.h"
#endif
#include "Containers/flat_map.hpp"

namespace sakura::graphics::vk
{
	class RenderPipeline;

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

		bool valid(const RenderResourceHandle handle) const override;
		void destroy_resource(const RenderResourceHandle to_destroy) override;

		sakura::string_view get_name() const override;

		QueueIndex request_copy_queue() const override;
		bool execute_let_fly(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override;
		bool execute_let_fly(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override;
		bool execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override;
		bool execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override;
		FenceHandle execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override;
		FenceHandle execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override;
		void wait_idle() override;
    	
		bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) override;
		bool execute(const RenderGraph& graph_to_execute) override;
		bool present(const SwapChainHandle handle) override;
		void terminate() override;

		void write_texture(GpuTextureHandle texture, void const* data, size_t data_size, const TextureSlice& slice,
			const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index) override;
    	
		GpuShaderHandle create_shader(const GpuShaderHandle handle, const ShaderDesc& config) override;
		GpuBufferHandle create_buffer(const GpuBufferHandle handle, const BufferDesc& config) override;
		GpuTextureHandle create_texture(const GpuTextureHandle handle, const TextureDesc& desc) override;

		FenceHandle create_fence(const FenceHandle handle, const FenceDesc& desc) override;
		SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc) override;
		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) override;

		GpuBufferHandle update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size) override;

		IGpuMemoryResource* get_unsafe(const RenderResourceHandle handle) const override;
		IGpuMemoryResource* optional_unsafe(const RenderResourceHandle handle) const override;
		IGpuObject* get_unsafe(const RenderObjectHandle handle) const override;
		IGpuObject* optional_unsafe(const RenderObjectHandle handle) const override;

		// RenderGraph Device Resource.
    	
		sakura::vector<sakura::pair<IGpuMemoryResource*, RenderGraphId::uhalf_t>> created_resources_;
		sakura::vector<sakura::pair<IGpuObject*, RenderGraphId::uhalf_t>> created_objects_;

		// vulkan-specific.
		
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
			// ?
			VkCommandPool command_pool_ = VK_NULL_HANDLE;
			
			RenderPipelineHandle pipeline = GenerationalId::UNINITIALIZED;

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
		void processCommandBeginRenderPass(
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
	template<> struct handle_error<GpuBufferHandle>
	{
		static void not_find(const GpuBufferHandle handle)
		{
			sakura::error("GpuBuffer With Handle {} Not found in Vulkan Resources!", handle);
		}
		static void generation_dismatch(const GpuBufferHandle handle)
		{
			sakura::error("Generation Error : GpuBufferHandle{} has a different generation with Vulkan Resource!", handle);
		}
		static void create_on_existed(const GpuBufferHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: GpuBuffer with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<GpuShaderHandle>
	{
		static void not_find(const GpuShaderHandle handle)
		{
			sakura::error("GpuShader With Handle {} Not found in Vulkan Resources!", handle);
		}
		static void generation_dismatch(const GpuShaderHandle handle)
		{
			sakura::error("Generation Error : GpuShaderHandle{} has a different generation with Vulkan Resource!", handle);
		}
		static void create_on_existed(const GpuShaderHandle handle)
		{
			sakura::error(
				"[RenderDeviceWeb]: GpuShader with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
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
		static_assert(std::is_base_of_v<IGpuMemoryResource, ResourceType>, "[DeviceVulkan::_create_resouce_impl]: ResourceType must be derived from IGpuMemoryResource!");
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
		static_assert(std::is_base_of_v<IGpuObject, ObjectType>, "[DeviceVulkan::_create_object_impl]: ResourceType must be derived from IGpuObject!");
		static_assert(std::is_base_of_v<RenderObjectHandle, Handle>, "[DeviceVulkan::_create_object_impl]: Handle must be derived from RenderObjectHandle!");
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
