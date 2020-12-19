#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

RenderGraphVulkanAPI VkPresentModeKHR sakura::graphics::vk::transfer(EPresentMode present_mode)
{
	switch (present_mode)
	{
	case sakura::graphics::EPresentMode::Immediate:
		return VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
	case sakura::graphics::EPresentMode::FIFO:
		return VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR;
	case sakura::graphics::EPresentMode::Mailbox:
		return VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
	case sakura::graphics::EPresentMode::Count:
	default:
		return VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR;
	}
}

RenderGraphVulkanAPI EPresentMode sakura::graphics::vk::transfer(VkPresentModeKHR present_mode)
{
	switch (present_mode)
	{
	case VK_PRESENT_MODE_IMMEDIATE_KHR:
		return sakura::graphics::EPresentMode::Immediate;
	case VK_PRESENT_MODE_MAILBOX_KHR:
		return sakura::graphics::EPresentMode::Mailbox;
	case VK_PRESENT_MODE_FIFO_KHR:
	case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
		return sakura::graphics::EPresentMode::FIFO;
	case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
	case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
	case VK_PRESENT_MODE_MAX_ENUM_KHR:
	default:
		return sakura::graphics::EPresentMode::Count;
	}
}

RenderGraphVulkanAPI VkFormat sakura::graphics::vk::transfer(ETextureFormat format)
{
	return VkFormat::VK_FORMAT_UNDEFINED;
}

RenderGraphVulkanAPI ETextureFormat sakura::graphics::vk::transfer(VkFormat format)
{
	switch (format)
	{

	default:
		return ETextureFormat::Count;
	}
}