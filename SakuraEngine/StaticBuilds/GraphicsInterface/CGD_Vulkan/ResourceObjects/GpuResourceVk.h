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
 * @Date: 2020-03-05 18:01:43
 * @LastEditTime: 2020-03-15 20:54:31
 */
#pragma once
#include "vulkan/vulkan.h"
#include "../../GraphicsCommon/ResourceObjects/Resource.h"
#include "vk_mem_alloc.h"

using namespace Sakura::Graphics;

namespace Sakura::Graphics::Vk
{
    struct GpuResourceVkImage final : public GpuResource
    {
        friend class CGD_Vk;
        friend class ResourceViewVkImage;
        virtual ~GpuResourceVkImage() override final;
        virtual void Map(void** data) override final;
        virtual void Unmap() override final;
    protected:
        GpuResourceVkImage(const CGD_Vk& _cgd,
            const VkImage& img, Extent2D _extent)
            :image(img), cgd(_cgd), GpuResource(_extent){}
        VkImage image;   
        const CGD_Vk& cgd;
    };
    
    struct GpuResourceVkBuffer final : public GpuResource
    {
        friend class CGD_Vk;
        friend class CommandContextVk;
        virtual ~GpuResourceVkBuffer() override final;
        virtual void Map(void** data) override final;
        virtual void Unmap() override final;
    protected:
        GpuResourceVkBuffer(const CGD_Vk& _cgd, 
            const VmaAllocation& _alloc,
            const VkBuffer& buf, uint32 _length)
            :buffer(buf), allocation(_alloc),
            cgd(_cgd), GpuResource({_length, 1}){}
        VkBuffer buffer;
        VmaAllocation allocation;
        const CGD_Vk& cgd;
    };
} // namespace Sakura::Graphics::Vk

