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
 * @Date: 2020-03-07 11:13:03
 * @LastEditTime: 2020-03-07 11:36:54
 */
#pragma once
#include "SakuraEngine/Core/CoreMinimal/CoreMinimal.h"
#include "../../GraphicsCommon/GraphicsObjects/GraphicsPipeline.h"
#include "../ResourceObjects/ShaderVk.h"


namespace Sakura::Graphics::Vk
{
    sinline static VkExtent2D Transfer(const Extent2D& extent)
    {
        return *(VkExtent2D*)(&extent);
    } 

    sinline static Extent2D Transfer(const VkExtent2D& extent)
    {
        return *(Extent2D*)(&extent);
    } 

    sinline static VkPrimitiveTopology Transfer(const PrimitiveTopology topo)
    {
        return VkPrimitiveTopology(topo);
    }

    sinline static PrimitiveTopology Transfer(const VkPrimitiveTopology topo)
    {
        return PrimitiveTopology(topo);
    }

    sinline static VkPipelineInputAssemblyStateCreateInfo Transfer(
        const InputAssemblyStateCreateInfo& assemblyInfo)
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = 
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = Transfer(assemblyInfo.topo);
        assemblyInfo.primitiveRestartEnable ? VK_TRUE : VK_FALSE;
        return inputAssembly;
    }

    sinline static InputAssemblyStateCreateInfo Transfer(
        const VkPipelineInputAssemblyStateCreateInfo& info)
    {
        InputAssemblyStateCreateInfo tran;
        tran.primitiveRestartEnable = info.primitiveRestartEnable == VK_TRUE;
        tran.topo = Transfer(info.topology);
        return tran;
    }

    sinline static VkRect2D Transfer(Rect2D rect2D)
    {
        return *(VkRect2D*)&rect2D;
    }

    sinline static Rect2D Transfer(VkRect2D rect2D)
    {
        return *(Rect2D*)&rect2D;
    }

    sinline static VkViewport Transfer(const Viewport& vp)
    {
        return *(VkViewport*)&vp;
    }

    sinline static Viewport Transfer(const VkViewport& vp)
    {
        return *(Viewport*)&vp;
    }

    sinline static VkPipelineViewportStateCreateInfo Transfer(
        const ViewportStateCreateInfo& info)
    {
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = info.viewportCount;
		viewportState.scissorCount = info.scissorCount;
        // Scissors
        if (info.scissorCount == 0)
            viewportState.pScissors = nullptr;
        else
        {
			std::vector<VkRect2D> scissors(info.scissorCount);
			for (auto i = 0u; i < info.scissorCount; i++)
			{
				scissors[i] = Transfer(info.scissors[i]);
			}
			viewportState.pScissors = scissors.data();
        }
		// Viewports
		if (info.viewportCount == 0)
			viewportState.pViewports = nullptr;
		else
		{
			std::vector<VkViewport> vps(info.scissorCount);
			for (auto i = 0u; i < info.scissorCount; i++)
			{
                vps[i] = Transfer(info.vps[i]);
			}
			viewportState.pViewports = vps.data();
		}
    }

    sinline static ViewportStateCreateInfo Transfer(const VkPipelineViewportStateCreateInfo& info)
    {
        ViewportStateCreateInfo res;
        res.scissorCount = info.scissorCount;
        res.viewportCount = info.viewportCount;

		// Scissors
		if (info.scissorCount == 0)
            res.scissors = nullptr;
		else
		{
			std::vector<Rect2D> scissors(info.scissorCount);
			for (auto i = 0u; i < info.scissorCount; i++)
			{
				scissors[i] = Transfer(info.pScissors[i]);
			}
            res.scissors = scissors.data();
		}
		// Viewports
		if (info.viewportCount == 0)
            res.vps = nullptr;
		else
		{
			std::vector<Viewport> vps(info.scissorCount);
			for (auto i = 0u; i < info.scissorCount; i++)
			{
				vps[i] = Transfer(info.pViewports[i]);
			}
            res.vps = vps.data();
		}
    }

	sinline static VkPipelineShaderStageCreateInfo Transfer(
        const ShaderStageCreateInfo& info)
	{
		VkPipelineShaderStageCreateInfo stage = {};
        stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage.stage = Transfer(info.stage);
        stage.module = ((ShaderVk*)info.shader)->shaderModule;
        stage.pName = info.entry.c_str();
        return stage;
	}
}
