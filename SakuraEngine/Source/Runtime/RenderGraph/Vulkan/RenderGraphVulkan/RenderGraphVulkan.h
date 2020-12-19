#pragma once
#include "BufferVulkan.h"
#include "TextureVulkan.h"
#include "DeviceVulkan.h"
#include "FenceVulkan.h"
#include "SwapChainVulkan.h"

namespace sakura::graphics::vk
{
	RenderGraphVulkanAPI VkPresentModeKHR transfer(EPresentMode present_mode);

	RenderGraphVulkanAPI EPresentMode transfer(VkPresentModeKHR present_mode);

	RenderGraphVulkanAPI VkFormat transfer(ETextureFormat format);

	RenderGraphVulkanAPI ETextureFormat transfer(VkFormat format);

}