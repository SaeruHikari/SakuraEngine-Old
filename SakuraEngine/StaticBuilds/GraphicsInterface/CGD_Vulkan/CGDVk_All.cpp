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
 * @Description: Unity Build unit of CGDVk
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-11 08:10:11
 * @LastEditTime: 2020-03-11 08:26:23
 */
#include "CGD_Vulkan.cpp"
#include "CommandObjects/CommandContextVk.cpp"
#include "../CGD/CommandContext.cpp"
#include "CommandObjects/CommandQueueVk.cpp"
#include "../CGD/Profiling.cpp"
#include "ResourceObjects/GpuResourceVk.cpp"
#include "ResourceObjects/ResourceViewVk.cpp"
#include "ResourceObjects/ShaderVk.cpp"
#include "GraphicsObjects/SwapChainVk.cpp"
#include "GraphicsObjects/GraphicsPipelineVk.cpp"
#include "GraphicsObjects/RenderProgressVk.cpp"