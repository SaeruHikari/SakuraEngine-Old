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
 * @LastEditTime: 2020-03-09 12:32:25
 */
#pragma once
#include "SakuraEngine/Core/CoreMinimal/CoreMinimal.h"
#include "../ResourceObjects/ShaderVk.h"
#include "../../GraphicsCommon/Flags/GraphicsPipelineStates.h"
#include "FormatVk.h"

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

    sinline static VkPolygonMode Transfer(const PolygonMode mode)
    {
        return VkPolygonMode(mode);
    }

    sinline static PolygonMode Transfer(const VkPolygonMode mode)
    {
        return PolygonMode(mode);
    }

    sinline static CullModes Transfer(const VkCullModeFlagBits vkMode)
    {
        return CullModes(vkMode);
    }

    sinline static VkCullModeFlagBits Transfer(const CullModes mode)
    {
        return VkCullModeFlagBits(mode);
    }

    sinline static VkFrontFace Transfer(const FrontFace frontFace)
    {
        return VkFrontFace(frontFace);
    }

    sinline static FrontFace Transfer(const VkFrontFace frontFace)
    {
        return FrontFace(frontFace);
    }

    sinline static VkSampleCountFlagBits Transfer(const SampleCountFlag flag)
    {
        return VkSampleCountFlagBits(flag);
    }
     
    sinline static SampleCountFlag Transfer(const VkSampleCountFlagBits flag)
    {
        return SampleCountFlag(flag);
    }

    sinline static VkBlendFactor Transfer(const BlendFactor factor)
    {
        return VkBlendFactor(factor);
    }

    sinline static BlendFactor Transfer(const VkBlendFactor factor)
    {
        return BlendFactor(factor);
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

    sinline static VkPipelineVertexInputStateCreateInfo Transfer(
        const VertexInputStateCreateInfo& vertInfo)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = 
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount 
            = vertInfo.vertexBindingDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions
            = (const VkVertexInputBindingDescription*)(vertInfo.vertexAttributeDescriptions.data());
        vertexInputInfo.vertexAttributeDescriptionCount 
            = vertInfo.vertexAttributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions
            = (const VkVertexInputAttributeDescription*)(vertInfo.vertexAttributeDescriptions.data());
        return vertexInputInfo;
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

    sinline static VkDynamicState Transfer(const DynamicState state)
    {
        return VkDynamicState(state);
    }

    sinline static VkPipelineViewportStateCreateInfo Transfer(
        const ViewportStateCreateInfo& info)
    {
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pScissors = (const VkRect2D*)info.scissors.data();
        viewportState.scissorCount = info.scissors.size();
        viewportState.pViewports = (const VkViewport*)info.vps.data();
        viewportState.viewportCount = info.vps.size();
        return viewportState;
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

    sinline static VkPipelineRasterizationStateCreateInfo Transfer(
        const RasterizationStateCreateInfo& info)
    {
        VkPipelineRasterizationStateCreateInfo rasterizer  = {};
        rasterizer.sType = 
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = 
            info.depthClampEnable ? VK_TRUE : VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = Transfer(info.polygonMode);
        rasterizer.lineWidth = 1.f;
        rasterizer.cullMode = Transfer(info.cullMode);
        rasterizer.frontFace = Transfer(info.frontFace);
        rasterizer.depthBiasEnable = 
            info.depthBiasEnable ? VK_TRUE : VK_FALSE;
        rasterizer.depthBiasConstantFactor =
            info.depthBiasConstantFactor; 
        rasterizer.depthBiasClamp = info.depthBiasClamp;
        rasterizer.depthBiasSlopeFactor = info.depthBiasSlopeFactor; 
        return rasterizer;
    }

    sinline static VkPipelineMultisampleStateCreateInfo Transfer(
        const MultisampleStateCreateInfo& in)
    {
        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.alphaToCoverageEnable 
            = in.alphaToCoverageEnable ? VK_TRUE : VK_FALSE;
        multisampling.alphaToOneEnable
            = in.alphaToOneEnable ? VK_TRUE : VK_FALSE;
        multisampling.sampleShadingEnable
            = in.sampleShadingEnable ? VK_TRUE : VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = in.minSampleShading;
        multisampling.pSampleMask = nullptr;
        multisampling.rasterizationSamples
            = Transfer(in.rasterizationSamples);
        return multisampling;    
    }

    sinline static VkPipelineColorBlendAttachmentState Transfer(
        const ColorBlendAttachmentState& state)
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = state.colorWriteMask;
        colorBlendAttachment.blendEnable = 
            state.blendEnable ? VK_TRUE : VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor 
            = Transfer(state.srcColorBlendFactor); 
        colorBlendAttachment.dstColorBlendFactor 
            = Transfer(state.dstColorBlendFactor); 
        colorBlendAttachment.colorBlendOp 
            = Transfer(state.colorBlendOp); 
        colorBlendAttachment.srcAlphaBlendFactor 
            = Transfer(state.srcAlphaBlendFactor);
        colorBlendAttachment.dstAlphaBlendFactor
            = Transfer(state.dstAlphaBlendFactor); 
        colorBlendAttachment.alphaBlendOp 
            = Transfer(state.alphaBlendOp);
        return colorBlendAttachment;
    }

    sinline static VkPipelineDynamicStateCreateInfo Transfer(
        const DynamicStateCreateInfo dynamicCreateInfo)
    {
        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = 
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pDynamicStates 
            = (const VkDynamicState*)dynamicCreateInfo.dynamicStates.data();
        dynamicState.dynamicStateCount 
            = dynamicCreateInfo.dynamicStates.size();
        return dynamicState;
    }

    sinline static VkDescriptorSetLayout Transfer(const DescriptorSetLayout layout)
    {
        VkDescriptorSetLayout res = {};
        return res;
    }

    sinline static VkPushConstantRange Transfer(const PushConstantRange range)
    {
        VkPushConstantRange res = {};
        return res;
    }
}
