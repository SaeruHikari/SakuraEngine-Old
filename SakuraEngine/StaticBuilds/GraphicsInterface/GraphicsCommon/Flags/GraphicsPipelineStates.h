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
 * @Date: 2020-03-06 16:47:38
 * @LastEditTime: 2020-03-08 00:43:46
 */
#pragma once
#include <memory_resource>
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../ResourceObjects/Shader.h"
#include "PixelFormat.h"

namespace Sakura::Graphics
{
    enum PrimitiveTopology
    {
        PointList = 0,
        LineList = 1,
        LineStrip = 2,
        TriangleList = 3,
        TriangleStrip = 4,
        TriangleFan = 5,
        LineListWithAdjacency = 6,
        LineStripWithAdjacency = 7,
        TriangleListWithAdjacency = 8,
        TriangleStripWithAdjacency = 9,
        PatchList = 10
    };

    enum PolygonMode
    {
        PolygonFill = 0,
        PolygonLine = 1,
        PolygonPoint = 2,
        PolygonFillRectangleNV = 1000153000
    };

    enum SampleCountFlag
    {
        SampleCount_1Bit = 0x00000001,
        SampleCount_2Bit = 0x00000002,
        SampleCount_4Bit = 0x00000004,
        SampleCount_8Bit = 0x00000008,
        SampleCount_16Bit = 0x00000010,
        SampleCount_32Bit = 0x00000020,
        SampleCount_64Bit = 0x00000040
    };

    enum CullModes
    {
        CullModeNone = 0,
        CullModeFront = 0x00000001,
        CullModeBack = 0x00000002,
        CullModeFrontAndBack = 0x00000003
    };

    enum FrontFace
    {
        FrontFaceCounterClockWise = 0,
        FrontFaceClockWise = 1
    };

    struct InputAssemblyStateCreateInfo
    {
        PrimitiveTopology topo = PrimitiveTopology::TriangleList;
        bool primitiveRestartEnable = false;
    };

    struct Extent2D
    {
        uint32 width;
        uint32 height;
    };

    struct Offset2D
    {
		int32 x = 0;
		int32 y = 0;
    };

    struct Rect2D
    {
        Offset2D offset;
        Extent2D extent;
    };

    struct Viewport
    {
        float x = 0.f;
        float y = 0.f;
        float width = 0.f;
        float height = 0.f;
        float minDepth = 0.f;
        float maxDepth = 1.f;
    };

    struct ViewportStateCreateInfo
    {
        uint32 viewportCount = 1;
        Viewport* vps;
        uint32 scissorCount = 1;
        Rect2D* scissors = nullptr;
    };

    struct DepthStencilStateCreateInfo
    {
        bool depthTestEnable = true;
        bool depthWriteEnable = true;
        bool depthBoundsTestEnable = false;
        bool stencilTestEnable = false;
        float minDepthBounds;
        float maxDepthBounds;
    };

    struct ShaderStageCreateInfo
    {
        StageFlags stage;
        Shader* shader = nullptr;
        spmr_string entry;
    };

    struct RasterizationStateCreateInfo
    {
        bool depthClampEnable = false;
        CullModes cullMode = CullModes::CullModeBack;
        PolygonMode polygonMode = PolygonMode::PolygonFill;
        FrontFace frontFace = FrontFace::FrontFaceClockWise;
        bool depthBiasEnable = false;
        float depthBiasConstantFactor = 0.f;
        float depthBiasClamp = 0.f;
        float depthBiasSlopeFactor = 0.f;
    };

    struct MultisampleStateCreateInfo
    {
        bool sampleShadingEnable = false;
        SampleCountFlag rasterizationSamples 
            = SampleCountFlag::SampleCount_1Bit; 
        float minSampleShading = 1.f;
        bool alphaToCoverageEnable = false;
        bool alphaToOneEnable = false;
    };

    struct ColorBlendAttachmentState
    {
        ColorComoponents colorWriteMask =
            ColorComponentR | ColorComponentG |
            ColorComponentB | ColorComponentA;  
        bool blendEnable = false;
        BlendFactor srcColorBlendFactor = BlendFactor::BlendFactorOne;
        BlendFactor dstColorBlendFactor = BlendFactor::BlendFactorZero;
        BlendOp colorBlendOp = BlendOp::BlendOpAdd;
        BlendFactor srcAlphaBlendFactor = BlendFactor::BlendFactorOne;
        BlendFactor dstAlphaBlendFactor = BlendFactor::BlendFactorZero;
        BlendOp alphaBlendOp = BlendOp::BlendOpAdd;
    };

    struct GraphicsPipelineCreateInfo
    {
        InputAssemblyStateCreateInfo assemblyStateCreateInfo;
        std::pmr::vector<ShaderStageCreateInfo> stages;
        ViewportStateCreateInfo viewportStateCreateInfo;
    };
}
