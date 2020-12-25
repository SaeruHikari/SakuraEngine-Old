#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

sakura::graphics::vk::RenderPipeline::RenderPipeline(RenderPipelineHandle handle,
	const vk::RenderDevice& render_device, VkDevice device, const RenderPipelineDescriptor& desc)
	:handle_(handle), owned_device(device), render_device(render_device)
{
	// Binding Layout
	binding_layouts.resize(desc.binding_layout.tables.size());
	for (auto tbl = 0; tbl < desc.binding_layout.tables.size(); tbl++)
	{
		auto& set = desc.binding_layout.tables[tbl];
		sakura::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;
		uboLayoutBindings.resize(set.slots.size());
		for (auto i = 0u; i < set.slots.size(); i++)
		{
			auto& slot_info = set.slots[i];
			auto& uboLayoutBinding = uboLayoutBindings[i];
			uboLayoutBinding.binding = slot_info.binding;
			uboLayoutBinding.descriptorCount = slot_info.count;
			uboLayoutBinding.descriptorType = translate(slot_info.binding_type);
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBinding.stageFlags = translate(slot_info.visibility);
		}
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = uboLayoutBindings.size();
		layoutInfo.pBindings = uboLayoutBindings.data();
		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &binding_layouts[tbl]) != VK_SUCCESS)
		{
			sakura::error("failed to create descriptor set layout!");
		}
	}

	// Shader Stages Set-Up.
	sakura::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	shaderStages.resize(desc.shader_layout.count);
	for (auto i = 0u; i < desc.shader_layout.count; i++)
	{
		const auto& shaderHandle = desc.shader_layout.shaders[i];
		if (auto shaderFound = render_device.optional<vk::GpuShader>(shaderHandle); shaderFound)
		{
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

	// vertex input information
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	sakura::vector<VkVertexInputAttributeDescription> vertexAttributes;
	sakura::vector<VkVertexInputBindingDescription> vertexBindings;
	{
		vertexBindings.resize(desc.vertex_layout.size());
		for (size_t vbindex = 0u; vbindex < desc.vertex_layout.size(); vbindex++)
		{
			auto& vbbinding = desc.vertex_layout[vbindex];
			auto& vertexBinding = vertexBindings[vbindex];
			vertexBinding.binding = vbindex;
			vertexBinding.inputRate = translate(vbbinding.freq);
			vertexBinding.stride = vbbinding.stride;
			for (auto attr = 0u; attr < vbbinding.elements.size(); attr++)
			{
				VkVertexInputAttributeDescription attrib;
				attrib.format = translate(vbbinding.elements[attr].format);
				attrib.binding = vbindex;
				attrib.location = attr;
				attrib.offset = vbbinding.elements[attr].offset;
				vertexAttributes.emplace_back(attrib);
			}
		}
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = vertexBindings.size();
		vertexInputInfo.pVertexBindingDescriptions = vertexBindings.data();
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributes.size();
	}

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = translate(desc.primitive_topology);
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = translate(desc.polygon_mode);
	rasterizer.lineWidth = desc.lineWidth;
	rasterizer.cullMode = VK_CULL_MODE_NONE; 
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// cn: 多重采样设置.
	// en: multi-sampling setting up.
	// jp: マルチサンプリングの設定.
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VkSampleCountFlagBits(desc.sample_count);
	multisampling.pSampleMask = &desc.sample_mask;

	// cn: Color Attachments 的转译.
	// en: Translate Color Attachments.
	// jp: カラーアタッチメントを翻訳する.
	sakura::vector<VkPipelineColorBlendAttachmentState> attachmentStates;
	attachmentStates.resize(desc.attachment_layout.slots.size());
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


	// cn: RenderGraph暂时不支持logicOp以及blendConstants, 并且没有相应动机.
	// en: RenderGraph does not support logicOp & blendConstants, no motivation to support currently.
	// jp: レンダリンググラフは、ロジックオペレーションとブレンド定数をサポートしていません。
	//     現在、サポートする動機はありません。
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	{
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = attachmentStates.size();
		colorBlending.pAttachments = attachmentStates.data();
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = binding_layouts.size();
	pipelineLayoutInfo.pSetLayouts = binding_layouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipeline_layout) != VK_SUCCESS) 
	{
		sakura::error("[RenderGraphVulkan]: failed to create pipeline layout!");
	}

	
	// cn: subpass. 未实现, 暂时没有实现动机.
	// en: subpass, unimplemented, and no motivation for realizing temporarily.
	// jp: サブパス、未実現、一時的に実現する動機はありません。
	VkAttachmentReference colorAttachmentRef{};
	VkSubpassDescription subpass{};
	VkSubpassDependency dependency{};
	sakura::vector<VkAttachmentDescription> attachDescs(desc.attachment_layout.slots.size());
	{
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	}
	for(auto i = 0u; i < attachDescs.size(); i++)
	{
		auto& attachDesc = attachDescs[i];
		const auto& slot = desc.attachment_layout.slots[i];
		
		attachDesc.format = translate(slot.format);
		attachDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		attachDesc.loadOp = translate(slot.load_operation);
		attachDesc.storeOp = translate(slot.store_operation);
	
		attachDesc.stencilLoadOp = translate(slot.stencil_load_operation);
		attachDesc.stencilStoreOp = translate(slot.stencil_store_operation);

		// Barriers.
		attachDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32>(desc.attachment_layout.slots.size());
	renderPassInfo.pAttachments = attachDescs.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &pass) != VK_SUCCESS)
	{
		sakura::error("failed to create render pass!");
	}

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)1920.f;
	viewport.height = (float)1080.f;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { 1920, 1080 };

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipeline_layout;
	pipelineInfo.renderPass = pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(owned_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline)
		!= VK_SUCCESS)
	{
		sakura::error("failed to create graphics pipeline!");
	}
}

sakura::graphics::vk::RenderPipeline::~RenderPipeline()
{
	for(auto i = 0u; i < binding_layouts.size(); i++)
		vkDestroyDescriptorSetLayout(owned_device, binding_layouts[i], nullptr);
	
	vkDestroyPipelineLayout(owned_device, pipeline_layout, nullptr);
	vkDestroyPipeline(owned_device, pipeline, nullptr);
}

sakura::graphics::RenderObjectHandle sakura::graphics::vk::RenderPipeline::handle() const
{
	return handle_;
}
