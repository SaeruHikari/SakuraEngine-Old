#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include "System/Log.h"

namespace sakura {
	namespace graphics {
		namespace webgpu {
			class RenderPipeline;
		}
	}
}

using namespace sakura::graphics;
using namespace sakura::graphics::webgpu;

//WGPUTextureView backBufView = nullptr;
//static RenderPipeline* ppl = nullptr;
//sakura::vector<WGPUBindGroup> bindGroups;

void RenderDevice::PassCacheFrame::destroy()
{
   
}

void RenderDevice::PassCache::destroy()
{
    for (size_t i = 0u; i < frame_count; i++)
    {
        frames[i].destroy();
    }
}

RenderDevice::PassCache::PassCache(uint8 frameCount)
    :frame_count(frameCount)
{
    frames = static_cast<PassCacheFrame*>(::malloc(sizeof(PassCacheFrame) * frameCount));
	for(size_t i = 0u; i < frameCount; i++)
	{
        new (frames + i) PassCacheFrame();
	}
}

RenderDevice::PassCache::~PassCache()
{
    for (size_t i = 0u; i < frame_count; i++)
    {
        frames[i].~PassCacheFrame();
    }
}

RenderDevice::PassCacheFrame& RenderDevice::PassCache::frame(uint8 currentFrame)
{
    return frames[currentFrame];
}


