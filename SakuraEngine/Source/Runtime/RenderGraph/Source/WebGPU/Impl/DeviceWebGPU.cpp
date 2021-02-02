#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include "System/Log.h"
#include "TaskSystem/TaskSystem.h"

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
//sakura::vector<WGPUBindGroup> bind_groups;
void RenderDevice::processCommandBeginRenderPass(PassCacheFrame& cacheFrame, const RenderCommandBeginRenderPass& cmd) 
{
    auto& pass = cacheFrame.pass_encoder;
    auto& encoder = cacheFrame.encoder;
    WGPURenderPassDescriptor renderPass = {};

    sakura::vector<WGPURenderPassColorAttachmentDescriptor> colorDescs(cmd.attachments.slots.size());
    WGPURenderPassDepthStencilAttachmentDescriptor dsDesc = {};
    // DS Attachment.
    if (auto ds = optional<webgpu::GpuTexture>(cmd.depth_stencil.ds_attachment); ds)
    {
        dsDesc.attachment = ds->default_view;
        if (cmd.depth_stencil.clear_depth >= 0.f)
            dsDesc.clearDepth = cmd.depth_stencil.clear_depth;
        dsDesc.depthLoadOp = translate(cmd.depth_stencil.depth_load_op);
        dsDesc.depthStoreOp = translate(cmd.depth_stencil.depth_store_op);
        dsDesc.depthReadOnly = cmd.depth_stencil.clear_depth < 0.f;
       
        if(cmd.depth_stencil.clear_stencil >= 0.f)
            dsDesc.clearStencil = cmd.depth_stencil.clear_stencil;
        dsDesc.stencilLoadOp = translate(cmd.depth_stencil.stencil_load_op);
        dsDesc.stencilStoreOp = translate(cmd.depth_stencil.stencil_store_op);
        dsDesc.stencilReadOnly = cmd.depth_stencil.clear_stencil < 0.f;

        renderPass.depthStencilAttachment = &dsDesc;
    }
    // Color Attachments.
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
            if (auto wgpuChain = get<webgpu::SwapChain>(*swapChain); wgpuChain)
            {
                texView = wgpuSwapChainGetCurrentTextureView(wgpuChain->swapchain);
                colorDesc.attachment = texView;
                //colorDesc.resolveTarget = texView;
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
    renderPass.colorAttachmentCount = static_cast<uint32>(cmd.attachments.slots.size());
    renderPass.colorAttachments = colorDescs.data();

    pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);

	if(auto ppl = get<RenderPipeline>(cmd.pipeline);ppl)
	{
        wgpuRenderPassEncoderSetPipeline(pass, ppl->renderPipeline);
        cacheFrame.pipeline = ppl;
	}
    else
    {
        assert(0 && "Render Pipeline Not Found!");
    }
}

void RenderDevice::processCommandEndRenderPass(PassCacheFrame& cache, const RenderCommandEndRenderPass& command) 
{
    wgpuRenderPassEncoderEndPass(cache.pass_encoder);
}

void RenderDevice::processCommandSetScissorRect(PassCacheFrame& cache, const RenderCommandSetScissorRect& command) 
{
    wgpuRenderPassEncoderSetScissorRect(cache.pass_encoder,
        command.x, command.y, command.width, command.height);
}

void RenderDevice::processCommandFence(const RenderCommandFence& command, WGPUCommandEncoder* encoder,
	WGPURenderPassEncoder* pass) 
{
	//wgpuQueueCreateFence()
	
}

void RenderDevice::processCommandCopyTextureToTexture(
    WGPUCommandEncoder encoder, const RenderCommandCopyTextureToTexture& command) const
{
    WGPUTextureCopyView src = translate(command.src_slice, get<GpuTexture>(command.src)->texture);
    WGPUTextureCopyView dst = translate(command.dst_slice, get<GpuTexture>(command.dst)->texture);
    WGPUExtent3D size = { command.copy_size.width, command.copy_size.height, command.copy_size.depth };
    wgpuCommandEncoderCopyTextureToTexture(encoder, &src, &dst, &size);
}

