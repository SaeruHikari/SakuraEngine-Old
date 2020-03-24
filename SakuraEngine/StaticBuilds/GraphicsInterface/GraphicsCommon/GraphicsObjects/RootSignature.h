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
 * @Date: 2020-03-17 22:31:04
 * @LastEditTime: 2020-03-24 12:26:48
 */
#pragma once
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include <vector>
#include "../ResourceObjects/Shader.h"
#include "../ResourceObjects/ResourceViews.h"

namespace Sakura::Graphics
{
    sinterface GpuResource;
    sinterface GpuBuffer;
}

namespace Sakura::Graphics
{
    enum SignatureSlotType
    {
        SamplerSlot = 0,
        CombinedTextureSamplerSlot = 1,
        SampledImageSlot = 2,
        StorageImageSlot = 3,
        UniformTexelBufferSlot = 4,
        StorageTexelBufferSlot = 5,
        UniformBufferSlot = 6,
        StorageBufferSlot = 7,
        UniformBufferDynamicSlot = 8,
        StorageBufferDynamicSlot = 9,
        InputAttachmentSlot = 10,
		InlineUniformBlockExt = 11,
		AccelerationStructureNv = 12,
        SignatureSlotTypeCount = 13,
        SignatureSlotTypeMaxEnum = 0x7FFFFFFF
    };

    struct SignatureSlot
    {
        SignatureSlotType type;
        ShaderStages stageFlags;
    };

    struct RootSignatureCreateInfo
    {
        const SignatureSlot* paramSlots = nullptr;
        uint32_t paramSlotNum = 0;
        template<typename _SamplerCreateInfo>
        void PushStaticSampler(const _SamplerCreateInfo& info)
        {
            staticSamplers.push_back(info);
        }
        template<typename _SamplerCreateInfo, typename... Ts>
        void PushStaticSampler(const _SamplerCreateInfo& info, const Ts&... ts)
        {
            PushStaticSampler(info);
            PushStaticSampler(ts...);
        }
    private:    
        std::vector<SamplerCreateInfo> staticSamplers;
    };

    struct UniformBufferAttachment
    {
        uint32 offset = 0;
        uint32 range = 0;
        const GpuBuffer* buffer = nullptr; 
    };

    struct TexSamplerAttachment
    {
        const ResourceView* imageView = nullptr;
        const Sampler* sampler = nullptr;
        ImageLayout imageLayout;
    };

    struct RootArgumentAttachment
    {
        SignatureSlotType rootArgType = SignatureSlotType::UniformBufferSlot;
        uint32_t dstBinding = 0;
        uint32 dstArrayElement = 0;
        uint32 descriptorCount = 1;
        std::variant<UniformBufferAttachment, TexSamplerAttachment> info;
    };
    
    sinterface RootArgument
    {
        virtual ~RootArgument(){};
        virtual const SignatureSlotType GetType(void) const = 0;
        virtual void UpdateArgument(const RootArgumentAttachment* attachments,
            std::uint32_t attachmentCount) = 0;
        virtual const size_t GetSlotNum(void) const = 0;
    };

    sinterface RootSignature
    {
        virtual ~RootSignature(){};
        virtual [[nodiscard]] RootArgument* CreateArgument() const = 0;
    };
} // namespace Sakura::Graphics

