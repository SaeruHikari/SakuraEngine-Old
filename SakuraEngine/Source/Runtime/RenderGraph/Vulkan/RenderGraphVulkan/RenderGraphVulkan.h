#pragma once
#include "BufferVulkan.h"
#include "TextureVulkan.h"
#include "DeviceVulkan.h"
#include "FenceVulkan.h"
#include "ShaderVulkan.h"
#include "SwapChainVulkan.h"
#include "RenderPipelineVulkan.h"

namespace sakura::graphics::vk
{
	RenderGraphVulkanAPI VkPresentModeKHR translate(EPresentMode present_mode);

	RenderGraphVulkanAPI EPresentMode translate(VkPresentModeKHR present_mode);

	RenderGraphVulkanAPI VkFormat translate(ETextureFormat format);

	RenderGraphVulkanAPI ETextureFormat translate(VkFormat format);

	RenderGraphVulkanAPI VkShaderStageFlagBits translate(EShaderFrequency frequency);

	RenderGraphVulkanAPI VkColorComponentFlags translate(ColorMask mask);

	RenderGraphVulkanAPI VkBlendOp translate(EBlendOp blendOp);

	RenderGraphVulkanAPI VkBlendFactor translate(EBlendFactor blendFactor);

	RenderGraphVulkanAPI VkPolygonMode translate(EPolygonMode polygonMode);

	RenderGraphVulkanAPI VkAttachmentLoadOp translate(ELoadOp op);

	RenderGraphVulkanAPI VkAttachmentStoreOp translate(EStoreOp op);

	RenderGraphVulkanAPI VkBufferUsageFlags translate(EBufferUsage usage);

	RenderGraphVulkanAPI VkPrimitiveTopology translate(EPrimitiveTopology topo);

	RenderGraphVulkanAPI VkVertexInputRate translate(VertexLayout::Frequency freq);

	RenderGraphVulkanAPI VkFormat translate(EVertexFormat format);

	RenderGraphVulkanAPI VkDescriptorType translate(BindingLayout::EType type);
}