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
 * @LastEditTime: 2020-03-19 20:41:56
 */

// Swap Chain Support Details
#pragma once
#include "Flags/GraphicsPipelineStatesVk.h"
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
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && 
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
        VkExtent2D actualExtent = {(uint32)width, (uint32)height};

        actualExtent.width =
            std::max(capabilities.minImageExtent.width, 
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = 
            std::max(capabilities.minImageExtent.height, 
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

#ifdef _CGDVk_IMPLEMENTATION_
Sakura::Graphics::SwapChain* CGDVk::CreateSwapChain(
    const int width, const int height, void* mainSurface)
{
    VkSurfaceKHR surface = *(VkSurfaceKHR*)mainSurface;
    auto physicalDevice = entityVk.physicalDevice;
    SwapChainSupportDetails swapChainSupport 
        = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = 
        chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = 
        chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(width, height,
        swapChainSupport.capabilities);

    uint32_t imageCount = 
        entityVk.bTripleBuffering ? 3 : 2;
    if (swapChainSupport.capabilities.maxImageCount > 0 
        && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;
    if(entityVk.bTripleBuffering && (imageCount == 2))
    {
        CGDVk::error("Can't enable triplebuffering, device not supported!");
        entityVk.bTripleBuffering = false;
    }

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
    VkSwapchainKHR swapChain;
    if (vkCreateSwapchainKHR(entityVk.device, &createInfo,
            nullptr, &swapChain) != VK_SUCCESS) 
    {
        Sakura::log::error("failed to create swap chain!");
        throw std::runtime_error("failed to create swap chain!");
    }
    // Get SwapChain Images
    vkGetSwapchainImagesKHR(entityVk.device, swapChain,
        &imageCount, nullptr);
    std::vector<VkImage> chainImages(imageCount);
    vkGetSwapchainImagesKHR(entityVk.device, swapChain,
        &imageCount, chainImages.data());
    
    // Create Image Views
    ResourceViewCreateInfo vinfo = {};
    vinfo.format = Transfer(surfaceFormat.format);
    vinfo.viewType = ImageView2D;
    auto res = new Sakura::Graphics::Vk::SwapChainVk(swapChain, *this, imageCount);
    for(auto i = 0u; i < imageCount; i++)
    {
        // Images
        GpuResourceVkImage* _img = 
            new GpuResourceVkImage(*this, chainImages[i], Transfer(extent));
        
        res->swapChainImages[i] = _img;
        // Views
        auto vkView= ViewIntoResource(*res->swapChainImages[i], vinfo);

        res->resourceViews[i].reset(vkView);
    }
    res->swapChainImageFormat = Transfer(surfaceFormat.format);
    res->swapChainExtent = extent;
    res->swapChainCount = imageCount;
    return res;
}
#endif