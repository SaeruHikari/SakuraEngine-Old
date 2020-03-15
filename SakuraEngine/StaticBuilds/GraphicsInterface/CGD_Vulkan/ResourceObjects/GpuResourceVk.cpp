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
 * @LastEditTime: 2020-03-15 14:24:35
 */
#include "GpuResourceVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics::Vk;
using namespace Sakura;

GpuResourceVkBuffer::~GpuResourceVkBuffer()
{
    vkFreeMemory(cgd.GetCGDEntity().device, memory, nullptr);
    vkDestroyBuffer(cgd.GetCGDEntity().device, buffer, nullptr);
}

GpuResourceVkImage::~GpuResourceVkImage()
{
    vkDestroyImage(cgd.GetCGDEntity().device, image, nullptr);
}

void GpuResourceVkBuffer::Map(void** data)
{
    vkMapMemory(cgd.GetCGDEntity().device, memory, 0, extent.width, 0, data);
}

void GpuResourceVkBuffer::Unmap()
{
    vkUnmapMemory(cgd.GetCGDEntity().device, memory);
}

void GpuResourceVkImage::Map(void** data)
{

}

void GpuResourceVkImage::Unmap()
{
    
}

std::unique_ptr<GpuResource> CGD_Vk::CreateResource(
    const ResourceCreateInfo& info) const
{
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    switch (info.type)
    {
    case ResourceType::Buffer:
        {
            VkBufferCreateInfo bufferInfo = Transfer(info);
            if (vkCreateBuffer(
                entityVk.device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) 
            {
                CGD_Vk::error("Failed to create vertex buffer!");
                throw std::runtime_error("failed to create vertex buffer!");
            }
            VkMemoryRequirements memRequirement;
            vkGetBufferMemoryRequirements(entityVk.device, vertexBuffer, &memRequirement);
            
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirement.size;
            static auto _type = findMemoryType(
                memRequirement.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            allocInfo.memoryTypeIndex = _type;
            if (vkAllocateMemory(
                entityVk.device, &allocInfo, nullptr, &memory) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to allocate vertex buffer memory!");
            }
            vkBindBufferMemory(entityVk.device, vertexBuffer, memory, 0);
            GpuResourceVkBuffer* vkBuf = new GpuResourceVkBuffer(
                *this, memory, vertexBuffer, info.size);
            return std::move(std::unique_ptr<GpuResource>(vkBuf));
        }
    default:
        CGD_Vk::error("CreateBuffer: Resource type not supported!");
        break;
    }
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