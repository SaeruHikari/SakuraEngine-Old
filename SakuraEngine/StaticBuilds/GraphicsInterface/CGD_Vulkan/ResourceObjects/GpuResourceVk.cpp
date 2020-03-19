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
 * @Date: 2020-03-05 22:41:33
 * @LastEditTime: 2020-03-19 21:48:34
 */
#include "GpuResourceVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "../CGD_Vulkan.h"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

using namespace Sakura::Graphics::Vk;
using namespace Sakura;

SamplerVk::SamplerVk(const CGD_Vk& _cgd)
    :cgd(_cgd)
{
    
}

SamplerVk::~SamplerVk()
{
    vkDestroySampler(cgd.GetCGDEntity().device, sampler, nullptr);
}

GpuResourceVkBuffer::~GpuResourceVkBuffer()
{
    vmaDestroyBuffer(cgd.GetCGDEntity().vmaAllocator, buffer, allocation);
    //vmaDestroyAllocator(cgd.GetCGDEntity().vmaAllocator);
}

GpuResourceVkImage::~GpuResourceVkImage()
{
    if(allocation != VK_NULL_HANDLE)
        vmaDestroyImage(cgd.GetCGDEntity().vmaAllocator, image, allocation);
    else
        vkDestroyImage(cgd.GetCGDEntity().device, image, nullptr);
}

void GpuResourceVkBuffer::Map(void** data)
{
    vmaMapMemory(cgd.GetCGDEntity().vmaAllocator, allocation, data);
}

void GpuResourceVkBuffer::Unmap()
{
    vmaUnmapMemory(cgd.GetCGDEntity().vmaAllocator, allocation);
}

void GpuResourceVkImage::Map(void** data)
{
    vmaMapMemory(cgd.GetCGDEntity().vmaAllocator, allocation, data);
}

void GpuResourceVkImage::Unmap()
{
    vmaUnmapMemory(cgd.GetCGDEntity().vmaAllocator, allocation);
}   

void CGD_Vk::createAllocator()
{
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = entityVk.physicalDevice;
    allocatorInfo.device = entityVk.device;
    vmaCreateAllocator(&allocatorInfo, &entityVk.vmaAllocator);
}

GpuTexture* CGD_Vk::CreateResource(const TextureCreateInfo& info) const
{
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = info.width;
    imageInfo.extent.height = info.height;
    imageInfo.extent.depth = info.depth;
    imageInfo.mipLevels = info.mipLevels;
    imageInfo.arrayLayers = info.arrayLayers;
    imageInfo.format = Transfer(info.format);
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = info.usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    imageInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
    auto indices = queueFamilyIndices.GetIndices();
    imageInfo.pQueueFamilyIndices = indices.data();

    VmaAllocationCreateInfo vmaAllocInfo = {};
    VkMemoryPropertyFlags prop = 0;
    prop |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    vmaAllocInfo.pool = VK_NULL_HANDLE;
    vmaAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    vmaAllocInfo.requiredFlags = prop;
    VkImage image;
    VmaAllocation allocation;
    if(vmaCreateImage(entityVk.vmaAllocator, &imageInfo, &vmaAllocInfo,
        &image, &allocation, nullptr) != VK_SUCCESS)
    {
        CGD_Vk::error("Failed to create Image!");
    }
    GpuResourceVkImage* vkImg = new GpuResourceVkImage(
        *this, image, {imageInfo.extent.width, imageInfo.extent.height});
    vkImg->allocation = allocation;
    return vkImg;
}

GpuBuffer* CGD_Vk::CreateResource(const BufferCreateInfo& info) const
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = info.size;
    bufferInfo.usage = info.usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    bufferInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
    auto indices = queueFamilyIndices.GetIndices();
    bufferInfo.pQueueFamilyIndices = indices.data();

    VmaAllocationCreateInfo vmaAllocInfo = {};
    VkMemoryPropertyFlags prop = 0;
    if(info.cpuAccess 
        && CPUAccessFlags::ReadWrite != CPUAccessFlags::None)
    {
        prop |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        prop |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        vmaAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    }
    else 
    {
        prop |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        vmaAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    }
    vmaAllocInfo.requiredFlags = prop;
    vmaAllocInfo.pool = VK_NULL_HANDLE;
    VkBuffer buffer;
    VmaAllocation allocation;
    vmaCreateBuffer(entityVk.vmaAllocator, &bufferInfo, &vmaAllocInfo,
        &buffer, &allocation, nullptr);
        
    GpuResourceVkBuffer* vkBuf = new GpuResourceVkBuffer(
        *this, allocation, buffer, info.size);
    return vkBuf;
}

Sampler* CGD_Vk::CreateSampler(const SamplerCreateInfo& info) const
{
    SamplerVk* result = new SamplerVk(*this);
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = (VkFilter)info.magFilter;
    samplerInfo.minFilter = (VkFilter)info.minFilter;
    samplerInfo.addressModeU = (VkSamplerAddressMode)info.addressModeU;
    samplerInfo.addressModeV = (VkSamplerAddressMode)info.addressModeV;
    samplerInfo.addressModeW = (VkSamplerAddressMode)info.addressModeW;
    samplerInfo.anisotropyEnable = info.anisotropyEnable ? VK_TRUE : VK_FALSE;
    samplerInfo.maxAnisotropy = info.maxAnisotropy;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.compareOp = (VkCompareOp)info.compareOp;
    samplerInfo.mipmapMode = (VkSamplerMipmapMode)info.mipmapMode;

    samplerInfo.unnormalizedCoordinates = 
        info.unnormalizedCoordinates ? VK_TRUE : VK_FALSE;
    samplerInfo.compareEnable = info.compareEnable ? VK_TRUE : VK_FALSE;
    if (vkCreateSampler(entityVk.device, 
        &samplerInfo, nullptr, &result->sampler) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
    return result;
}

uint32_t CGD_Vk::findMemoryType(
    uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(entityVk.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    CGD_Vk::error("failed to find suitable memory type!");
    throw std::runtime_error("failed to find suitable memory type!");
}