void RenderDevice::processCommandCopyTextureToBuffer(
    WGPUCommandEncoder encoder, const RenderCommandCopyTextureToBuffer& command) const
{
    WGPUTextureCopyView src = translate(command.src_slice, get<GpuTexture>(command.src)->texture);
    WGPUBufferCopyView dst = translate(command.dst_layout, get<GPUBuffer>(command.dst)->buffer);
    WGPUExtent3D size = { command.copy_size.width, command.copy_size.height, command.copy_size.depth };
    wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &size);
}

void RenderDevice::processCommandCopyBufferToBuffer(
    WGPUCommandEncoder encoder, const RenderCommandCopyBufferToBuffer& command) const
{
    wgpuCommandEncoderCopyBufferToBuffer(encoder,
        get<GPUBuffer>(command.src)->buffer, command.src_offset,
        get<GPUBuffer>(command.dst)->buffer, command.dst_offset, command.copy_size);
}

void RenderDevice::processCommandCopyBufferToTexture(
    WGPUCommandEncoder encoder, const RenderCommandCopyBufferToTexture& command) const
{
	
	WGPUTextureCopyView dst = translate(command.dst_slice, get<GpuTexture>(command.dst)->texture);
    WGPUBufferCopyView src = translate(command.layout, get<GPUBuffer>(command.src)->buffer);
	WGPUExtent3D size = { command.copy_size.width, command.copy_size.height, command.copy_size.depth };
    wgpuCommandEncoderCopyBufferToTexture(encoder, &src, &dst, &size);
}


void RenderDevice::processCommandDrawIndirect(PassCacheFrame& cache, const RenderCommandDrawIndirect& command) 
{
    const auto buf_hdl = command.indirect_buffer;
    if (auto buf = get<GPUBuffer>(buf_hdl); buf)
    {
        wgpuRenderPassEncoderDrawIndexedIndirect(cache.pass_encoder, buf->buffer, command.offset);
    }
}

void RenderDevice::processCommandSetVB(PassCacheFrame& cacheFrame, const RenderCommandSetVB& command) 
{
    if (auto vb = get<GPUBuffer>(command.vertex_buffer); vb)
    {
        wgpuRenderPassEncoderSetVertexBuffer(cacheFrame.pass_encoder, command.slot, vb->buffer, command.offset, command.stride);
    }
    else
    {
        assert(0 && "VB NOT FOUND");
    }
}

void RenderDevice::processCommandSetIB(PassCacheFrame& cacheFrame, const RenderCommandSetIB& command) 
{
    if (auto ib = get<GPUBuffer>(command.index_buffer); ib)
    {
#ifdef _____DESPERATED // Emscripten hasn't yet caught up with the API changes
        wgpuRenderPassEncoderSetIndexBuffer(*pass, ib->buffer, ib_src.offset, ib_src.stride);
#else
        wgpuRenderPassEncoderSetIndexBuffer(
            cacheFrame.pass_encoder, ib->buffer, translate(command.format), command.offset, command.stride);
#endif
    }
    else
    {
        assert(0 && "IB NOT FOUND");
    }
}

void RenderDevice::updateBindGroups(PassCacheFrame& cacheFrame)
{
    // Update Bindings
    for (auto i = 0u; i < cacheFrame.sets.size(); i++)
    {
        auto& set = cacheFrame.sets[i];
        WGPUBindGroupDescriptor bgDesc = {};
        sakura::vector<WGPUBindGroupEntry> sets = sakura::vector<WGPUBindGroupEntry>(set.first.size());
        auto& bindGroup = cacheFrame.bind_groups[i];

        for (auto& binder : set.first)
        {
            bgDesc.layout = cacheFrame.pipeline->bindingGroups[binder.set()]; // Get Layout.
            bgDesc.entryCount = static_cast<uint32>(cacheFrame.sets[binder.set()].first.size());

            auto& entry = sets[binder.bind()];
            auto& slot = set.first[binder.bind()];

            entry.binding = slot.bind();
            if (auto buf = slot.as_buffer(); buf)
            {
                entry.size = buf->size;
                entry.offset = 0;
                if (auto buff = get<GPUBuffer>(buf->buffer); buff)
                    entry.buffer = buff->buffer;
                else
                    assert(0 && "Buffer Not Found!");
            }
            else if (auto sampler = slot.as_sampler(); sampler)
            {
                entry.sampler = get<GpuSampler>(sampler->sampler)->sampler;
            }
            else if (auto tex = slot.as_sampled_texture(); tex)
            {
                if (tex->follow_default)
                {
                    entry.textureView = get<GpuTexture>(tex->texture)->default_view;
                }
                else
                {
                    assert(0 && "Unimplemented!");
                }
            }
        }

        bgDesc.entries = sets.data();
        if (bindGroup)
            wgpuBindGroupRelease(bindGroup);
        bindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);
    }
}

