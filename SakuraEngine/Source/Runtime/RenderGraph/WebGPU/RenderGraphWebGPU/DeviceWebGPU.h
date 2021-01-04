#pragma once
#include "RenderGraph/RenderDevice.h"
#include "System/Log.h"
#ifdef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
#include <webgpu/webgpu.h>
#else
#include <dawn/webgpu.h>
#include <dawn_native/DawnNative.h>
#endif

namespace sakura::graphics
{
	struct RenderPass;
}

namespace sakura::graphics::webgpu
{
	class RenderPipeline;
	struct RenderGraphWebGPUAPI RenderFrameWebGPU : public IRenderFrame
	{
		virtual ~RenderFrameWebGPU();
		const void* raw() const override;
		void* raw() override;
		bool valid(const size_t currentFrame) const override;

	protected:
		size_t this_frame_ = 0;
	};
	
	class RenderGraphWebGPUAPI RenderDevice final : public IRenderDevice
	{
	public:
		RenderDevice(const RenderGraph& renderGraph, const DeviceConfiguration& config);
		~RenderDevice();
		static WGPUBackendType get_backend();
		
		virtual EBackend backend() const override;

		bool valid(const RenderResourceHandle handle) const override;
		void destroy(const RenderResourceHandle to_destroy) override;

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

		GpuShaderHandle create_shader(const GpuShaderHandle handle, const ShaderDescriptor& config) override;
		GpuBufferHandle create_buffer(const GpuBufferHandle handle, const BufferDescriptor& config) override;
		GpuTextureHandle create_texture(const GpuTextureHandle handle, const TextureDescriptor& desc) override;
		GpuSamplerHandle create_sampler(const GpuSamplerHandle handle, const SamplerDescriptor& desc) override;
		ComputePipelineHandle create_compute_pipeline(
			const ComputePipelineHandle handle, const ComputePipelineDescriptor& desc) override;

		void write_texture(GpuTextureHandle texture, void const* data, size_t data_size, const TextureSlice& slice,
			const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index = QueueIndex(-1)) override;
		
		FenceHandle create_fence(const FenceHandle handle, const FenceDescriptor& desc) override;
		SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDescriptor& desc) override;
		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDescriptor& desc) override;

		GpuBufferHandle update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size) override;
		//GpuBufferHandle resize(const GpuBufferHandle handle, size_t size, size_t from = 0, size_t to = 0) override;

		[[nodiscard]] IGpuMemoryResource* get_unsafe(const RenderResourceHandle handle) const override;
		[[nodiscard]] IGpuMemoryResource* optional_unsafe(const RenderResourceHandle handle) const override;
		[[nodiscard]] IGpuObject* get_unsafe(const RenderObjectHandle handle) const override;
		[[nodiscard]] IGpuObject* optional_unsafe(const RenderObjectHandle handle) const override;
		
		sakura::vector<sakura::pair<IGpuMemoryResource*, RenderGraphId::uhalf_t>> created_resources;
		sakura::vector<sakura::pair<IGpuObject*, RenderGraphId::uhalf_t>> created_objects;
		sakura::unordered_map<sakura::string, WGPUShaderModule> shader_modules;
		struct PassCacheFrame
		{
			sakura::vector<WGPUTextureView> texture_views
				= sakura::vector<WGPUTextureView>(0); // [NOT FINISHED] Clear Every Frame.
			
			sakura::vector< sakura::pair<sakura::vector<Binding>, bool> > entries
				= sakura::vector< sakura::pair<sakura::vector<Binding>, bool> >(0); // Finished

			sakura::vector<WGPUBindGroup> bind_groups = sakura::vector<WGPUBindGroup>(0); // Finished

			RenderPipeline* pipeline = nullptr; // 
			WGPUCommandBuffer commands = nullptr;
			WGPUCommandEncoder encoder = nullptr;
			WGPURenderPassEncoder pass_encoder = nullptr;
			WGPUQueue queue = nullptr;
		};
		sakura::vector<PassCacheFrame> pass_cache;

#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		dawn_native::Adapter adapter;
#endif
		// devices_
		WGPUDevice device = nullptr;
		WGPUQueue default_queue = nullptr;
		sakura::string name;

		const RenderGraph& render_graph;
	protected:
		template <typename ResourceType, typename Handle, typename... Args>
		Handle _create_resouce_impl(const Handle handle, Args&&... args) noexcept;
		template <typename ObjectType, typename Handle, typename... Args>
		Handle _create_object_impl(const Handle handle, Args&&... args) noexcept;
		
		void initPlatformSpecific(const DeviceConfiguration& config);

		void processCommand(PassCacheFrame& cache, const RenderCommand* command) const;
		void compileCommand(WGPUCommandEncoder encoder, const RenderCommand* command) const;
		
		// Graphics
		void processCommandUpdateBinding(PassCacheFrame& cache, const RenderCommandUpdateBinding& command) const;
		void processCommandUpdateBinding(PassCacheFrame& cache, const sakura::graphics::Binding& binder) const;
		void processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command) const;
		void processCommandSetVB(PassCacheFrame& cacheFrame, const RenderCommandSetVB& command) const;
		void processCommandSetIB(PassCacheFrame& cacheFrame, const RenderCommandSetIB& command) const;
		void processCommandDrawIndirect(PassCacheFrame& cache, const RenderCommandDrawIndirect& command) const;
		void processCommandBeginRenderPass(PassCacheFrame& cache, const RenderCommandBeginRenderPass& command) const;
		void processCommandEndRenderPass(PassCacheFrame& cache, const RenderCommandEndRenderPass& command) const;
		void processCommandSetScissorRect(PassCacheFrame& cache, const RenderCommandSetScissorRect& command) const;
		void processCommandFence(const RenderCommandFence& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;

		// Copy/Transfer
		void processCommandCopyTextureToTexture(WGPUCommandEncoder encoder, const RenderCommandCopyTextureToTexture& command) const;
		void processCommandCopyTextureToBuffer(WGPUCommandEncoder encoder, const RenderCommandCopyTextureToBuffer& command) const;
		void processCommandCopyBufferToBuffer(WGPUCommandEncoder encoder, const RenderCommandCopyBufferToBuffer& command) const;
		void processCommandCopyBufferToTexture(WGPUCommandEncoder encoder, const RenderCommandCopyBufferToTexture& command) const;
	};
}

