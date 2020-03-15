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
 * @Date: 2020-03-06 00:57:40
 * @LastEditTime: 2020-03-15 13:59:36
 */
#include "ResourceViewVk.h"
#include "GpuResourceVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

void ResourceViewVkImage::Detach()
{
    CGD_Vk& vkdevice = (CGD_Vk&)(device);
    if(vkImgView != VK_NULL_HANDLE)
        vkDestroyImageView(vkdevice.GetCGDEntity().device, vkImgView, nullptr);
    vkImgView = VK_NULL_HANDLE;
}

void ResourceViewVkImage::Attach(
    const GpuResource& resource, const ResourceViewCreateInfo& info)
{
    Detach();
    CGD_Vk& vkdevice = (CGD_Vk&)device;
    auto vkres = (const GpuResourceVkImage&)resource;
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.viewType = Transfer(info.viewType);
    viewCreateInfo.format = Transfer(info.format);
    viewCreateInfo.image = vkres.image;
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewCreateInfo.subresourceRange.baseMipLevel 
        = info.view.texture2D.baseMipLevel;
    viewCreateInfo.subresourceRange.levelCount 
        = info.view.texture2D.mipLevels;
    viewCreateInfo.subresourceRange.baseArrayLayer 
        = info.view.texture2D.baseArrayLayer;
    viewCreateInfo.subresourceRange.layerCount 
        = info.view.texture2D.layerCount;
    if (vkCreateImageView(vkdevice.GetCGDEntity().device, &viewCreateInfo, 
        nullptr, &vkImgView) != VK_SUCCESS) 
    {
        Sakura::log::error("failed to create image views!");
        throw std::runtime_error("failed to create image views!");
    }
}

ResourceViewVkImage::~ResourceViewVkImage()
{
    Detach();
}

ResourceViewVkImage::ResourceViewVkImage(
    const CGD_Vk& _device)
    :ResourceView(_device, ResourceViewType::IMAGE_VIEW_TYPE_2D)
{
   
}

std::unique_ptr<ResourceView> CGD_Vk::ViewIntoImage(
    const GpuResource& resource, const ResourceViewCreateInfo& info) const
{
    auto res = 
        std::make_unique<ResourceViewVkImage>(*this);
    
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.viewType = Transfer(info.viewType);
    viewCreateInfo.format = Transfer(info.format);
    viewCreateInfo.image = ((const GpuResourceVkImage&)resource).image;
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(entityVk.device, &viewCreateInfo, 
        nullptr, &res->vkImgView) != VK_SUCCESS) 
    {
        Sakura::log::error("failed to create image views!");
        throw std::runtime_error("failed to create image views!");
    }
    return std::move(res);
}