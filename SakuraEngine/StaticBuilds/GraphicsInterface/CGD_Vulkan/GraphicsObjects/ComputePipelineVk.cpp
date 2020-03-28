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
 * @Date: 2020-03-23 10:22:14
 * @LastEditTime: 2020-03-25 17:13:37
 */
#include "ComputePipelineVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

ComputePipeline* CGDVk::CreateComputePipeline(const ComputePipelineCreateInfo& info) const
{
	return new ComputePipelineVk(info, *this);
}

ComputePipelineVk::~ComputePipelineVk()
{
    vkDestroyPipeline(cgd.GetCGDEntity().device, pipeline, nullptr);
    vkDestroyPipelineLayout(
        cgd.GetCGDEntity().device, pipelineLayout, nullptr);
}

ComputePipelineVk::ComputePipelineVk(const ComputePipelineCreateInfo& info,
    const CGDVk& _cgd)
    :cgd(_cgd)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	if (info.rootSignature)
	{
		pipelineLayoutInfo.setLayoutCount = RootParameterSetCount + 1;
		pipelineLayoutInfo.pSetLayouts
			= ((const RootSignatureVk*)info.rootSignature)->descriptorSetLayout;
	}
	if (vkCreatePipelineLayout(cgd.GetCGDEntity().device,
		&pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		Sakura::Graphics::Vk::CGDVk::debug_error("failed to create pipeline layout!");
		throw std::runtime_error("failed to create pipeline layout!");
	}
	VkComputePipelineCreateInfo computePipelineCreateInfo = {};
	computePipelineCreateInfo.sType = 
		VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.layout = pipelineLayout;
	computePipelineCreateInfo.flags = 0;
	computePipelineCreateInfo.stage = Transfer(info.shaderStage);
	if (vkCreateComputePipelines(cgd.GetCGDEntity().device, 
		cgd.GetCGDEntity().pipelineCache,
		1, &computePipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS)
	{
		Sakura::Graphics::Vk::CGDVk::debug_error("failed to create compute pipeline!");
		throw std::runtime_error("failed to create compute pipeline!");
	}
}