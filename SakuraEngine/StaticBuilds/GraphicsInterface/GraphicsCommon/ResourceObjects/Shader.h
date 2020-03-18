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
 * @Date: 2020-03-06 16:50:13
 * @LastEditTime: 2020-03-18 08:10:13
 */
#pragma once
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"

namespace Sakura::Graphics
{
    sinterface Shader
    {
        virtual ~Shader(){}
    };

    enum ShaderType
    {
        Undefined,
        VertexShader,
        HullShader, // Tessellation Control
        DomainShader,  // Tessellation Evaluation
        GeometryShader,
        PixelShader,
        ComputeShader,
        RayTracingGeneration,
        RayTracingAnyHit,
        RayTracingClosetHit,
        RayTracingMiss,
        MeshShader,
        ShaderTypeCount
    };

    enum ShaderSourceType
    {
        CodeString,
        BinaryBuffer,
        ShaderSourceTypeCount
    };

    enum ShaderCompileFlags
    {
        Debug       = (1 << 0), //!< Insert debug information.
        O1          = (1 << 1), //!< Optimization level 1.
        O2          = (1 << 2), //!< Optimization level 2.
        O3          = (1 << 3), //!< Optimization level 3.
        WarnError   = (1 << 4) //!< Warnings are treated as errors.
    };

    enum ShaderStageFlags
    {
        VertexStage         = 0x00000001,
        HullStage           = 0x00000002,
        DomainStage         = 0x00000004,
        GeometryStage       = 0x00000008,
        PixelStage          = 0x00000010,
        ComputeStage        = 0x00000020,
        AllGraphics         = 0x0000001F,
        RayGeneration       = 0x00000100,
        RayAnyHit           = 0x00000200,
        RayClosestHit       = 0x00000400,
        RayMiss             = 0x00000800,
        MeshStage           = 0x00000080,
    };
    using ShaderStages = uint32_t;
}