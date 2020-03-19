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
 * @Date: 2020-03-06 23:50:42
 * @LastEditTime: 2020-03-19 13:17:24
 */
#pragma once

namespace Sakura::Graphics
{
    enum class ResourceType
    {
        Undefined,
        Buffer,
        Texture1D,
        Texture2D,
        Texture3D,
        Sampler
    };

    enum ImageLayout
    {
        Unknown = 0,
        General = 1,
        ColorAttachment = 2,
        DepthStencilAttachment = 3,
        DepthStencilReadOnly = 4,
        TransferSrcOptimal = 6,
        TransferDstOptimal = 7,
        Preinitialized = 8,
        DepthReadOnlyStencilAttachment = 1000117000,
        DepthAttachmentStencilReadOnly = 1000117001,
        DepthAttachment = 1000241000,
        DepthReadOnly = 1000241001,
        StencilAttachment = 1000241002,
        StencilReadOnly = 1000241003,
        PresentSrc = 1000001002,
        SharedPresent = 1000111000,
        ShadingRateOptimalNV = 1000164003,
        FragmentDensityMapOptimalExt = 1000218000,
        IMAGE_LAYOUT_BITS_MAX_ENUM = 0x7FFFFFFF
    };

    enum CPUAccessFlags
    {
        None        = 0,
        Read        = (1 << 0),
        Write       = (1 << 1),
        ReadWrite   = (Read | Write)
    };

    enum class MiscFlags
    {
        DynamicUsage    = (1 << 0),
        GenerateMips    = (1 << 1),
        NoInitialData   = (1 << 2)
    };

    enum BufferUsage
    {
        TransferSrcBuffer = 0x00000001,
        TransferDstBuffer = 0x00000002,
        UniformTexelBuffer = 0x00000004,
        StorageTexelBuffer = 0x00000008,
        ConstantBuffer = 0x00000010,
        StorageBuffer = 0x00000020,
        IndexBuffer = 0x00000040,
        VertexBuffer = 0x00000080,
        IndirectBuffer = 0x00000100,
        ShaderDeviceAddress = 0x00020000,
        TransformFeedback = 0x00000800,
        TransformFeedbackCounter = 0x00001000,
        ConditionalRendering = 0x00000200,
        RayTracing = 0x00000400,
        BufferUsageMaxBit = 0x7FFFFFFF
    };
    using BufferUsages = uint32_t;

    enum ImageUsage
    {
        TransferSrcImage = 0x00000001,
        TransferDstImage = 0x00000002,
        SampledImage = 0x00000004,
        StorageImage = 0x00000008,
        ColorAttachmentImage = 0x000000010,
        DepthStencilAttachmentImage = 0x00000020,
        TransiendtAttachmentImage = 0x00000040,
        InputAttachmentImage = 0x00000080,
        ShadingRateImageNv = 0x00000100,
        FragmentDensityMapExt = 0x00000200,
        ImageUsageMaxBit = 0x7FFFFFFF
    };
    using ImageUsages = uint32_t;
}