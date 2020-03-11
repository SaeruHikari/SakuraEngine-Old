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
 * @Date: 2020-03-06 00:49:22
 * @LastEditTime: 2020-03-11 14:34:25
 */
#include "SwapChainVk.h"
#include "../ResourceObjects/GpuResourceVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics::Vk;
using namespace Sakura;

SwapChainVk::SwapChainVk(const VkSwapchainKHR _chain, 
    const CGD_Vk& _device,const uint32 _chainCount)
    :swapChain(_chain), SwapChain(_device, _chainCount)
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAvailableSemaphores.resize(_chainCount);
    renderFinishedSemaphores.resize(_chainCount);
    frameSubmitFences.resize(_chainCount);
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for(auto i = 0; i < swapChainCount; i++)
        if(vkCreateFence(_device.GetCGDEntity().device, &fenceInfo,
            nullptr, &frameSubmitFences[i]) != VK_SUCCESS)
        {        
            CGD_Vk::error("failed to create fence for swapchain!");
            throw std::runtime_error("failed to create fence forswapchain!");
        }
    for(auto i = 0; i < swapChainCount; i++)
    {
        if (vkCreateSemaphore(_device.GetCGDEntity().device,
            &semaphoreInfo,
            nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
            ||
            vkCreateSemaphore(_device.GetCGDEntity().device,
            &semaphoreInfo,
            nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
        {
            Sakura::log::error("Vulkan: failed to create VkSemaphore for swapchain!");
            throw std::runtime_error("failed to create VkSemaphore for swapchain!");
        }
    }
    vkAcquireNextImageKHR(
        _device.GetCGDEntity().device,
        swapChain,
        UINT64_MAX,
        imageAvailableSemaphores[0],
        VK_NULL_HANDLE,
        &presentImageIndex
    );
}

SwapChainVk::~SwapChainVk()
{
    CGD_Vk& vkdevice = (CGD_Vk&)(device);
    vkDestroySwapchainKHR(vkdevice.GetCGDEntity().device, swapChain, nullptr);
    for(auto i = 0u; i < imageAvailableSemaphores.size(); i++)
    {
        vkWaitForFences(vkdevice.GetCGDEntity().device,
            1, &frameSubmitFences[i], VK_TRUE, UINT64_MAX);
        vkDestroyFence(vkdevice.GetCGDEntity().device, frameSubmitFences[i], nullptr);
        vkDestroySemaphore(vkdevice.GetCGDEntity().device, 
            imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(vkdevice.GetCGDEntity().device, 
            renderFinishedSemaphores[i], nullptr);
    }
}

Extent2D SwapChainVk::GetExtent() const
{
    Extent2D extent;
    extent.width = swapChainExtent.width;
    extent.height = swapChainExtent.height;
    return extent;
}