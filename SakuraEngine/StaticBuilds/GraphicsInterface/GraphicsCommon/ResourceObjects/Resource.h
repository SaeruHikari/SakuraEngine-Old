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
 * @Date: 2020-03-05 17:36:56
 * @LastEditTime: 2020-03-20 23:17:50
 */
#pragma once
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../Flags/ResourceFlags.h"
#include "../Flags/CommonFeatures.h"
#include "../Flags/GraphicsPipelineStates.h"
#include <functional>

namespace Sakura::Graphics
{
    enum ImageAspectFlag
    {
        ImageAspectColor = 0x00000001,
        ImageAspectDepth = 0x00000002,
        ImageAspectStencil = 0x00000004,
        ImageAspectMetaData = 0x00000008,
        ImageAspectPlane0 = 0x00000010,
        ImageAspectPlane1 = 0x00000020,
        ImageAspectPlane2 = 0x00000040,
        ImageAspectMemoryPlane0 = 0x00000080,
        ImageAspectMemoryPlane1 = 0x00000100,
        ImageAspectMemoryPlane2 = 0x00000200,
        ImageAspectMemoryPlane3 = 0x00000400,
        ImageAspectMaxEnum = 0x7FFFFFFF
    };
    using ImageAspectFlags = uint32_t;
    
    struct ImageSubresourceLayers
    {
        uint32_t mipLevel; 
        uint32_t baseArrayLayer;
        uint32_t layerCount;
        ImageAspectFlags aspectMask;
    };

    struct BufferImageCopy
    {
        uint64 bufferOffset = 0;
        uint32 bufferRowLength = 0;
        uint32 bufferImageHeight = 0;
        ImageSubresourceLayers imageSubresource;
        Offset3D imageOffset = {0, 0, 0};
        Extent3D imageExtent = {0, 0, 0};
    };

    struct BufferCreateInfo
    {
        BufferUsages usage;
        CPUAccessFlags cpuAccess;
        uint64 size;
    };

    struct TextureCreateInfo
    {
        ImageUsages usage;
        uint32 width;
        uint32 height;
        Format format;
        uint32 depth = 1;
        uint32 mipLevels = 1;
        uint32 arrayLayers = 1;
    };
    
    sinterface GpuResource
    {
        friend sinterface CGD;
        virtual ~GpuResource() = default;
        virtual void Map(void** data) = 0;
        virtual void Unmap() = 0;
        inline void UpdateValue(std::function<void(void*)> func)
        {
            void* data;
            Map(&data);
            func(data);
            Unmap();
        }
        const Extent2D GetExtent() const
        {
            return extent;
        }
    protected:
        GpuResource() = default;
        GpuResource(Extent2D _extent)
            :extent(_extent){}
        void SetExtent(Extent2D _extent)
        {
            extent = _extent;
        }
    protected:
        Extent2D extent;
    };
    
    sinterface GpuBuffer : simplements GpuResource
    {
        GpuBuffer() = default;
        GpuBuffer(Extent2D _extent)
            :GpuResource(_extent){}
    };

    sinterface GpuTexture : simplements GpuResource
    {
        GpuTexture() = default;
        GpuTexture(Extent2D _extent)
            :GpuResource(_extent){}
    };

    struct SamplerCreateInfo
    {
        bool unnormalizedCoordinates = false;
        bool compareEnable = false;
        bool anisotropyEnable = true;
        float maxAnisotropy = 16.f;
        float minLod;
        float maxLod;
        float mipLodBias;
        Filter magFilter = Filter::FilterLinear;
        Filter minFilter = Filter::FilterLinear;
        SamplerAddressMode addressModeU = SamplerAddressMode::AddressModeRepeat;
        SamplerAddressMode addressModeV = SamplerAddressMode::AddressModeRepeat;
        SamplerAddressMode addressModeW = SamplerAddressMode::AddressModeRepeat;
        CompareOp compareOp = CompareOp::CompareOpAlways;
        SamplerMipmapMode mipmapMode = SamplerMipmapMode::SamplerMipmapModeLinear;
    };

    sinterface Sampler 
    {
        Sampler() = default;
        virtual ~Sampler(){}
    };
}