#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

sakura::graphics::vk::RenderPipeline::RenderPipeline(RenderPipelineHandle handle,
	const vk::RenderDevice& render_device, VkDevice device, const RenderPipelineDesc& desc)
	:handle_(handle), owned_device_(device), render_device_(render_device)
{
	sakura::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	shaderStages.resize(desc.shader_layout.count);
	for (auto i = 0u; i < desc.shader_layout.count; i++)
	{
		const auto& shaderHandle = desc.shader_layout.shaders[i];
		if (auto tryFindShader = render_device.optional(shaderHandle); tryFindShader)
		{
			vk::GPUShader* shaderFound = static_cast<vk::GPUShader*>(tryFindShader);
			const auto freq = shaderFound->frequency();
			shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[i].module = shaderFound->shader_module_;
			shaderStages[i].pName = shaderFound->entry_name().data();
			switch (freq)
			{
			case EShaderFrequency::VertexShader:
				shaderStages[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case EShaderFrequency::PixelShader:
				shaderStages[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			default:
				sakura::error("[RenderGraphVulkan]: Shader {} with Unsupported Frequency: {} in RenderPipeline", shaderHandle, freq);
			}
		}
		else
		{
			sakura::error("[RenderGraphVulkan]: Failed to find shader with handle: {}", handle);
			assert(0);
		}
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	/*
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
	*/

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = translate(desc.polygon_mode);
	rasterizer.lineWidth = desc.lineWidth;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VkSampleCountFlagBits(desc.sample_count);
	multisampling.pSampleMask = &desc.sample_mask;

	// cn: Color Attachments 的转译.
	// en: Translate Color Attachments.
	// jp: カラーアタッチメントを翻訳する.
	sakura::vector<VkPipelineColorBlendAttachmentState> attachmentStates(desc.attachment_layout.slots.size());
	for (size_t i = 0u; i < desc.attachment_layout.slots.size(); i++)
	{
		auto& attachmentState = attachmentStates[i];
		auto& attachment_slot = desc.attachment_layout.slots[i];
		attachmentState.blendEnable = attachment_slot.blend_enable;
		attachmentState.colorWriteMask = translate(attachment_slot.write_mask);
		if (attachmentState.blendEnable)
		{
			attachmentState.alphaBlendOp = translate(attachment_slot.alpha_blend.operation);
			attachmentState.srcAlphaBlendFactor = translate(attachment_slot.alpha_blend.src_factor);
			attachmentState.dstAlphaBlendFactor = translate(attachment_slot.alpha_blend.dst_factor);

			attachmentState.colorBlendOp = translate(attachment_slot.color_blend.operation);
			attachmentState.dstColorBlendFactor = translate(attachment_slot.color_blend.dst_factor);
			attachmentState.srcColorBlendFactor = translate(attachment_slot.color_blend.src_factor);
		}
	}
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.attachmentCount = attachmentStates.size();
	colorBlending.pAttachments = attachmentStates.data();

	// cn: RenderGraph暂时不支持logicOp以及blendConstants, 因为没有动机.
	// en: RenderGraph does not support logicOp & blendConstants, no motivation to support currently.
	// jp: レンダリンググラフは、ロジックオペレーションとブレンド定数をサポートしていません。
	//     現在、サポートする動機はありません。
	{
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
	}


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipeline_layout_) != VK_SUCCESS) 
	{
		sakura::error("[RenderGraphVulkan]: failed to create pipeline layout!");
	}
}

sakura::graphics::vk::RenderPipeline::~RenderPipeline()
{
	vkDestroyPipelineLayout(owned_device_, pipeline_layout_, nullptr);
}

sakura::graphics::RenderGraphHandle sakura::graphics::vk::RenderPipeline::handle() const
{
	return handle_;
}

