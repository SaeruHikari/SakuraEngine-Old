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
 * @Date: 2020-03-05 17:35:59
 * @LastEditTime: 2020-03-18 09:04:25
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../Flags/CommonFeatures.h"
#include "../Flags/Format.h"
#include "../Flags/GraphicsPipelineStates.h"

namespace Sakura::Graphics
{
    SInterface CGD;
    SInterface GpuResource;
}

struct VkImageViewCreateInfo;

namespace Sakura::Graphics
{
    enum ResourceViewType
    {
        ImageView1D,
        ImageView2D,
        ImageView3D,
        ImageViewCube,
        ImageView1DArray,
        ImageView2DArray,
        ImageViewCubeArray,
        ImageViewTypesCount,
        BufferView
    };
    inline static bool isImageView(const ResourceViewType type)
    {
        return type <= ImageViewCubeArray;
    }
    
    struct ResourceViewCreateInfo
    {
        struct Tex2DInfo
        {
            uint16 baseMipLevel = 0;
            uint16 mipLevels = 1;
            uint16 baseArrayLayer = 0;
            uint16 layerCount = 1;
        };
        struct Tex3DInfo
        {

        };
        struct BufferInfo
        {

        };
        Format format;
        ResourceViewType viewType;
        union View
        {
            Tex2DInfo texture2D;
            Tex3DInfo texture3D;
            BufferInfo buffer;
        } view;
    };
    
    SInterface ResourceView
    {
        virtual ~ResourceView(){}
        const Format GetFormat(void) const {return format;}
        const ResourceViewType GetViewType(void) const  
        {
            return viewType;
        }
        virtual void Attach(const GpuResource&, const ResourceViewCreateInfo&) = 0;
        virtual void Detach() = 0;
    protected:
        ResourceView(const CGD& _device, const ResourceViewType vt)
            :device(_device), viewType(vt){}
    protected:
        Format format;
        ResourceViewType viewType;
        const CGD& device;
    };

    SInterface VertexBufferView
    {
        const GpuResource& vb;
        uint32 stride;
        uint32 size;
        VertexInputRate rate = VertexInputRate::VertexInputRateVertex;
    };

    SInterface IndexBufferView
    {   

    };
}