void RenderDevice::setBindGroups(PassCacheFrame& cacheFrame)
{
    auto& binding_layout = cacheFrame.pipeline->desc.binding_layout;
    for (auto i = 0u; i < binding_layout.tables.size(); i++)
    {
        auto& set = cacheFrame.sets[i];
        auto& table = binding_layout.tables[i];
        {
            // Remove Dirty Flag.
            set.second = false;

            auto& bindGroup = cacheFrame.bind_groups[i];
            std::vector<uint32> dynamic_offsets;
            dynamic_offsets.reserve(cacheFrame.sets[i].first.size());

            for (auto j = 0u; j < table.slots.size(); j++)
            {
                auto& slot = table.slots[j];
                auto& bind = cacheFrame.sets[i].first[j];
                if (slot.binding_type == BindingLayout::UniformBuffer)
                {
                    dynamic_offsets.emplace_back(bind.offset());
                }
            }

            if (bindGroup && cacheFrame.pass_encoder)
                wgpuRenderPassEncoderSetBindGroup(cacheFrame.pass_encoder, i, bindGroup,
                    dynamic_offsets.size(),
                    dynamic_offsets.size() == 0 ? nullptr : dynamic_offsets.data());
        }
    }
}

void RenderDevice::processCommandDraw(PassCacheFrame& cacheFrame, const RenderCommandDraw& command)
{
    updateBindGroups(cacheFrame);
    setBindGroups(cacheFrame);
    
    wgpuRenderPassEncoderDrawIndexed(cacheFrame.pass_encoder,
        static_cast<uint32>(command.index_count), command.instance_count, 
        command.first_index, command.base_vertex, command.first_instance);
}

