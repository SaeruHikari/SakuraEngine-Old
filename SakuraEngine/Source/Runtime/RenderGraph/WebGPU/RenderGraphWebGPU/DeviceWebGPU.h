#pragma once
#include "RenderGraph/IRenderDevice.h"
#include "System/Log.h"
#ifdef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
#include <webgpu/webgpu.h>
#else
#include <dawn/webgpu.h>
#include <dawn_native/DawnNative.h>
#endif

namespace sakura {
	namespace graphics {
		struct RenderPass;
	}
	
}

namespace sakura::graphics::webgpu
{
	class RenderPipeline;
	
	class RenderGraphWebGPUAPI RenderDevice final : public IRenderDevice
	{
	public:
		RenderDevice(const DeviceConfiguration& config);
		~RenderDevice();
		static WGPUBackendType get_backend();
		
		virtual EBackend backend() const override;
		bool valid(const RenderShaderHandle shader) const override;
		sakura::string_view get_name() const override;
		bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) override;
		bool execute(const RenderGraph& graph_to_execute) override;
		bool present(const SwapChainHandle handle) override;
		void terminate() override;

		void destroy_resource(const RenderShaderHandle to_destroy) override;
		RenderShaderHandle create_shader(const RenderShaderHandle handle, const ShaderDesc& config) override;
		RenderBufferHandle create_buffer(const RenderBufferHandle handle, const BufferDesc& config) override;
		RenderAttachmentHandle create_render_attachment(const RenderAttachmentHandle handle, const Attachment& config) override;

		FenceHandle create_fence(const FenceHandle handle, const FenceDesc& desc) override;
		SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc) override;
		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) override;

		RenderBufferHandle update_buffer(const RenderBufferHandle handle, size_t offset, void* data, size_t size) override;

		IGPUMemoryResource* get(const RenderResourceHandle handle) const override;
		IGPUMemoryResource* optional(const RenderResourceHandle handle) const override;
		IGPUObject* get(const RenderGraphHandle handle) const override;
		IGPUObject* optional(const RenderGraphHandle handle) const override;
		
		sakura::vector<sakura::pair<IGPUMemoryResource*, RenderGraphId::uhalf_t>> created_resources_;
		sakura::vector<sakura::pair<IGPUObject*, RenderGraphId::uhalf_t>> created_objects_;
		sakura::unordered_map<sakura::string, WGPUShaderModule> shaderModules;
		struct PassCacheFrame
		{
			sakura::vector<WGPUTextureView> texture_views = sakura::vector<WGPUTextureView>(0); // [NOT FINISHED] Clear Every Frame.
			sakura::vector< sakura::pair<sakura::vector<WGPUBindGroupEntry>, bool> > entries
				= sakura::vector< sakura::pair<sakura::vector<WGPUBindGroupEntry>, bool> >(0); // Finished
			sakura::vector<WGPUBindGroup> bindGroups = sakura::vector<WGPUBindGroup>(0); // Finished
			RenderPipeline* pipeline = nullptr; // 

			WGPUCommandBuffer commands = nullptr;
			WGPUCommandEncoder encoder = nullptr;
			WGPURenderPassEncoder pass_encoder = nullptr;
			WGPUQueue queue = nullptr;
		};
		sakura::vector<PassCacheFrame> passCache;

#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		dawn_native::Adapter adapter;
#endif
		// devices
		WGPUDevice device = nullptr;
		WGPUQueue defaultQueue = nullptr;
		sakura::string name;

	protected:
		template <typename ResourceType, typename Handle, typename... Args>
		Handle _create_resouce_impl(const Handle handle, Args&&... args) noexcept;
		template <typename ObjectType, typename Handle, typename... Args>
		Handle _create_object_impl(const Handle handle, Args&&... args) noexcept;

		
		void initPlatformSpecific(const DeviceConfiguration& config);
		void processCommand(PassCacheFrame& cache, const RenderCommand* command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		void processCommandUpdateBinding(PassCacheFrame& cache,
			const RenderCommandUpdateBinding& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass, RenderPipeline* ppl) const;
		void processCommandUpdateBinding(PassCacheFrame& cache,
			const sakura::graphics::Binding& binder, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass, RenderPipeline* ppl) const;

		void processCommandDrawInstancedWithArgs(PassCacheFrame& cache, const RenderCommandDrawInstancedWithArgs& command,
			WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass, RenderPipeline* ppl) const;
		void processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command,
			WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		void processCommandDrawIndirect(const RenderCommandDrawIndirect& command,
			WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		RenderPipeline* processCommandBeginRenderPass(PassCacheFrame& cache,
			const RenderCommandBeginRenderPass& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		void processCommandEndRenderPass(const RenderCommandEndRenderPass& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		void processCommandSetScissorRect(const RenderCommandSetScissorRect& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
		void processCommandFence(const RenderCommandFence& command, WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const;
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
	template<> struct handle_error<RenderBufferHandle>
	{
		static void not_find(const RenderBufferHandle handle)
		{
			sakura::error("RenderBuffer With Handle {} Not found in WebGPU Resources!", handle);
		}
		static void generation_dismatch(const RenderBufferHandle handle)
		{
			sakura::error("Generation Error : RenderBufferHandle{} has a different generation with WebGPU Resource!", handle);
		}
		static void create_on_existed(const RenderBufferHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: RenderBuffer with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
		}
	};
	template<> struct handle_error<RenderShaderHandle>
	{
		static void not_find(const RenderShaderHandle handle)
		{
			sakura::error("RenderShader With Handle {} Not found in WebGPU Resources!", handle);
		}
		static void generation_dismatch(const RenderShaderHandle handle)
		{
			sakura::error("Generation Error : RenderShaderHandle{} has a different generation with WebGPU Resource!", handle);
		}
		static void create_on_existed(const RenderShaderHandle handle)
		{
			sakura::error("[RenderDeviceWeb]: RenderShader with handle {} alreay created! hash code: {}", handle, size_t(handle.id()));
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
		static_assert(std::is_base_of_v<IGPUMemoryResource, ResourceType>, "[DeviceWebGPU::_create_resouce_impl]: ResourceType must be derived from IGPUMemoryResource!");
		static_assert(std::is_base_of_v<RenderResourceHandle, Handle>, "[DeviceWebGPU::_create_resouce_impl]: Handle must be derived from RenderResourceHandle!");
		static_assert(std::is_base_of_v<typename Handle::ResourceType, ResourceType>, "[DeviceWebGPU::_create_resouce_impl]: Handle must match to it's ResourceType!");
		if (this->optional(handle))
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
		static_assert(std::is_base_of_v<IGPUObject, ObjectType>, "[DeviceWebGPU::_create_object_impl]: ResourceType must be derived from IGPUObject!");
		static_assert(std::is_base_of_v<RenderGraphHandle, Handle>, "[DeviceWebGPU::_create_object_impl]: Handle must be derived from RenderObjectHandle!");
		static_assert(std::is_base_of_v<typename Handle::ObjectType, ObjectType>, "[DeviceWebGPU::_create_object_impl]: Handle must match to it's ObjectType!");
		if (this->optional(handle))
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
