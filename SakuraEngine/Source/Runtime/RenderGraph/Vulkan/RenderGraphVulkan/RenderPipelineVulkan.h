#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI RenderPipeline : public IRenderPipeline
	{
	public:
		RenderPipeline(RenderPipelineHandle handle, const vk::RenderDevice& render_device, 
			VkDevice device, const RenderPipelineDesc& desc);
		~RenderPipeline();
		RenderGraphHandle handle() const override;

		void start(VkRenderPass render_pass);

		const vk::RenderDevice& render_device_;
		VkDevice owned_device_ = VK_NULL_HANDLE;
		VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
		VkPipeline pipeline_ = VK_NULL_HANDLE;

		std::vector<VkDescriptorSetLayout> binding_layouts_;

		sakura::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		sakura::vector<VkPipelineColorBlendAttachmentState> attachmentStates;
		sakura::vector<VkVertexInputAttributeDescription> vertexAttributes;
		sakura::vector<VkVertexInputBindingDescription> vertexBingings;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		VkPipelineMultisampleStateCreateInfo multisampling{};
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		VkDescriptorSetLayoutCreateInfo layoutInfo{};

	protected:
		RenderPipelineHandle handle_;
	};
}
