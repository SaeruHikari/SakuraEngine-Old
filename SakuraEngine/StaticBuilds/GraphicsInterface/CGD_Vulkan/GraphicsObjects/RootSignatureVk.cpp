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
 * @Date: 2020-03-18 09:18:23
 * @LastEditTime: 2020-03-18 20:05:14
 */
#include "RootSignatureVk.h"
#include "../CGD_Vulkan.h"
#include "../ResourceObjects/GpuResourceVk.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

RootSignatureVk::RootSignatureVk(
    const CGD_Vk& _cgd, const RootSignatureCreateInfo& info)
        :cgd(_cgd)
{
    descriptorSetLayouts.resize(info.paramSlotNum);
    for(auto i = 0; i < info.paramSlotNum; i++)
    {
        auto slot = info.paramSlots[i];
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = i;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = Transfer(slot.type);
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = slot.stageFlags;

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(cgd.GetCGDEntity().device,
            &layoutInfo, nullptr, &descriptorSetLayouts[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    pools.resize(info.paramSlotNum);
    for(auto i = 0; i < info.paramSlotNum; i++)
    {
        auto slot = info.paramSlots[i];
        VkDescriptorPoolSize poolSize = {};
        poolSize.type = Transfer(slot.type);
        poolSize.descriptorCount = 1000;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 10;

        if (vkCreateDescriptorPool(cgd.GetCGDEntity().device,
                &poolInfo, nullptr, &pools[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
}

RootSignatureVk::~RootSignatureVk()
{
    for(auto i = 0; i < descriptorSetLayouts.size(); i++)
    {
        vkDestroyDescriptorSetLayout(cgd.GetCGDEntity().device,
            descriptorSetLayouts[i], nullptr);
    }
    for(auto i = 0; i < pools.size(); i++)
    {
        vkDestroyDescriptorPool(cgd.GetCGDEntity().device, 
            pools[i], nullptr);
    }
}

RootArgumentVk::RootArgumentVk(const CGD_Vk& _cgd,
    const VkDescriptorSetLayout& layout,
    const SignatureSlotType _type, VkDescriptorPool pool)
    :cgd(_cgd), type(_type)
{
    switch (_type)
    {
    case SignatureSlotType::UniformBufferSlot:
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;
        if (vkAllocateDescriptorSets(
            _cgd.GetCGDEntity().device, &allocInfo, &descriptorSet) != VK_SUCCESS) 
        {
            CGD_Vk::error("Create Root Argument: failed to allocate descriptor sets!");
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        break;    
    }
    default:
        CGD_Vk::error("Create Root Argument: Current Type not supported!");
        break;
    }
}

RootArgumentVk::~RootArgumentVk()
{
    
}

const SignatureSlotType RootArgumentVk::GetType(void) const
{
    return type;
}

const size_t RootSignatureVk::GetSlotNum(void) const
{
    return descriptorSetLayouts.size();
}

RootSignature* CGD_Vk::CreateRootSignature(
    const RootSignatureCreateInfo& sigInfo) const
{
    return new RootSignatureVk(*this, sigInfo);
}

RootArgument* RootSignatureVk::CreateArgument(
    uint32_t slot, const SignatureSlotType type) const
{
    VkDescriptorPool pool = pools[0];
    return new RootArgumentVk(cgd, descriptorSetLayouts[slot], type, pool);
}

void RootArgumentVk::UpdateArgument(const RootArgumentAttachment& attachment)
{
    VkWriteDescriptorSet descriptorWrite = {};
    switch (attachment.rootArgType)
    {
    case SignatureSlotType::UniformBufferSlot:
    {
        VkDescriptorBufferInfo bufferInfo = {};
#if defined(SAKURA_DEBUG_GAME) || defined(SAKURA_DEBUG_EDITOR)
        // Debug: Runtime check of resource type
        if(attachment.info.uniformBuffer.buffer->GetDefaultView() != ResourceViewType::Buffer)
            CGD_Vk::error("Can not update argument with incorrected resource type, \
                resource type cast not supported now!");
#endif
        bufferInfo.buffer = 
            ((const GpuResourceVkBuffer*)attachment.info.uniformBuffer.buffer)->buffer;
        bufferInfo.offset = attachment.info.uniformBuffer.offset;
        bufferInfo.range = attachment.info.uniformBuffer.range;
        descriptorWrite.pBufferInfo = &bufferInfo;
        break;
    }
    default:
        CGD_Vk::error("Root argument Slot type not supported!");
        break;
    }
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = attachment.dstBinding;
    descriptorWrite.dstArrayElement = attachment.dstArrayElement;
    descriptorWrite.descriptorType = Transfer(attachment.rootArgType);
    descriptorWrite.descriptorCount = attachment.descriptorCount;
    vkUpdateDescriptorSets(cgd.GetCGDEntity().device,
        1, &descriptorWrite, 0, nullptr);
}