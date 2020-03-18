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
 * @LastEditTime: 2020-03-18 10:00:28
 */
#include "GpuResourceVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "../CGD_Vulkan.h"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

using namespace Sakura::Graphics::Vk;
using namespace Sakura;

GpuResourceVkBuffer::~GpuResourceVkBuffer()
{
    vmaDestroyBuffer(cgd.GetCGDEntity().vmaAllocator, buffer, allocation);
    //vmaDestroyAllocator(cgd.GetCGDEntity().vmaAllocator);
}

GpuResourceVkImage::~GpuResourceVkImage()
{
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

}

void GpuResourceVkImage::Unmap()
{
    
}

void CGD_Vk::createAllocator()
{
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = entityVk.physicalDevice;
    allocatorInfo.device = entityVk.device;
    vmaCreateAllocator(&allocatorInfo, &entityVk.vmaAllocator);
}

GpuResource* CGD_Vk::CreateResource(const ResourceCreateInfo& info) const
{
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    switch (info.type)
    {
    case ResourceType::Buffer:
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = info.size;
            bufferInfo.usage = info.detail.buffer.usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            bufferInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
            auto indices = queueFamilyIndices.GetIndices();
            bufferInfo.pQueueFamilyIndices = indices.data();

            VmaAllocationCreateInfo vmaAllocInfo = {};
            VkMemoryPropertyFlags prop = 0;
            if(info.detail.buffer.cpuAccess 
                && CPUAccessFlags::ReadWrite != CPUAccessFlags::None)
            {
                prop |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
                prop |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                vmaAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            }
            else 
            {
                prop |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ;
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
    default:
        CGD_Vk::error("CreateBuffer: Resource type not supported!");
        break;
    }
    return nullptr;
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
