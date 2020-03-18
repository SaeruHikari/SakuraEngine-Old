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
 * @LastEditTime: 2020-03-18 09:22:47
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include <vector>
#include "../ResourceObjects/Shader.h"

namespace Sakura::Graphics
{
    enum SignatureSlotType
    {
        DescriptorTable = 0,
        RootDescriptor = 1,
        RootConstants = 2,
        SignatureSlotTypeCount
    };
    struct SignatureSlot
    {
        SignatureSlotType type;
        ShaderStages stageFlags;
    };
    struct RootSignatureCreateInfo
    {
        const SignatureSlot* paramSlots;
        const uint32_t paramSlotNum;
    };
    SInterface RootSignature
    {
        
    };
} // namespace Sakura::Graphics

