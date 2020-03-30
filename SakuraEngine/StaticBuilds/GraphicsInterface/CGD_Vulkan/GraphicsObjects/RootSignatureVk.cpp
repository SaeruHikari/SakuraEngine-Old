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
 * @LastEditTime: 2020-03-29 19:49:01
 */
#include "RootSignatureVk.h"
#include "../CGD_Vulkan.h"
#include "../ResourceObjects/GpuResourceVk.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;
using namespace Sakura;

RootSignatureVk::RootSignatureVk(
    const CGDVk& _cgd, const RootSignatureCreateInfo& info)
        :cgd(_cgd)
{
	std::set<SignatureSlotType> types;
    if(info.staticSamplers.size() > 0)
        types.insert(SamplerSlot);
    for (auto setN = 0; setN < RootParameterSetCount; setN++)
    {
		auto paramSlotNum = info.paramSlots[setN].size();
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings(paramSlotNum);
		for (auto i = 0; i < paramSlotNum; i++)
		{
			auto slot = info.paramSlots[setN][i];
			layoutBindings[i].binding = i;
			layoutBindings[i].descriptorCount = 1;
			layoutBindings[i].descriptorType = Transfer(slot.type);
			layoutBindings[i].pImmutableSamplers = nullptr;
			layoutBindings[i].stageFlags = slot.stageFlags;
			types.insert(slot.type);
		}
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = (uint32_t)layoutBindings.size();
		layoutInfo.pBindings = layoutBindings.data();
		if (vkCreateDescriptorSetLayout(cgd.GetCGDEntity().device,
			&layoutInfo, nullptr, &descriptorSetLayout[setN]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
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
    poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(cgd.GetCGDEntity().device,
            &poolInfo, nullptr, &pool) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    staticSamplers.resize(info.staticSamplers.size());
    for (auto i = 0; i < staticSamplers.size(); i++)
    {
        auto samp = info.staticSamplers[i];
		VkSamplerCreateInfo samplerInfo = {};   
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = (VkFilter)samp.magFilter;
		samplerInfo.minFilter = (VkFilter)samp.minFilter;
		samplerInfo.minLod = samp.minLod;
		samplerInfo.maxLod = samp.maxLod;
		samplerInfo.addressModeU = (VkSamplerAddressMode)samp.addressModeU;
		samplerInfo.addressModeV = (VkSamplerAddressMode)samp.addressModeV;
		samplerInfo.addressModeW = (VkSamplerAddressMode)samp.addressModeW;
		samplerInfo.anisotropyEnable = samp.anisotropyEnable ? VK_TRUE : VK_FALSE;
		samplerInfo.maxAnisotropy = samp.maxAnisotropy;
		samplerInfo.mipLodBias = samp.mipLodBias;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.compareOp = (VkCompareOp)samp.compareOp;
		samplerInfo.mipmapMode = (VkSamplerMipmapMode)samp.mipmapMode;

		samplerInfo.unnormalizedCoordinates =
            samp.unnormalizedCoordinates ? VK_TRUE : VK_FALSE;
		samplerInfo.compareEnable = samp.compareEnable ? VK_TRUE : VK_FALSE;
		if (vkCreateSampler(cgd.GetCGDEntity().device,
			&samplerInfo, nullptr, &staticSamplers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
    }
    std::vector<VkDescriptorSetLayoutBinding> 
            layoutBinding(staticSamplers.size());
	for (auto i = 0; i < staticSamplers.size(); i++)
	{
		layoutBinding[i].binding = i;
		layoutBinding[i].descriptorCount = 1;
		layoutBinding[i].descriptorType =
            VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
		layoutBinding[i].pImmutableSamplers = &staticSamplers[i];
		layoutBinding[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = (uint32_t)staticSamplers.size();
	layoutInfo.pBindings = layoutBinding.data();
	if (vkCreateDescriptorSetLayout(cgd.GetCGDEntity().device, &layoutInfo,
		nullptr, &descriptorSetLayout[RootParameterSetCount]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
    
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout[RootParameterSetCount];
	if (vkAllocateDescriptorSets(
		_cgd.GetCGDEntity().device, &allocInfo, &staticSamplerSet) != VK_SUCCESS)
	{
		CGDVk::error("Create Root Argument: failed to allocate descriptor sets!");
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

RootSignatureVk::~RootSignatureVk()
{
    for (auto i = 0; i < staticSamplers.size(); i++)
        vkDestroySampler(cgd.GetCGDEntity().device, staticSamplers[i], nullptr);
    for(auto i = 0; i < RootParameterSetCount + 1; i++)
        vkDestroyDescriptorSetLayout(cgd.GetCGDEntity().device,
            descriptorSetLayout[i], nullptr);
    vkDestroyDescriptorPool(cgd.GetCGDEntity().device, 
            pool, nullptr);
}

RootParameterVk::RootParameterVk(const CGDVk& _cgd, 
    const RootSignatureVk* rootSignature,
    const RootParameterSet _targetSet,
    const VkDescriptorSetLayout& layout, VkDescriptorPool pool)
    :cgd(_cgd), rootSig(rootSignature), targetSet(_targetSet)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;
    if (vkAllocateDescriptorSets(
        _cgd.GetCGDEntity().device, &allocInfo, &descriptorSet) != VK_SUCCESS) 
    {
        CGDVk::error("Create Root Argument: failed to allocate descriptor sets!");
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

RootParameterVk::~RootParameterVk()
{
    
}

const SignatureSlotType RootParameterVk::GetType(void) const
{
    return type;
}

const size_t RootParameterVk::GetSlotNum(void) const
{
    return 0;
}

RootSignature* CGDVk::CreateRootSignature(
    const RootSignatureCreateInfo& sigInfo) const
{
    return new RootSignatureVk(*this, sigInfo);
}

RootParameter* RootSignatureVk::CreateArgument(const RootParameterSet targetSet) const
{
	auto res = new RootParameterVk(cgd, this,
		targetSet, descriptorSetLayout[targetSet], pool);
    res->staticSamplers = &staticSamplerSet;
    return res;
}

void RootParameterVk::UpdateArgument(
    const RootParameterAttachment* attachments, std::uint32_t attachmentCount)
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
            CGDVk::error("Root argument Slot type not supported!");
            return;
        }
    }
}