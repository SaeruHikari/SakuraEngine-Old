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
 * @Date: 2020-03-18 09:13:31
 * @LastEditTime: 2020-03-25 09:28:52
 */
#pragma once
#include <vector>
#include <memory_resource>
#include "../../GraphicsCommon/GraphicsObjects/RootSignature.h"
#include "vulkan/vulkan.h"
#include "SakuraEngine/Core/EngineUtils/log.h"

namespace Sakura::Graphics::Vk
{
    class RootParameterVk;
}

namespace Sakura::Graphics::Vk
{
    class RootSignatureVk final : simplements Sakura::Graphics::RootSignature
    {
        friend class CGD_Vk;
    public:
        virtual ~RootSignatureVk() final override;
        VkDescriptorSetLayout descriptorSetLayout[RootParameterSetCount + 1];
        VkDescriptorPool pool = VK_NULL_HANDLE;
        virtual [[nodiscard]] RootParameter* CreateArgument(
            const RootParameterSet targetSet) const override final;
    protected:
        std::vector<VkSampler> staticSamplers;
        VkDescriptorSet staticSamplerSet;
        RootSignatureVk(const CGD_Vk& _cgd, const RootSignatureCreateInfo& info);
        const CGD_Vk& cgd;
    };

    class RootParameterVk final : simplements Sakura::Graphics::RootParameter
    {
        friend class CGD_Vk;
        friend class RootSignatureVk;
        friend class CommandContextVk;
    public:
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        virtual ~RootParameterVk() override final;
        virtual const SignatureSlotType GetType(void) const override final;
        virtual void UpdateArgument(const RootParameterAttachment* attachments,
            std::uint32_t attachmentCount) override final;
        virtual const size_t GetSlotNum(void) const override final;
    protected:
		RootParameterVk(const CGD_Vk& _cgd, const RootSignatureVk*,
			const RootParameterSet targetSet,
            const VkDescriptorSetLayout& layout, VkDescriptorPool pool);
        const RootSignatureVk* rootSig;
        uint32_t slotNum = 0;
        const RootParameterSet targetSet;
        SignatureSlotType type;
        const CGD_Vk& cgd; 
    };

    inline static const VkDescriptorType Transfer(const SignatureSlotType in)
    {
        if(in != InlineUniformBlockExt || in != AccelerationStructureNv)
            return ((VkDescriptorType)in);
        else 
        {
            switch (in)
            {
            case SignatureSlotType::InlineUniformBlockExt:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
            case SignatureSlotType::AccelerationStructureNv:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;            
            default:
                Sakura::log::error("Vulkan: Failed to transfer SlotType to VkDescriptorType!");
            }
        }
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_END_RANGE;
    }
}