void RenderDevice::processCommand(PassCacheFrame& cacheFrame, const RenderCommand* command)
{
    switch (command->type())
    {
    case ERenderCommandType::begin_render_pass:
	{
        auto& cmd = *static_cast<const RenderCommandBeginRenderPass*>(command);
        processCommandBeginRenderPass(cacheFrame, cmd);
	}break;
    case ERenderCommandType::draw_indirect:
    {
        auto& cmd = *static_cast<const RenderCommandDrawIndirect*>(command);
        processCommandDrawIndirect(cacheFrame, cmd);
    }break;
    case ERenderCommandType::set_scissor_rect:
    {
        auto& cmd = *static_cast<const RenderCommandSetScissorRect*>(command);
        processCommandSetScissorRect(cacheFrame, cmd);
    }break;
    case ERenderCommandType::set_ib:
    {
        auto& cmd = *static_cast<const RenderCommandSetIB*>(command);
        processCommandSetIB(cacheFrame, cmd);
    }break;
    case ERenderCommandType::set_vbs:
    {
        auto& cmd = *static_cast<const RenderCommandSetVB*>(command);
        processCommandSetVB(cacheFrame, cmd);
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
    case ERenderCommandType::copy_texture_to_texture:
    {
        auto& cmd = *static_cast<const RenderCommandCopyTextureToTexture*>(command);
        processCommandCopyTextureToTexture(cacheFrame.encoder, cmd);
    }break;
    case ERenderCommandType::copy_texture_to_buffer:
    {
        auto& cmd = *static_cast<const RenderCommandCopyTextureToBuffer*>(command);
        processCommandCopyTextureToBuffer(cacheFrame.encoder, cmd);
    }break;
    case ERenderCommandType::copy_buffer_to_buffer:
    {
        auto& cmd = *static_cast<const RenderCommandCopyBufferToBuffer*>(command);
        processCommandCopyBufferToBuffer(cacheFrame.encoder, cmd);
    }break;
    case ERenderCommandType::copy_buffer_to_texture:
    {
        auto& cmd = *static_cast<const RenderCommandCopyBufferToTexture*>(command);
        processCommandCopyBufferToTexture(cacheFrame.encoder, cmd);
    }break;
    default:break;
    }
}

void RenderDevice::processCommandUpdateBinding(RenderDevice::PassCacheFrame& cacheFrame, const sakura::graphics::Binding& binder)
{
    // TODO: REFACTOR THIS.
    const auto& bl = cacheFrame.pipeline->desc.binding_layout;// render_graph.builder(hdl).binding_layout;
	if(cacheFrame.sets.empty())
	{
        cacheFrame.sets.resize(bl.tables.size());
        for (auto i = 0u; i < cacheFrame.sets.size(); i++)
        {
            cacheFrame.sets[i].first.resize(bl.tables[i].slots.size());

            cacheFrame.sets[i].second = false;
        }
	}
    if (cacheFrame.bind_groups.empty())
    {
        cacheFrame.bind_groups.resize(bl.tables.size());
    }
	
    auto* bindingDesc = &binder;
    auto& set = cacheFrame.sets[binder.set()];
    if (cacheFrame.bind_groups.size() < bindingDesc->set() + 1 || cacheFrame.sets.size() < bindingDesc->set() + 1)
    {
        sakura::error("Binding Set OverRange!");
    }
    else if (cacheFrame.sets[bindingDesc->set()].first.size() < bindingDesc->bind() + 1)
    {
        sakura::error("Binding Slot OverRange!");
    }
	
    // Create New Binding on Slot.
    if (cacheFrame.sets[bindingDesc->set()].first.at(bindingDesc->bind()) != binder)
    {
        if (cacheFrame.pipeline && !cacheFrame.sets.empty())
        {
            set.second = true; // Set Dirty.
            set.first[binder.bind()] = binder;
        }
        else
        {
            assert(0 && "Pipeline as NULL is invalid!");
        }
    }
}

void RenderDevice::processCommandUpdateBinding(PassCacheFrame& cacheFrame, const RenderCommandUpdateBinding& command) 
{
    processCommandUpdateBinding(cacheFrame, command.binder);
}

sakura::string_view RenderDevice::get_name() const
{
    return name;
}

QueueIndex RenderDevice::request_copy_queue() const
{
	// WebGPU supports only main queue.
    return 0;
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

bool RenderDevice::execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer)
{
    assert(0 && "Unimplemented!");
    return false;
}

FenceHandle RenderDevice::execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer)
{
	assert(0 && "Unimplemented!");
    return GenerationalId::UNINITIALIZED;
}

void RenderDevice::compileCommand(WGPUCommandEncoder encoder, const RenderCommand* command) 
{
	switch (command->type())
	{
    case ERenderCommandType::copy_texture_to_buffer:
    {
        auto& cmd = *static_cast<const RenderCommandCopyTextureToBuffer*>(command);
        processCommandCopyTextureToBuffer(encoder, cmd);
    }break;
    case ERenderCommandType::copy_texture_to_texture:
    {
        auto& cmd = *static_cast<const RenderCommandCopyTextureToTexture*>(command);
        processCommandCopyTextureToTexture(encoder, cmd);
    }break;
    case ERenderCommandType::copy_buffer_to_buffer:
    {
        auto& cmd = *static_cast<const RenderCommandCopyBufferToBuffer*>(command);
        processCommandCopyBufferToBuffer(encoder, cmd);
    }break;
    case ERenderCommandType::copy_buffer_to_texture:
    {
        auto& cmd = *static_cast<const RenderCommandCopyBufferToTexture*>(command);
        processCommandCopyBufferToTexture(encoder, cmd);
    }break;
    default:
    {
        sakura::error("NOT IMPLEMENTED!");
    }break;
	}
}

