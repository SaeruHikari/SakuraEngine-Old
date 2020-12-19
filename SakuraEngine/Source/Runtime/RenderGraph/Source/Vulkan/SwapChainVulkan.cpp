#include "RenderGraphVulkan/RenderGraphVulkan.h"
#include "vulkan/vulkan_win32.h"
#include "System/Log.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

struct VkSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

VkSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(EPresentMode presentMode, const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(sakura::Window window, const VkSurfaceCapabilitiesKHR& capabilities);


sakura::graphics::vk::SwapChain::SwapChain(
	const SwapChainHandle handle, const vk::RenderDevice& dev, const SwapChainDesc& desc)
	:_window(desc.window), _handle(handle), device(dev)
{
	_extent = Window::extent(_window);

	surface = dev.create_and_validate_surface(_window);

	auto swapChainSupport = querySwapChainSupport(device.master_device().device, surface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(desc.present_mode, swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(desc.window, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{
		if (desc.buffer_count > swapChainSupport.capabilities.maxImageCount)
			imageCount = swapChainSupport.capabilities.maxImageCount;
		else
			swapChainSupport.capabilities.maxImageCount = desc.buffer_count;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// cn: 跨 queue 的 shared mode, 不支持, 且暂时没有支持的动机.
	// en: concurrent-shared mode, not supported and no motivation to support currently.
	// jp: 並行共有モード、サポートされておらず、現在サポートする動機はありません.
	const bool presentWithGfxQueue = true;
	if (!presentWithGfxQueue) 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		//createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device.master_device().logical_device, &createInfo, nullptr, &swap_chain) != VK_SUCCESS) 
	{
		sakura::error("[VulkanSwapChain]: Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device.master_device().logical_device, swap_chain, &imageCount, nullptr);
	swapchian_images.resize(imageCount);
	vkGetSwapchainImagesKHR(device.master_device().logical_device, swap_chain, &imageCount, swapchian_images.data());

	format_ = surfaceFormat.format;
	_extent.width = extent.width;
	_extent.height = extent.height;
}

sakura::graphics::vk::SwapChain::~SwapChain()
{
	vkDestroySwapchainKHR(device.master_device().logical_device, swap_chain, nullptr);
	vkDestroySurfaceKHR(device.instance, surface, nullptr);
}

sakura::uint8 sakura::graphics::vk::SwapChain::buffer_count() const
{
	return static_cast<sakura::uint8>(swapchian_images.size());
}

sakura::graphics::extent2d sakura::graphics::vk::SwapChain::extent() const
{
	return _extent;
}

sakura::Window sakura::graphics::vk::SwapChain::window() const
{
	return _window;
}

sakura::graphics::RenderGraphHandle sakura::graphics::vk::SwapChain::handle() const
{
	return _handle;
}

sakura::graphics::ETextureFormat sakura::graphics::vk::SwapChain::render_format() const
{
	return transfer(format_);
}

bool sakura::graphics::vk::SwapChain::present()
{
	return false;
}


// vulkan-specific.

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(EPresentMode presentMode, const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == transfer(presentMode))
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(sakura::Window window, const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else {
		const auto ext = window.extent();

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(ext.width),
			static_cast<uint32_t>(ext.height)
		};
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}

VkSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	VkSwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) 
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}