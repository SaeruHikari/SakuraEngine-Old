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
 * @LastEditTime: 2020-03-07 01:20:42
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
    
    enum class Topology
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip
    };

    struct InputAssemblyStateCreateInfo
    {
        Topology topo = Topology::TriangleList;
        bool primitiveRestartEnable = false;
    };

    struct Rect2D
    {
        Rect2D(uint32 _extentX, uint32 _extentY)
            :ExtentX(_extentX), ExtentY(_extentY){}
        int offsetX = 0;
        int offsetY = 0;
        uint32 ExtentX;
        uint32 ExtentY;
    };

    struct Viewport
    {
        Viewport(float _width, float _height)
            :width(_width), height(_height){}
        float x = 0.f;
        float y = 0.f;
        float width = 0.f;
        float height = 0.f;
    };

    struct ViewportStateCreateInfo
    {
        ViewportStateCreateInfo(float _width, float _height)
            :vp(_width, _height)
        {
            scissors = new Rect2D(_width, _height);
        }
        uint32 viewportCount = 1;
        Viewport vp;
        uint32 scissorCount = 1;
        Rect2D* scissors = nullptr;
    };

    struct ShaderStage
    {
        StageFlags stage;
        Shader* shader = nullptr;
        std::pmr::string entry;
    };

    struct GraphicsPipelineCreateInfo
    {
        GraphicsPipelineCreateInfo(
            const std::size_t stageNum, float width, float height) 
            :viewportStateCreateInfo(width, height)
        {
            stages.resize(stageNum);
        }
        InputAssemblyStateCreateInfo assemblyStateCreateInfo;
        std::pmr::vector<ShaderStage> stages;
        ViewportStateCreateInfo viewportStateCreateInfo;
    };
    SInterface GraphcisPipeline
    {
        virtual void Create(const ShaderStage& stageToBind) = 0;
    };
}