bool RenderDevice::execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer)
{
    if (queue == ERenderQueueType::Copy)
    {
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);// create encoder
        for(auto& cmd : command_buffer)
        {
            compileCommand(encoder, cmd);
        }
        WGPUFenceDescriptor fenceDesc = {};
        auto fence = wgpuQueueCreateFence(default_queue, &fenceDesc);
        WGPUCommandBuffer commands = wgpuCommandEncoderFinish(encoder, nullptr);// create commands
        wgpuQueueSubmit(default_queue, 1, &commands);
        wgpuQueueSignal(default_queue, fence, 1);
    	wgpuCommandBufferRelease(commands);
        wgpuCommandEncoderRelease(encoder);
		while(!wgpuFenceGetCompletedValue(fence))
		{
			
		}
        return true;
    }
    else
    {
        assert(0 && "Unimplemented!");
        return false;
    }
}

FenceHandle RenderDevice::execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer)
{
    assert(0 && "Unimplemented!");
    return GenerationalId::UNINITIALIZED;
}

void RenderDevice::wait_idle()
{
	
}


RenderFrameWebGPU::~RenderFrameWebGPU()
{
	
}

const void* RenderFrameWebGPU::raw() const
{
    return this;
}

void* RenderFrameWebGPU::raw()
{
    return this;
}

bool RenderFrameWebGPU::valid(const size_t currentFrame) const
{
    return currentFrame == this_frame_;
}

RenderDevice::RenderDevice(const RenderGraph& renderGraph, const DeviceConfiguration& config)
    :name(config.name), render_graph(renderGraph)
{
    initPlatformSpecific(config);
    // Get Default Queue
    default_queue = wgpuDeviceGetDefaultQueue(device);
    if (!default_queue)
    {
        sakura::error("[RenderDeviceWebGPU]: Get Default Queue Failed!");
    }
}

RenderDevice::~RenderDevice()
{
	for(auto& iter : shader_modules)
	{
        wgpuShaderModuleRelease(iter.second);
	}
    wgpuQueueRelease(default_queue);
}

EBackend RenderDevice::backend() const
{
    return EBackend::WebGPU;
}

bool RenderDevice::valid(const RenderResourceHandle handle) const
{
    return optional_unsafe(handle);
}

void RenderDevice::destroy(const RenderResourceHandle to_destroy)
{
	if(!optional_unsafe(to_destroy))
	{
        return;
	}
    else
    {
        delete get_unsafe(to_destroy);
    }
    created_resources[to_destroy.id().index()].first = nullptr;
    created_resources[to_destroy.id().index()].second++;
}


bool RenderDevice::execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl)
{
    // TODO: Move These to Constuction Phase.
    if (hdl.id().index() + 1 > pass_cache.size())
        pass_cache.resize(hdl.id().index() + 1); // Create New Cache
    // TODO: Generation Check & Validate
    auto&& cacheFrame = pass_cache[hdl.id().index()];
    {// create pass cache objects.
        cacheFrame.encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);// create encoder

        //WGPUFenceDescriptor desc = {};
        //cacheFrame.committed_fence =
        //    cacheFrame.committed_fence ? cacheFrame.committed_fence : wgpuQueueCreateFence(default_queue, &desc);
    }
    // Evaluate.
    for (auto& cmd : cmdBuffer)
    {
        processCommand(cacheFrame, cmd);
    }
	WGPUFenceDescriptor desc = {};
    if (cacheFrame.pass_encoder)
    {
        wgpuRenderPassEncoderRelease(cacheFrame.pass_encoder);
        WGPUCommandBuffer commands = wgpuCommandEncoderFinish(cacheFrame.encoder, nullptr);// create commands
        wgpuQueueSubmit(default_queue, 1, &commands);
        wgpuCommandBufferRelease(commands);
    }
    for (size_t i = 0u; i < cacheFrame.texture_views.size(); i++)
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
    if(auto swapChain = get<SwapChain>(handle);swapChain)
    {
        return swapChain->present();
    }
    else
    {
        assert(0 && "SwapChain not found!");
    }
    return false;
}


GpuBufferHandle RenderDevice::update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size)
{
    auto buf = get<GPUBuffer>(handle);
    wgpuQueueWriteBuffer(default_queue, buf->buffer, offset, data, size);
    return handle;
}


sakura::graphics::IGpuMemoryResource* RenderDevice::get_unsafe(const RenderResourceHandle handle) const
{
    if (handle == GenerationalId::UNINITIALIZED)
    {
        assert(0 && "Get on an invalide handle!");
        return nullptr;
    }
    else if (created_resources.size() < handle.id().index() + 1)
	{
		handle_error<RenderResourceHandle>::not_find(handle);
		return nullptr;
	}
	const auto& resource = created_resources[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		handle_error<RenderResourceHandle>::generation_dismatch(handle);
		return nullptr;
	}
}

