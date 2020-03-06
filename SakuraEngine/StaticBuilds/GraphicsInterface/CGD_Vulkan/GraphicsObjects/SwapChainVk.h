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
 * @Date: 2020-03-05 01:29:37
 * @LastEditTime: 2020-03-07 00:03:40
 */
#pragma once
#include "../../GraphicsCommon/GraphicsObjects/SwapChain.h"
#include "../../GraphicsCommon/ResourceObjects/Resource.h"
#include "../Vulkan_FormatTransfer.h"

namespace Sakura::Graphics::Vk
{
    struct SwapChainVk : public Sakura::Graphics::SwapChain
    {
        friend class CGD_Vk;
        SwapChainVk(const VkSwapchainKHR _chain, 
            const CGD& _device,const uint32 _chainCount)
            :swapChain(_chain), SwapChain(_device, _chainCount)
        {
            
        }
        virtual ~SwapChainVk() override final;
        virtual void GetExtent(uint32& width, uint32& height) override final;
        inline VkFormat GetVkPixelFormat() 
        {
            return Transfer(swapChainImageFormat);
        }
        VkSwapchainKHR swapChain;
        VkExtent2D swapChainExtent;
    };
}
