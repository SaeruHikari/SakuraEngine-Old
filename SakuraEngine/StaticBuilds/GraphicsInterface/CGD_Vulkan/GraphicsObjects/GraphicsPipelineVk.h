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
 * @Date: 2020-03-08 21:04:52
 * @LastEditTime: 2020-03-18 10:48:49
 */
#pragma once
#include "../../GraphicsCommon/GraphicsObjects/GraphicsPipeline.h"
#include <memory_resource>
#include <unordered_map>
#include "RenderPassVk.h"
#include "vulkan/vulkan.h"

using namespace Sakura::Graphics;
namespace Sakura::Graphics::Vk
{
    class CGD_Vk;
    class RenderPassVk;
}

namespace Sakura::Graphics::Vk
{
    class GraphicsPipelineVk final : simplements GraphicsPipeline
    {
        friend class CGD_Vk;
        friend class CommandContextVk;
        virtual ~GraphicsPipelineVk() override final;
    protected:
        VkFramebuffer FindFrameBuffer(const RenderTargetSet& rts);
        VkFramebuffer createFrameBuffer(const RenderTargetSet& rts);
        
        GraphicsPipelineVk(const GraphicsPipelineCreateInfo& info,
            const RenderPassVk& prog, const CGD_Vk& cgd);
    protected:
        std::pmr::unordered_map<uint64, VkFramebuffer> fbs;
        const RenderPassVk& progress;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        const CGD_Vk& cgd;
    };
}