sakura::graphics::IGpuMemoryResource* RenderDevice::optional_unsafe(const RenderResourceHandle handle) const
{
    if (handle == GenerationalId::UNINITIALIZED)
    {
        return nullptr;
    }
	else if (created_resources.size() < handle.id().index() + 1)
	{
		return nullptr;
	}
	const auto& resource = created_resources[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		return nullptr;
	}
}

sakura::graphics::IGpuObject* RenderDevice::get_unsafe(const RenderObjectHandle handle) const
{
    if (handle == GenerationalId::UNINITIALIZED)
    {
        assert(0 && "Get on an invalide handle!");
        return nullptr;
    }
    else if (created_objects.size() < handle.id().index() + 1)
	{
		handle_error<RenderObjectHandle>::not_find(handle);
		return nullptr;
	}
	const auto& resource = created_objects[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		handle_error<RenderObjectHandle>::generation_dismatch(handle);
		return nullptr;
	}
}

sakura::graphics::IGpuObject* RenderDevice::optional_unsafe(const RenderObjectHandle handle) const
{
    if (handle == GenerationalId::UNINITIALIZED)
    {
        return nullptr;
    }
    else if (created_objects.size() < handle.id().index() + 1)
	{
		return nullptr;
	}
	const auto& resource = created_objects[handle.id().index()];
	if (handle.id().generation() == resource.second)
		return resource.first;
	else
	{
		return nullptr;
	}
}

GpuShaderHandle RenderDevice::create_shader(const GpuShaderHandle handle, const ShaderDescriptor& config)
{
    return _create_resouce_impl<webgpu::GpuShader, GpuShaderHandle>(handle, *this, config);
}

GpuBufferHandle RenderDevice::create_buffer(const GpuBufferHandle handle, const BufferDescriptor& config)
{
	return _create_resouce_impl<webgpu::GPUBuffer, GpuBufferHandle>(handle, *this, config);
}

GpuTextureHandle RenderDevice::create_texture(const GpuTextureHandle handle, const TextureDescriptor& desc)
{
	return _create_resouce_impl<webgpu::GpuTexture, GpuTextureHandle>(handle, *this, desc);
}

GpuSamplerHandle RenderDevice::create_sampler(const GpuSamplerHandle handle, const SamplerDescriptor& desc)
{
    return _create_resouce_impl<webgpu::GpuSampler, GpuSamplerHandle>(handle, *this, desc);
}

ComputePipelineHandle RenderDevice::create_compute_pipeline(const ComputePipelineHandle handle,
	const ComputePipelineDescriptor& desc)
{
    assert(0 && "Unimplemented!");
    return GenerationalId::UNINITIALIZED;
}

void RenderDevice::write_texture(GpuTextureHandle texture, void const* data, size_t data_size,
                                 const TextureSlice& slice, const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index)
{
    WGPUTextureCopyView copyView = translate(slice, get<GpuTexture>(texture)->texture);
    WGPUTextureDataLayout lo = {};
    lo.offset = layout.offset;
    lo.rowsPerImage = layout.rows_per_image;
    lo.bytesPerRow = layout.bytes_per_raw;
    WGPUExtent3D size = { write_size.width, write_size.height, write_size.depth };
    wgpuQueueWriteTexture(default_queue, &copyView, data, data_size, &lo, &size);
}


SwapChainHandle RenderDevice::create_swap_chain(
    const SwapChainHandle handle, const SwapChainDescriptor& desc)
{
    return _create_object_impl<SwapChain, SwapChainHandle>(handle, *this, desc);
}

RenderPipelineHandle RenderDevice::create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDescriptor& desc)
{
    return _create_object_impl<RenderPipeline, RenderPipelineHandle>(handle, *this, desc);
}

FenceHandle RenderDevice::create_fence(const FenceHandle handle, const FenceDescriptor& desc)
{
    return _create_object_impl<Fence, FenceHandle>(handle, *this, desc);
}

void RenderDevice::terminate()
{

}
