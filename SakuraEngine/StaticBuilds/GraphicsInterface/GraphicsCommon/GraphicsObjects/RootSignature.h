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
 * @LastEditTime: 2020-03-18 16:57:18
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include <vector>
#include "../ResourceObjects/Shader.h"

namespace Sakura::Graphics
{
    SInterface GpuResource;
}

namespace Sakura::Graphics
{
    enum SignatureSlotType
    {
        SamplerSlot = 0,
        CombinedImageSamplerSlot = 1,
        SampledImageSlot = 2,
        StorageImageSlot = 3,
        UniformTexelBufferSlot = 4,
        StorageTexelBufferSlot = 5,
        UniformBufferSlot = 6,
        StorageBufferSlot = 7,
        UniformBufferDynamicSlot = 8,
        StorageBufferDynamicSlot = 9,
        InputAttachmentSlot = 10,
        InlineUniformBlockExt = 1000138000,
        AccelerationStructureNv = 1000165000
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
    };

    enum RootArgumentType
    {
        UniformBuffer
    };
    
    struct RootArgumentCreateInfo
    {
        struct UniformBufferInfo
        {
            uint32 offset = 0;
            uint32 range = 0;
            const GpuResource& buffer; 
        };
        RootArgumentType rootArgType;
        union Information
        {
            UniformBufferInfo uniformBuffer;
        } info;
    };
    SInterface RootArgument
    {
        virtual ~RootArgument(){};
        virtual const RootArgumentType GetType(void) const = 0;
    };

    SInterface RootSignature
    {
        virtual ~RootSignature(){};
        virtual [[nodiscard]] RootArgument* CreateArgument(
            uint32_t slot, const RootArgumentCreateInfo& info) const = 0;
    };
} // namespace Sakura::Graphics

