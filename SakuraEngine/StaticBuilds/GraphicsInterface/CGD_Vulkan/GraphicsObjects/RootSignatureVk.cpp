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
 * @LastEditTime: 2020-03-24 11:53:08
 */
#include "RootSignatureVk.h"
#include "../CGD_Vulkan.h"
#include "../ResourceObjects/GpuResourceVk.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;
using namespace Sakura;

RootSignatureVk::RootSignatureVk(
    const CGD_Vk& _cgd, const RootSignatureCreateInfo& info)
        :cgd(_cgd)
{
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings(info.paramSlotNum);
    std::set<SignatureSlotType> types;
    for(auto i = 0; i < info.paramSlotNum; i++)
    {
        auto slot = info.paramSlots[i];
        layoutBindings[i].binding = i;
        layoutBindings[i].descriptorCount = 1;
        layoutBindings[i].descriptorType = Transfer(slot.type);
        layoutBindings[i].pImmutableSamplers = nullptr;
        layoutBindings[i].stageFlags = slot.stageFlags;
        types.insert(slot.type);
    }
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = layoutBindings.size();
    layoutInfo.pBindings = layoutBindings.data();
    if (vkCreateDescriptorSetLayout(cgd.GetCGDEntity().device,
        &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    std::vector<VkDescriptorPoolSize> poolSizes(types.size());
    int i = 0;
    for(auto iter = types.begin(); iter != types.end(); iter++)
    {
        poolSizes[i].type = Transfer(*iter);
        poolSizes[i].descriptorCount = 1000;
        i++;
    }
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(cgd.GetCGDEntity().device,
            &poolInfo, nullptr, &pool) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
    
}

RootSignatureVk::~RootSignatureVk()
{
    vkDestroyDescriptorSetLayout(cgd.GetCGDEntity().device,
        descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(cgd.GetCGDEntity().device, 
            pool, nullptr);
}

RootArgumentVk::RootArgumentVk(const CGD_Vk& _cgd,
    const VkDescriptorSetLayout& layout, VkDescriptorPool pool)
    :cgd(_cgd)
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
}

RootArgumentVk::~RootArgumentVk()
{
    
}

const SignatureSlotType RootArgumentVk::GetType(void) const
{
    return type;
}

const size_t RootArgumentVk::GetSlotNum(void) const
{
    return 0;
}

RootSignature* CGD_Vk::CreateRootSignature(
    const RootSignatureCreateInfo& sigInfo) const
{
    return new RootSignatureVk(*this, sigInfo);
}

RootArgument* RootSignatureVk::CreateArgument() const
{
    return new RootArgumentVk(cgd, descriptorSetLayout, pool);
}

void RootArgumentVk::UpdateArgument(
    const RootArgumentAttachment* attachments, std::uint32_t attachmentCount)
{
    std::vector<VkWriteDescriptorSet> descriptorWrites(attachmentCount);
    for(auto i = 0u; i < attachmentCount; i++)
    {
        switch (attachments[i].rootArgType)
        {
        case SignatureSlotType::StorageBufferSlot:
        case SignatureSlotType::UniformBufferSlot:
        {
            VkDescriptorBufferInfo buffer = {};
            buffer.buffer = 
                ((const GpuResourceVkBuffer*)std::get<UniformBufferAttachment>(attachments[i].info).buffer)->buffer;
            buffer.offset =
                std::get<UniformBufferAttachment>(attachments[i].info).offset;
            buffer.range = 
                std::get<UniformBufferAttachment>(attachments[i].info).range;
            descriptorWrites[i].pBufferInfo = &buffer;
            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = descriptorSet;
            descriptorWrites[i].dstBinding = attachments[i].dstBinding;
            descriptorWrites[i].dstArrayElement = attachments[i].dstArrayElement;
            descriptorWrites[i].descriptorType = Transfer(attachments[i].rootArgType);
            descriptorWrites[i].descriptorCount = attachments[i].descriptorCount;
            vkUpdateDescriptorSets(cgd.GetCGDEntity().device,
                1, &descriptorWrites[i], 0, nullptr);
            continue;
        }
        case SignatureSlotType::SamplerSlot:
        case SignatureSlotType::SampledImageSlot:
        case SignatureSlotType::StorageImageSlot:
        case SignatureSlotType::CombinedTextureSamplerSlot:
        {
            VkDescriptorImageInfo image = {};
            auto tex2D = std::get<TexSamplerAttachment>(attachments[i].info);
            if(attachments[i].rootArgType != SamplerSlot)
            {
                image.imageLayout 
                    = Transfer(tex2D.imageLayout);
                image.imageView  
                    = ((const ResourceViewVkImage*)tex2D.imageView)->vkImgView;
            }
            if(attachments[i].rootArgType != SampledImageSlot)
            {
                image.sampler 
                    = ((const SamplerVk*)tex2D.sampler)->sampler;
            }
            descriptorWrites[i].pImageInfo = &image;
            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = descriptorSet;
            descriptorWrites[i].dstBinding = attachments[i].dstBinding;
            descriptorWrites[i].dstArrayElement = attachments[i].dstArrayElement;
            descriptorWrites[i].descriptorType = Transfer(attachments[i].rootArgType);
            descriptorWrites[i].descriptorCount = attachments[i].descriptorCount;
            vkUpdateDescriptorSets(cgd.GetCGDEntity().device,
                1, &descriptorWrites[i], 0, nullptr);
            continue;
        }
        default:
            CGD_Vk::error("Root argument Slot type not supported!");
            return;
        }
    }
}