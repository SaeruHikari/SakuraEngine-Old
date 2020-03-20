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
 * @Date: 2020-03-07 23:25:42
 * @LastEditTime: 2020-03-20 16:13:45
 */
#pragma once
#include "../Flags/GraphicsPipelineStates.h"
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../ResourceObjects/ResourceViews.h"
#include "RootSignature.h"

namespace Sakura::Graphics
{
    union ClearColorValue 
    {
        float       float32[4];
        int32_t     int32[4];
        uint32_t    uint32[4];
    };
    struct ClearDepthStencilValue 
    {
        float       depth;
        uint32_t    stencil;
    };
    struct RenderTarget
    {
        const GpuResource* resource = nullptr;
        const ResourceView* srv = nullptr;
        union ClearValue
        {
            ClearColorValue clearColor;
            ClearDepthStencilValue clearDepth;
        } clearValue;
    };

    struct RenderTargetSet
    {
        RenderTarget* rts;
        uint32 rtNum;
    };
    sinterface GraphicsPipeline
    {
        virtual ~GraphicsPipeline() = default;
    protected:
        GraphicsPipeline() = default;
    };
}