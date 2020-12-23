#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "System/Log.h"

sakura::graphics::webgpu::RenderPipeline::RenderPipeline(RenderPipelineHandle handle,
	const webgpu::RenderDevice& dev, const RenderPipelineDesc& desc)
		:_handle(handle)
{
    WGPURenderPipelineDescriptor rpDesc = {};
	// PipelineLayout (Bindings).
    bindingGroups.resize(desc.binding_layout.tables.size());
	for(auto tbl = 0; tbl < desc.binding_layout.tables.size(); tbl++)
	{
        auto& table = desc.binding_layout.tables[tbl];
        sakura::vector<WGPUBindGroupLayoutEntry> binding_slots(table.slots.size());
        for (auto i = 0u; i < table.slots.size(); i++)
        {
            auto& slot_info = table.slots[i];
            WGPUBindGroupLayoutEntry bglEntry = {};
            binding_slots[i].binding = slot_info.binding;
            binding_slots[i].visibility = translate(slot_info.visibility);
            binding_slots[i].type = translate(slot_info.binding_type);
            binding_slots[i].hasDynamicOffset = true;
        }
        WGPUBindGroupLayoutDescriptor bglDesc = {};
        bglDesc.entryCount = static_cast<uint32>(table.slots.size());
        bglDesc.entries = binding_slots.data();
        // Bind Group Layout
        bindingGroups[tbl] = wgpuDeviceCreateBindGroupLayout(dev.device, &bglDesc);
	}
    // PipelineLayout (used by the render pipeline, released after its creation)
    WGPUPipelineLayoutDescriptor layoutDesc = {};
    layoutDesc.bindGroupLayoutCount = static_cast<uint32>(desc.binding_layout.tables.size());
    layoutDesc.bindGroupLayouts = bindingGroups.data();
    auto pipelineLayout = wgpuDeviceCreatePipelineLayout(dev.device, &layoutDesc);

    rpDesc.layout = pipelineLayout;
    
    // Shader Stages Set-Up.
    WGPUProgrammableStageDescriptor fragStage = {};
	if(desc.shader_layout.count != 2)
	{
        sakura::error(
            "[RenderGraphWebGPU]: WebGPU RenderPipeline should have 2 shaders, but {} were given.", desc.shader_layout.count);
	}
    for(auto i = 0u; i < desc.shader_layout.count; i++)
    {
        const auto& shaderHandle = desc.shader_layout.shaders[i];
        if(auto shaderFound = dev.optional<webgpu::GpuShader>(shaderHandle); shaderFound)
        {
            const auto freq = shaderFound->frequency();
            switch (freq)
            {
            case EShaderFrequency::VertexShader:
                rpDesc.vertexStage.entryPoint = shaderFound->entry_name().data();
                rpDesc.vertexStage.module = shaderFound->module_ref;
                break;
            case EShaderFrequency::PixelShader:
                fragStage.module = shaderFound->module_ref;
                fragStage.entryPoint = shaderFound->entry_name().data();
                rpDesc.fragmentStage = &fragStage;
                break;
            default:
                sakura::error("[RenderGraphWebGPU]: Shader {} with Unsupported Frequency: {} in RenderPipeline", shaderHandle, freq);
            }
        }
        else
        {
            sakura::error("[RenderGraphWebGPU]: Failed to find shader with handle: {}", handle);
            assert(0);
        }
    }
    rpDesc.sampleCount = desc.sample_count;

    // Vertex Attributes.
    sakura::vector<WGPUVertexBufferLayoutDescriptor>vertDescs(desc.vertex_layout.size());
    sakura::vector<sakura::vector<WGPUVertexAttributeDescriptor>> vertexAttributesList(desc.vertex_layout.size());
	for(size_t vbindex = 0u; vbindex < desc.vertex_layout.size(); vbindex++)
	{
        auto& vbslot = desc.vertex_layout[vbindex];
        auto& vertDesc = vertDescs[vbindex];
        // Describe VertexBuffer layouts
        vertexAttributesList[vbindex].resize(vbslot.elements.size());
        auto& vertexAttributes = vertexAttributesList[vbindex];
        for (auto i = 0u; i < vbslot.elements.size(); i++)
        {
            auto&& vertexElement = vbslot.elements[i];
            auto&& vertexAttribute = vertexAttributes[i];
            vertexAttribute.format = translate(vertexElement.format);
            vertexAttribute.offset = vertexElement.offset;
            vertexAttribute.shaderLocation = i;
        }
        vertDesc.arrayStride = vbslot.stride;
        vertDesc.attributeCount = static_cast<uint32>(vertexAttributes.size());
        vertDesc.attributes = vertexAttributes.data();
        vertDesc.stepMode = translate(vbslot.freq);
	}
    WGPUVertexStateDescriptor vertState = {};
#ifdef __EMSCRIPTEN__ // Emscripten hasn't yet caught up with the API changes
	vertState.indexFormat = WGPUIndexFormat_Uint16;
#endif
	// TODO: We only support one VertexBuffer now, Do something with this?
    vertState.vertexBufferCount = static_cast<uint32>(vertDescs.size());
    vertState.vertexBuffers = vertDescs.data(); 
    rpDesc.vertexState = &vertState;

	// Primitive Topology.
    rpDesc.primitiveTopology = translate(desc.primitive_topology);

    // Attachments
    sakura::vector<WGPUColorStateDescriptor> colorDescs(desc.attachment_layout.slots.size());
    for (size_t i = 0u; i < desc.attachment_layout.slots.size(); i++)
    {
        auto& colorDesc = colorDescs[i];
        auto& attachment_slot = desc.attachment_layout.slots[i];
        colorDesc.format = translate(attachment_slot.format);
        colorDesc.alphaBlend = translate(attachment_slot.alpha_blend);
        colorDesc.colorBlend = translate(attachment_slot.color_blend);
        colorDesc.writeMask = translate(attachment_slot.write_mask);
    }
    rpDesc.colorStateCount = static_cast<uint32>(desc.attachment_layout.slots.size());
    rpDesc.colorStates = colorDescs.data();

    // Sample Mask
    rpDesc.sampleMask = desc.sample_mask;

    // Create pipeline
    renderPipeline = wgpuDeviceCreateRenderPipeline(dev.device, &rpDesc);
	if(!renderPipeline)
	{
        sakura::error("[RenderGraphWebGPU]: Failed to create RenderPipeline {}", handle);
	}
    wgpuPipelineLayoutRelease(pipelineLayout);
}

sakura::graphics::webgpu::RenderPipeline::~RenderPipeline()
{
    // Clean.
    for (size_t i = 0u; i < bindingGroups.size(); i++)
    {
        wgpuBindGroupLayoutRelease(bindingGroups[i]);
    }
}

sakura::graphics::RenderObjectHandle sakura::graphics::webgpu::RenderPipeline::handle() const
{
	return _handle;
}
