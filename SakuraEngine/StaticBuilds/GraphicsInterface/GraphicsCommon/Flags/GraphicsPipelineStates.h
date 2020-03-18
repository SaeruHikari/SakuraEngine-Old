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
 * @LastEditTime: 2020-03-18 09:44:00
 */
#pragma once
#include <memory_resource>
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "Core/Containers/SString.h"
#include "../ResourceObjects/Shader.h"
#include "Format.h"

using namespace Sakura;

namespace Sakura::Graphics
{
    enum PipelineStageFlags
    {
        TopOfPipeStage = 0x00000001,
        DrawIndirectStage = 0x00000002,
        VertexInputStage = 0x00000004,
        VetrtexShaderStage = 0x00000008,
        HullShaderStage = 0x00000010,
        DomainShaderStage = 0x00000020,
        GeometryShaderStage = 0x00000040,
        PixelShaderStage = 0x00000080,
        EarlyPixelTestsStage = 0x00000100,
        LatePixelTestsStage = 0x00000200,
        ColorAttachmentOutputStage = 0x00000400,
        ComputeShaderStage = 0x00000800,
        TransferStage = 0x00001000,
        BottomOfPipeStage = 0x00002000,
        HostStage = 0x00004000,
        AllGraphicsStage = 0x00008000,
        AllCommandsStage = 0x00010000,
        TransformFeedBackStage = 0x01000000,
        ConditionalRenderingStage = 0x00040000,
        CommandProcessStage = 0x00020000,
        ShadingRateImageStage = 0x00400000,
        RayTracingShaderStage = 0x00200000,
        AccelerationStructureBuildStage = 0x02000000,
        TaskShaderStage = 0x00080000,
        MeshShaderStage = 0x00100000,
        FragmentDensityProcessStage = 0x00800000,
        BUFFER_PIPELINE_STAGE_MAX_ENUM = 0x7FFFFFFF
    };
    using PipelineStageFlag = uint32;
    
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
        PolygonFillRectangleNV = 1000153000,
        POLYGON_MODE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
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
    
    enum DynamicState
    {
        DynamicStateViewport = 0,
        DynamicStateScissor = 1,
        DynamicStateLineWidth = 2,
        DynamicStateDepthBias = 3,
        DynamicStateBlendConstants = 4,
        DynamicStateDepthBounds = 5,
        DynamicStateStencilCompareMask = 6,
        DynamicStateStencilWriteMask = 7,
        DynamicStateStencilReference = 8,
        DynamicStateViewportWScalingNV = 1000087000,
        DynamicStateDiscardRectangleExt = 1000099000,
        DynamicStateSampleLocationsExt = 1000143000,
        DynamicStateViewportShadingRatePaletteNV = 1000164004,
        DynamicStateViewportCoarseSampleOrderNV = 1000164006,
        DynamicStateExclusiveScissorNV = 1000205001,
        DynamicStateLineStrippleExt = 1000259000
    };

    enum FrontFace
    {
        FrontFaceCounterClockWise = 0,
        FrontFaceClockWise = 1
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

    enum VertexInputRate
    {
        VertexInputRateVertex = 0,
        VertexInputRateInstance = 1
    };

    struct InputAssemblyStateCreateInfo
    {
        PrimitiveTopology topo = PrimitiveTopology::TriangleList;
        bool primitiveRestartEnable = false;
    };

    struct VertexInputBindingDescription
    {
        uint32 binding;
        uint32 stride;
        VertexInputRate inputRate;
    };

    struct VertexInputAttributeDescription
    {
        uint32 location;
        uint32 binding;
        Format format;
        uint32 offset;
    };

    struct VertexInputStateCreateInfo 
    {
        const VertexInputBindingDescription* vertexBindingDescriptions;
        uint32_t vertexBindingDescriptionCount = 0;
        const VertexInputAttributeDescription* vertexAttributeDescriptions;
        uint32_t vertexAttributeDescriptionCount = 0;
    };

    struct ViewportStateCreateInfo
    {
        const Viewport* vps = nullptr;
        uint32_t vpCount = 0;
        const Rect2D* scissors;
        uint32_t scissorCount = 0;
    };
    
    struct DepthStencilStateCreateInfo
    {
        bool depthTestEnable = true;
        bool depthWriteEnable = true;
        bool depthBoundsTestEnable = false;
        bool stencilTestEnable = false;
        float minDepthBounds = 0.f;
        float maxDepthBounds;
    };

    struct ShaderStageCreateInfo
    {
        ShaderStageFlags stage;
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
        float lineWidth = 1.f;
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
    inline static const ColorBlendAttachmentState defaultAttachment = {};

    struct ColorBlendStateCreateInfo
    {
        bool logicOpEnable = false;
        LogicOp logicOp = LogicOp::LogicOpCopy;
        const ColorBlendAttachmentState* colorBlendAttachment = &defaultAttachment;
        uint32_t colorBlendAttachmentCount = 1;
        float blendConstants[4] = {0.f, 0.f, 0.f, 0.f};
    };

    struct DynamicStateCreateInfo
    {
        const DynamicState* dynamicStates;
        uint32_t dynamicStateCount;
    };

    struct DescriptorSetLayout
    {

    };

    struct PushConstantRange
    {

    };

    /**
     * @description: uniform values in the shaders. 
     * @author: SaeruHikari
     */
    struct PipelineLayoutCreateInfo
    {
        const DescriptorSetLayout* setLayouts = nullptr;
        uint32_t setLayoutCount = 0;
        const PushConstantRange* pushConstantRanges;
        uint32_t pushConstantRangeCount = 0;
    };

    /**
     * @description: fill this structure to Create PSO Object.
     * @author: SaeruHikari
     */
    struct GraphicsPipelineCreateInfo
    {
        InputAssemblyStateCreateInfo inputassembly;
        VertexInputStateCreateInfo vertexInputInfo;
        ViewportStateCreateInfo viewportStateCreateInfo;
        DepthStencilStateCreateInfo depthStencilCreateInfo;
        RasterizationStateCreateInfo rasterizationStateCreateInfo;
        MultisampleStateCreateInfo multisampleStateCreateInfo;
        DynamicStateCreateInfo dynamicStateCreateInfo;
        ColorBlendStateCreateInfo colorBlendStateCreateInfo;
        PipelineLayoutCreateInfo pipelineLayoutInfo;
        const ShaderStageCreateInfo* shaderStages = nullptr;
        uint32_t shaderStageCount = 0;
    };
}