namespace sakura::graphics::webgpu
{	
	template<typename Handle>
	struct RenderGraphWebGPUAPI handle_error
	{
		static void not_find(const Handle handle)
		{
			sakura::error("RenderResource/RenderObject With Handle {} Not found in WebGPU Resources!", handle);
		}
		static void generation_dismatch(const Handle handle)
		{
			sakura::error("Generation Error : RenderResourceHandle {} has a different generation with WebGPU Resource!", handle);
		}
		static void create_on_existed(const Handle handle)
		{
			sakura::error("[RenderDeviceWeb]: Resource with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<GpuBufferHandle>
	{
		static void not_find(const GpuBufferHandle handle)
		{
			sakura::error("GpuBuffer With Handle {} Not found in WebGPU Resources!", handle);
		}
		static void generation_dismatch(const GpuBufferHandle handle)
		{
			sakura::error("Generation Error : GpuBufferHandle{} has a different generation with WebGPU Resource!", handle);
		}
		static void create_on_existed(const GpuBufferHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: GpuBuffer with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<GpuShaderHandle>
	{
		static void not_find(const GpuShaderHandle handle)
		{
			sakura::error("GpuShader With Handle {} Not found in WebGPU Resources!", handle);
		}
		static void generation_dismatch(const GpuShaderHandle handle)
		{
			sakura::error("Generation Error : GpuShaderHandle{} has a different generation with WebGPU Resource!", handle);
		}
		static void create_on_existed(const GpuShaderHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: GpuShader with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<SwapChainHandle>
	{
		static void not_find(const SwapChainHandle handle)
		{
			sakura::error("RenderSwapChain With Handle {} Not found in WebGPU Objects!", handle);
		}
		static void generation_dismatch(const SwapChainHandle handle)
		{
			sakura::error("Generation Error : RenderSwapChainHandle{} has a different generation with WebGPU Objects!", handle);
		}
		static void create_on_existed(const SwapChainHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: RenderSwapChain with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<RenderPipelineHandle>
	{
		static void not_find(const RenderPipelineHandle handle)
		{
			sakura::error("RenderPipeline With Handle {} Not found in WebGPU Objects!", handle);
		}
		static void generation_dismatch(const RenderPipelineHandle handle)
		{
			sakura::error("Generation Error : RenderPipelineHandle{} has a different generation with WebGPU Objects!", handle);
		}
		static void create_on_existed(const RenderPipelineHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: RenderPipeline with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};

	template <typename ResourceType, typename Handle, typename ... Args>
	Handle RenderDevice::_create_resouce_impl(const Handle handle, Args&&... args) noexcept
	{
		static_assert(std::is_base_of_v<IGpuMemoryResource, ResourceType>, "[DeviceWebGPU::_create_resouce_impl]: ResourceType must be derived from IGpuMemoryResource!");
		static_assert(std::is_base_of_v<RenderResourceHandle, Handle>, "[DeviceWebGPU::_create_resouce_impl]: Handle must be derived from RenderResourceHandle!");
		static_assert(std::is_base_of_v<typename Handle::ResourceType, ResourceType>, "[DeviceWebGPU::_create_resouce_impl]: Handle must match to it's ResourceType!");
		if (this->optional<ResourceType>(handle))
		{
			handle_error<Handle>::create_on_existed(handle);
		}
		else
		{
			if (created_resources.size() < handle.id().index() + 1)
				created_resources.resize(handle.id().index() + 1);
			auto newRes = new ResourceType(handle, std::forward<Args>(args)...);
			created_resources[handle.id().index()] = sakura::make_pair(newRes, handle.id().generation());
		}
		return handle;
	}

	template <typename ObjectType, typename Handle, typename ... Args>
	Handle RenderDevice::_create_object_impl(const Handle handle, Args&&... args) noexcept
	{
		static_assert(std::is_base_of_v<IGpuObject, ObjectType>, "[DeviceWebGPU::_create_object_impl]: ResourceType must be derived from IGpuObject!");
		static_assert(std::is_base_of_v<RenderObjectHandle, Handle>, "[DeviceWebGPU::_create_object_impl]: Handle must be derived from RenderObjectHandle!");
		static_assert(std::is_base_of_v<typename Handle::ObjectType, ObjectType>, "[DeviceWebGPU::_create_object_impl]: Handle must match to it's ObjectType!");
		if (this->optional<ObjectType>(handle))
		{
			handle_error<Handle>::create_on_existed(handle);
		}
		else
		{
			if (created_objects.size() < handle.id().index() + 1)
				created_objects.resize(handle.id().index() + 1);
			auto newRes = new ObjectType(handle, std::forward<Args>(args)...);
			created_objects[handle.id().index()] = sakura::make_pair(newRes, handle.id().generation());
		}
		return handle;
	}
}
