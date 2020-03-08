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
 * @Date: 2020-03-08 21:06:12
 * @LastEditTime: 2020-03-08 21:43:00
 */
#include "GraphicsPipelineVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

GraphicsPipelineVk::~GraphicsPipelineVk()
{
    vkDestroyPipelineLayout(
        cgd.GetCGDEntity().device, pipelineLayout, nullptr);
}

GraphicsPipelineVk::GraphicsPipelineVk(
    const GraphicsPipelineCreateInfo& info, const CGD_Vk& _cgd)
        :cgd(_cgd)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = 
        Transfer(info.pipelineLayoutInfo);
    if (vkCreatePipelineLayout(cgd.GetCGDEntity().device,
        &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        Sakura::Graphics::Vk::CGD_Vk::debug_error("failed to create pipeline layout!");
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

std::unique_ptr<GraphicsPipeline> CGD_Vk::CreateGraphicsPipeline(
    const GraphicsPipelineCreateInfo& info) const
{
    auto* vkPipeline = new GraphicsPipelineVk(info, *this);
    return std::move(std::unique_ptr<GraphicsPipeline>(vkPipeline));
}