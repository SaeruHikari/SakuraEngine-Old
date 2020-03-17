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
 * @Date: 2020-03-06 00:37:46
 * @LastEditTime: 2020-03-17 23:37:00
 */
#pragma once
#include "SakuraEngine/Core/CoreMinimal/SInterface.h"
#include "../Flags/TypesVk.h"
#include "../../GraphicsCommon/ResourceObjects/ResourceViews.h"
#include "vulkan/vulkan.h"

namespace Sakura::Graphics::Vk
{
    class CGD_Vk;
}

namespace Sakura::Graphics::Vk
{
    using namespace Sakura::Graphics;

    struct ResourceViewVkImage final : public ResourceView
    {
        ResourceViewVkImage(const CGD_Vk&);
        virtual ~ResourceViewVkImage() override final;
        virtual void Detach() override final;
        virtual void Attach(const GpuResource&, const ResourceViewCreateInfo&) override final;
        VkImageView vkImgView = VK_NULL_HANDLE;
    };

}