RenderPipeline* RenderDevice::processCommandBeginRenderPass(
    PassCacheFrame& cacheFrame,
    const RenderCommandBeginRenderPass& cmd, WGPUCommandEncoder* encoder,
    WGPURenderPassEncoder* pass) const
{
    sakura::vector<WGPURenderPassColorAttachmentDescriptor> colorDescs(cmd.attachments.slots.size());
    cacheFrame.texture_views.resize(cmd.attachments.slots.size());
    for (size_t i = 0u; i < cmd.attachments.slots.size(); i++)
    {
        auto& colorDesc = colorDescs[i];
        auto& attachment_slot = cmd.attachments.slots[i];
        auto& slot_var = cmd.attachments.slots[i].attachment;
        auto& texView = cacheFrame.texture_views[i];
        if (auto swapChain
            = std::get_if<SwapChainHandle>(&slot_var); swapChain)
        {
            if (auto get_wgpu_chain = get(*swapChain); get_wgpu_chain)
            {
                auto wgpuChain = static_cast<webgpu::SwapChain*>(get_wgpu_chain);
                texView = wgpuSwapChainGetCurrentTextureView(wgpuChain->swapchain);
                colorDesc.attachment = texView;
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
            auto get_wgpu_res = created_resources[attachment->id().index()];
            // TODO: Support This & Generation Check & Diff Check.
            assert(0 && "Unimplemented");
        }
        else
        {
            assert(0 && "Unknown Handle Bound.");
        }
        colorDesc.loadOp = translate(attachment_slot.load_op);
        colorDesc.storeOp = translate(attachment_slot.store_op);
        colorDesc.clearColor.r = attachment_slot.clear_color.r;
        colorDesc.clearColor.g = attachment_slot.clear_color.g;
        colorDesc.clearColor.b = attachment_slot.clear_color.b;
        colorDesc.clearColor.a = attachment_slot.clear_color.a;
    }
    WGPURenderPassDescriptor renderPass = {};
    renderPass.colorAttachmentCount = static_cast<uint32>(cmd.attachments.slots.size());
    renderPass.colorAttachments = colorDescs.data();
    *pass = wgpuCommandEncoderBeginRenderPass(*encoder, &renderPass);

	if(auto ppl = static_cast<RenderPipeline*>(get(cmd.pipeline));ppl)
	{
        wgpuRenderPassEncoderSetPipeline(*pass, ppl->renderPipeline);
        return ppl;
	}
    else
    {
        assert(0 && "Render Pipeline Not Found!");
    }
    return nullptr;
}

void RenderDevice::processCommandEndRenderPass(const RenderCommandEndRenderPass& command, WGPUCommandEncoder* encoder,
	WGPURenderPassEncoder* pass) const
{
    wgpuRenderPassEncoderEndPass(*pass);
}

void RenderDevice::processCommandSetScissorRect(
    const RenderCommandSetScissorRect& command, WGPUCommandEncoder* encoder,
	WGPURenderPassEncoder* pass) const
{
    wgpuRenderPassEncoderSetScissorRect(*pass,
        command.x, command.y, command.width, command.height);
}

void RenderDevice::processCommandFence(const RenderCommandFence& command, WGPUCommandEncoder* encoder,
	WGPURenderPassEncoder* pass) const
{
	//wgpuQueueCreateFence()
	
}

void RenderDevice::processCommandDrawIndirect(const RenderCommandDrawIndirect& command,
    WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const
{
    const auto buf_hdl = command.indirect_buffer;
    if (auto buf = static_cast<GPUBuffer*>(get(buf_hdl)); buf)
    {
        wgpuRenderPassEncoderDrawIndexedIndirect(*pass, buf->_buffer, command.offset);
    }
}

void RenderDevice::processCommandDraw(PassCacheFrame& cacheFrame, 
    const RenderCommandDraw& command,
	WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const
{
    if (command.instance_draw)
        goto DRAW_INSTANCE;

	{
        const auto& vb_src = command.vb;
        const auto& ib_src = command.ib;

        if (auto vb = static_cast<GPUBuffer*>(get(vb_src.vertex_buffer)); vb)
        {
            wgpuRenderPassEncoderSetVertexBuffer(*pass, 0, vb->_buffer, vb_src.offset, vb_src.stride);
        }
        else
        {
            assert(0 && "VB NOT FOUND");
        }

        if(auto ib = static_cast<GPUBuffer*>(get(ib_src.index_buffer));ib)
		{
#ifdef _____DESP // Emscripten hasn't yet caught up with the API changes
	        wgpuRenderPassEncoderSetIndexBuffer(*pass, ib->_buffer, ib_src.offset, ib_src.stride);
#else
            wgpuRenderPassEncoderSetIndexBufferWithFormat(
                *pass, ib->_buffer, translate(ib_src.format), ib_src.offset, ib_src.stride);
#endif
        }
        else
        {
            assert(0 && "IB NOT FOUND");
        }
	}
DRAW_INSTANCE:
    wgpuRenderPassEncoderDrawIndexed(*pass,
        static_cast<uint32>(command.ib.index_count), command.instance_count, 
        command.first_index, command.base_vertex, command.first_instance);
}

void RenderDevice::processCommand(PassCacheFrame& cacheFrame, const RenderCommand* command,
    WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass) const
{
    switch (command->type())
    {
    case ERenderCommandType::begin_render_pass:
	{
        auto& cmd = *static_cast<const RenderCommandBeginRenderPass*>(command);
        cacheFrame.pipeline = processCommandBeginRenderPass(cacheFrame, cmd, encoder, pass);
	}break;
    case ERenderCommandType::set_scissor_rect:
    {
        auto& cmd = *static_cast<const RenderCommandSetScissorRect*>(command);
        processCommandSetScissorRect(cmd, encoder, pass);
    }break;
    case ERenderCommandType::draw:
    {
        auto& cmd = *static_cast<const RenderCommandDraw*>(command);
        processCommandDraw(cacheFrame, cmd, encoder, pass);
    }break;
    case ERenderCommandType::end_render_pass:
    {
        auto& cmd = *static_cast<const RenderCommandEndRenderPass*>(command);
        processCommandEndRenderPass(cmd, encoder, pass);
    }break;
    case ERenderCommandType::update_binding:
    {
        auto& cmd = *static_cast<const RenderCommandUpdateBinding*>(command);
        processCommandUpdateBinding(cacheFrame, cmd, encoder, pass, cacheFrame.pipeline);
    }break;
	case ERenderCommandType::draw_instanced_with_args:
	{
		auto& cmd = *static_cast<const RenderCommandDrawInstancedWithArgs*>(command);
        processCommandDrawInstancedWithArgs(cacheFrame, cmd, encoder, pass, cacheFrame.pipeline);
	}break;
    default:break;
    }
}

void RenderDevice::processCommandUpdateBinding(RenderDevice::PassCacheFrame& cacheFrame, const sakura::graphics::Binding& binder,
    WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass, RenderPipeline* ppl) const
{
	{
		auto* bindingDesc = &binder;
		if (cacheFrame.bindGroups.size() < bindingDesc->sets.size())
		{
			cacheFrame.bindGroups.resize(bindingDesc->sets.size());
			cacheFrame.entries.resize(bindingDesc->sets.size());
		}
		// Foreach BindingGroup.
		for (auto i = 0u; i < bindingDesc->sets.size(); i++)
		{
			auto& bindGroup = cacheFrame.bindGroups[i];
			auto set = bindingDesc->sets[i];
			// Matching Size.
			if (cacheFrame.entries[i].first.size() != set.slots.size())
			{
				cacheFrame.entries[i].first.resize(set.slots.size());
				goto CREATE_BINDINGS;
			}
			// Check Slot Differences.
			for (auto j = 0u; j < set.slots.size(); j++)
			{
				auto& slot = set.slots[j];
				auto& entry = cacheFrame.entries[i].first[j];
				if (entry.binding == slot.slot_index && entry.offset == slot.offset && entry.size == slot.size)
				{
					if (auto buf = static_cast<GPUBuffer*>(get(slot.buffer)); buf)
						if (entry.buffer == buf->_buffer) continue;
				}
				cacheFrame.entries[i].second = false;
				goto CREATE_BINDINGS;
			}
			cacheFrame.entries[i].second = true;
		CREATE_BINDINGS:
			auto& unchanged = cacheFrame.entries[i].second;
			// Optionally Create Bindings
			if (!unchanged)
			{
				for (auto j = 0u; j < set.slots.size(); j++)
				{
					auto& slot = set.slots[j];
					auto& entry = cacheFrame.entries[i].first[j];
					entry.binding = slot.slot_index; entry.offset = slot.offset; entry.size = slot.size;
					if (auto buf = static_cast<GPUBuffer*>(get(slot.buffer)); buf)
						entry.buffer = buf->_buffer;
					else
						assert(0 && "Buffer Not Found!");
				}
				WGPUBindGroupDescriptor bgDesc = {};
				if (ppl && !cacheFrame.entries.empty())
				{
					bgDesc.layout = ppl->bindingGroups[i]; // Get Layout.
					bgDesc.entryCount = static_cast<uint32>(cacheFrame.entries[i].first.size());
					bgDesc.entries = cacheFrame.entries[i].first.data();
					{
						if (bindGroup)
							wgpuBindGroupRelease(bindGroup);
						bindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);
					}
				}
				else
				{
					assert(0 && "Pipeline as NULL is invalid!");
				}
			}
			if (bindGroup && pass)
				wgpuRenderPassEncoderSetBindGroup(*pass, i, bindGroup, 0u, 0u);
		}// End Foreach BindingGroup.
	}
}

void RenderDevice::processCommandUpdateBinding(PassCacheFrame& cacheFrame,
    const RenderCommandUpdateBinding& command, WGPUCommandEncoder* encoder,
	WGPURenderPassEncoder* pass, RenderPipeline* ppl) const
{
    processCommandUpdateBinding(cacheFrame, command.binder, encoder, pass, ppl);
}

void RenderDevice::processCommandDrawInstancedWithArgs(PassCacheFrame& cacheFrame, const RenderCommandDrawInstancedWithArgs& command,
    WGPUCommandEncoder* encoder, WGPURenderPassEncoder* pass, RenderPipeline* ppl) const
{
    if(command.binder)
        processCommandUpdateBinding(cacheFrame, *command.binder, encoder, pass, ppl);
	wgpuRenderPassEncoderDrawIndexed(*pass, static_cast<uint32>(command.index_count), command.instance_count,
		command.first_index, command.base_vertex, command.first_instance);
}

sakura::string_view RenderDevice::get_name() const
{
    return name;
}

RenderDevice::RenderDevice(const DeviceConfiguration& config)
    :name(config.name)
{
    initPlatformSpecific(config);
    // Get Default Queue
    defaultQueue = wgpuDeviceGetDefaultQueue(device);
    if (!defaultQueue)
    {
        sakura::error("[RenderDeviceWebGPU]: Get Default Queue Failed!");
    }
}

RenderDevice::~RenderDevice()
{
	for(size_t i = 0; i < passCache.size(); i++)
	{
        passCache[i].destroy();
	}
	for(auto& iter : shaderModules)
	{
        wgpuShaderModuleRelease(iter.second);
	}
    wgpuQueueRelease(defaultQueue);
}

WGPUBackendType RenderDevice::get_backend()
{
    return WGPUBackendType_D3D12;
}

EBackend RenderDevice::backend() const
{
    return EBackend::WebGPU;
}

bool RenderDevice::valid(const RenderShaderHandle shader) const
{
    return false;
}

bool RenderDevice::execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl, const size_t frame)
{
	// TODO: Move These to Constuction Phase.
    if (hdl.id().index() + 1 > passCache.size())
        passCache.resize(hdl.id().index() + 1, {3}); // Create New Cache
    // TODO: Generation Check & Validate
    auto&& cacheFrame = passCache[hdl.id().index()].frame(frame);
    {// create pass cache objects.
        cacheFrame.encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);// create encoder
    }

    // Evaluate.
    for (auto& cmd : cmdBuffer)
    {
        processCommand(cacheFrame, cmd, &cacheFrame.encoder, &cacheFrame.pass_encoder);
    }

    if (cacheFrame.pass_encoder)
    {
        wgpuRenderPassEncoderRelease(cacheFrame.pass_encoder);
        WGPUCommandBuffer commands = wgpuCommandEncoderFinish(cacheFrame.encoder, nullptr);// create commands
        //cacheFrame.last_commited += 1;
    	wgpuQueueSubmit(defaultQueue, 1, &commands);
        //wgpuQueueSignal(defaultQueue, cacheFrame.committed_fence, cacheFrame.last_commited);
        wgpuCommandBufferRelease(commands);
    }
	for(size_t i = 0u; i < cacheFrame.texture_views.size(); i++)
	{
        auto& view = cacheFrame.texture_views[i];
        if (view)
        {
            wgpuTextureViewRelease(view);
            view = nullptr;
        }
	}
    wgpuCommandEncoderRelease(cacheFrame.encoder);
    cacheFrame.pass_encoder = nullptr;
    return true;
}

