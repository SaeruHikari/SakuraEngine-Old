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
	:window_(desc.window), handle_(handle), device_(dev)
{
	extent_ = Window::extent(window_);

	surface_ = dev.create_and_validate_surface(window_);

	auto swapChainSupport = querySwapChainSupport(device_.master_device().device, surface_);

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
	createInfo.surface = surface_;

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
		createInfo.pQueueFamilyIndices = NULL;
		createInfo.queueFamilyIndexCount = 0;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device_.master_device().logical_device, &createInfo, nullptr, &swap_chain_) != VK_SUCCESS)
	{
		sakura::error("[VulkanSwapChain]: Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device_.master_device().logical_device, swap_chain_, &imageCount, nullptr);
	images_.resize(imageCount);
	vkGetSwapchainImagesKHR(device_.master_device().logical_device, swap_chain_, &imageCount, images_.data());

	format_ = surfaceFormat.format;
	extent_.width = extent.width;
	extent_.height = extent.height;

	image_views_.resize(images_.size());
	for (size_t i = 0; i < image_views_.size(); i++) 
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = images_[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format_;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device_.master_device().logical_device, &createInfo, nullptr, &image_views_[i]) != VK_SUCCESS)
		{
			sakura::error("failed to create image views!");
		}
	}

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	render_to_screen_semaphores_.resize(images_.size());
	vkCreateSemaphore(device_.master_device().logical_device, &semaphoreInfo, nullptr, &presentCompleteSemaphore);
	for (size_t i = 0; i < image_views_.size(); i++)
	{
		vkCreateSemaphore(device_.master_device().logical_device, &semaphoreInfo, nullptr, &render_to_screen_semaphores_[i]);
	}
}

sakura::graphics::vk::SwapChain::~SwapChain()
{
	vkDestroySemaphore(device_.master_device().logical_device, presentCompleteSemaphore, nullptr);
	for (size_t i = 0; i < image_views_.size(); i++) 
	{
		vkDestroySemaphore(device_.master_device().logical_device, render_to_screen_semaphores_[i], nullptr);

		vkDestroyImageView(device_.master_device().logical_device, image_views_[i], nullptr);
	}
	vkDestroySwapchainKHR(device_.master_device().logical_device, swap_chain_, nullptr);
	vkDestroySurfaceKHR(device_.instance_, surface_, nullptr);
}

sakura::uint8 sakura::graphics::vk::SwapChain::buffer_count() const
{
	return static_cast<sakura::uint8>(images_.size());
}

sakura::graphics::extent2d sakura::graphics::vk::SwapChain::extent() const
{
	return extent_;
}

sakura::Window sakura::graphics::vk::SwapChain::window() const
{
	return window_;
}

sakura::graphics::RenderGraphHandle sakura::graphics::vk::SwapChain::handle() const
{
	return handle_;
}

sakura::graphics::ETextureFormat sakura::graphics::vk::SwapChain::render_format() const
{
	return translate(format_);
}

bool sakura::graphics::vk::SwapChain::present()
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	VkSwapchainKHR swapChains[] = { swap_chain_ };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	{
		presentInfo.pWaitSemaphores = &render_to_screen_semaphores_[0];
		presentInfo.waitSemaphoreCount = 1;
	}
	vkQueuePresentKHR(device_.master_device().master_queue.queue, &presentInfo);

	// Get next image in the swap chain (back/front buffer)
	// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
	VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	return true;
}

VkImageView sakura::graphics::vk::SwapChain::back_buffer() const
{
	return image_views_[(imageIndex + 1) % buffer_count()];
}

// vulkan-specific.

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT)
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
		if (availablePresentMode == translate(presentMode))
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