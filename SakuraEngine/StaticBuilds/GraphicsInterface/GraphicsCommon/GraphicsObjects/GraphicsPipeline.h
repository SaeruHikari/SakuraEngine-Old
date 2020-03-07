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
 * @LastEditTime: 2020-03-07 11:34:28
 */
#pragma once
#include <memory_resource>
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../ResourceObjects/Shader.h"

namespace Sakura::Graphics
{
    struct VertexInput
    {

    };
    
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

    struct ShaderStageCreateInfo
    {
        StageFlags stage;
        Shader* shader = nullptr;
        spmr_string entry;
    };

    struct GraphicsPipelineCreateInfo
    {
        InputAssemblyStateCreateInfo assemblyStateCreateInfo;
        std::pmr::vector<ShaderStageCreateInfo> stages;
        ViewportStateCreateInfo viewportStateCreateInfo;
    };

    SInterface GraphcisPipeline
    {
        virtual void Create(const ShaderStageCreateInfo& stageToBind) = 0;
    };
}
