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
 * @LastEditTime: 2020-03-18 16:59:40
 */
#include "RootSignatureVk.h"
#include "../CGD_Vulkan.h"

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
        uboLayoutBinding.descriptorType = ((VkDescriptorType)slot.type);
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
}

RootSignatureVk::~RootSignatureVk()
{
    for(auto i = 0; i < descriptorSetLayouts.size(); i++)
    {
        vkDestroyDescriptorSetLayout(cgd.GetCGDEntity().device,
            descriptorSetLayouts[i], nullptr);
    }
}

RootArgumentVk::RootArgumentVk(const CGD_Vk& _cgd,
    const VkDescriptorSetLayout& layout,
    const RootArgumentCreateInfo& info, VkDescriptorPool pool)
    :cgd(_cgd), type(info.rootArgType)
{
    switch (info.rootArgType)
    {
    case RootArgumentType::UniformBuffer:
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

const RootArgumentType RootArgumentVk::GetType(void) const
{
    return type;
}

RootSignature* CGD_Vk::CreateRootSignature(
    const RootSignatureCreateInfo& sigInfo) const
{
    return new RootSignatureVk(*this, sigInfo);
}

RootArgument* RootSignatureVk::CreateArgument(
    uint32_t slot, const RootArgumentCreateInfo& info) const
{
    VkDescriptorPool pool = VK_NULL_HANDLE;
    return new RootArgumentVk(cgd, descriptorSetLayouts[slot],
        info, pool);
}