bool RenderDevice::execute(const RenderGraph& graph_to_execute)
{
    sakura::info("EXE RG: Unimplemented");
    return false;
}

bool RenderDevice::present(const SwapChainHandle handle)
{
#ifdef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
    return true;
#endif
    if(auto swapChain = static_cast<SwapChain*>(get(handle));swapChain)
    {
        return swapChain->present();
    }
    else
    {
        assert(0 && "SwapChain not found!");
    }
    return false;
}

void RenderDevice::destroy_resource(const RenderShaderHandle to_destroy)
{

}

RenderBufferHandle RenderDevice::update_buffer(const RenderBufferHandle handle, size_t offset, void* data, size_t size)
{
    auto buf = static_cast<GPUBuffer*>(get(handle));
    wgpuQueueWriteBuffer(defaultQueue, buf->_buffer, offset, data, size);
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

RenderShaderHandle RenderDevice::create_shader(const RenderShaderHandle handle, const ShaderDesc& config)
{
    return _create_resouce_impl<webgpu::GPUShader, RenderShaderHandle>(handle, *this, config);
}

RenderBufferHandle RenderDevice::create_buffer(const RenderBufferHandle handle, const BufferDesc& config)
{
	return _create_resouce_impl<webgpu::GPUBuffer, RenderBufferHandle>(handle, *this, config);
}

RenderAttachmentHandle RenderDevice::create_render_attachment(const RenderAttachmentHandle handle, const Attachment& config)
{
    return handle;
}

SwapChainHandle RenderDevice::create_swap_chain(
    const SwapChainHandle handle, const SwapChainDesc& desc)
{
    return _create_object_impl<SwapChain, SwapChainHandle>(handle, *this, desc);
}

RenderPipelineHandle RenderDevice::create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc)
{
    return _create_object_impl<RenderPipeline, RenderPipelineHandle>(handle, *this, desc);
}

FenceHandle RenderDevice::create_fence(const FenceHandle handle, const FenceDesc& desc)
{
    return _create_object_impl<Fence, FenceHandle>(handle, *this, desc);
}

void RenderDevice::terminate()
{

}
