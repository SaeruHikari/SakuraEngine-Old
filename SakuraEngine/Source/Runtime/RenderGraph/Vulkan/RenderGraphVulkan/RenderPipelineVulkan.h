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
			VkDevice device, const RenderPipelineDescriptor& desc);
		~RenderPipeline();
		RenderObjectHandle handle() const override;

		const vk::RenderDevice& render_device;
		VkDevice owned_device = nullptr;
		VkPipelineLayout pipeline_layout = nullptr;
		VkPipeline pipeline = nullptr;
		VkRenderPass pass = nullptr;

		std::vector<VkDescriptorSetLayout> binding_layouts;
	protected:
		RenderPipelineHandle handle_;
	};
}
