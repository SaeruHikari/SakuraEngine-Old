/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-05 00:59:21
 * @LastEditTime: 2020-03-06 01:20:46
 */

// Swap Chain Support Details
#pragma once
#include <algorithm>
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

inline SwapChainSupportDetails querySwapChainSupport(
    VkPhysicalDevice phy_device, const VkSurfaceKHR surface) 
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_device,
        surface, &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, 
        &formatCount, nullptr);  
    if (formatCount != 0) 
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, 
            &formatCount, details.formats.data());
    }
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface,
        &presentModeCount, nullptr);
    if (presentModeCount != 0) 
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface,
            &presentModeCount, details.presentModes.data());
    }
    return details;
}

inline VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }
    return availableFormats[0];
}

inline VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) 
{
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
            return availablePresentMode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

inline VkExtent2D chooseSwapExtent(const int width, const int height, 
    const VkSurfaceCapabilitiesKHR& capabilities) 
{
    if (capabilities.currentExtent.width != UINT32_MAX) 
        return capabilities.currentExtent;
    else 
    {
        VkExtent2D actualExtent = {width, height};

        actualExtent.width =
            std::max(capabilities.minImageExtent.width, 
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = 
            std::max(capabilities.minImageExtent.height, 
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

#ifdef _CGD_VK_IMPLEMENTATION_
std::unique_ptr<Sakura::Graphics::SwapChain>
    CGD_Vk::CreateSwapChain(const int width, const int height, 
        CGDEntity& device, void* mainSurface)
{
    auto res = std::make_unique<Sakura::Graphics::Vk::SwapChainVk>();
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    VkSurfaceKHR surface = *(VkSurfaceKHR*)mainSurface;
    auto physicalDevice = vkdevice.physicalDevice;
    SwapChainSupportDetails swapChainSupport 
        = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = 
        chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = 
        chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(width, height,
        swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 
        && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = 
        {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    if (indices.graphicsFamily != indices.presentFamily) 
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } 
    else
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    // Create SwapChain
    if (vkCreateSwapchainKHR(vkdevice.device, &createInfo,
            nullptr, &res->swapChain) != VK_SUCCESS) 
    {
        Sakura::log::error("failed to create swap chain!");
        throw std::runtime_error("failed to create swap chain!");
    }

    // Get SwapChain Images
    vkGetSwapchainImagesKHR(vkdevice.device, res->swapChain,
        &imageCount, nullptr);
    std::vector<VkImage> chainImages;
    chainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkdevice.device, res->swapChain,
        &imageCount, chainImages.data());
    
    // Create Image Views
    std::vector<VkImageView> swapChainImageViews(imageCount);
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewCreateInfo.format = surfaceFormat.format;
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;
    for (size_t i = 0; i < imageCount; i++) 
    {
        viewCreateInfo.image = chainImages[i];
        if (vkCreateImageView(vkdevice.device, &viewCreateInfo, 
            nullptr, &swapChainImageViews[i]) != VK_SUCCESS) 
        {
            Sakura::log::error("failed to create image views!");
            throw std::runtime_error("failed to create image views!");
        }
    }

    // Export
    res->swapChainImages.resize(imageCount);
    res->resourceViews.resize(imageCount);
    for(auto i = 0u; i < imageCount; i++)
    {
        std::unique_ptr<GpuResourceVkImage> vkImg 
            = std::make_unique<GpuResourceVkImage>();
        vkImg->image = chainImages[i];
        res->swapChainImages[i] = std::move(vkImg);

        std::unique_ptr<ResourceViewVkImage> vkView
            = std::make_unique<ResourceViewVkImage>(device);
        vkView->vkImgView = swapChainImageViews[i];
        res->resourceViews[i] = std::move(vkView);
    }
    res->SetPixelFormat(surfaceFormat.format);
    res->swapChainExtent = extent;
    res->swapChainCount = imageCount;
    res->device = &vkdevice.device;
    return std::move(res);
}
#endif