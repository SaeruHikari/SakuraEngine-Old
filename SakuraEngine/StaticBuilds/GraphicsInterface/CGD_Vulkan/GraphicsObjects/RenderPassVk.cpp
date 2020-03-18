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
 * @Date: 2020-03-09 00:45:42
 * @LastEditTime: 2020-03-18 10:44:19
 */
#include "RenderPassVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

RenderPassVk::~RenderPassVk()
{
    vkDestroyRenderPass(cgd.GetCGDEntity().device, renderPass, nullptr);
}

RenderPassVk::RenderPassVk(
    const RenderPassCreateInfo& info, const CGD_Vk& _cgd)
        :cgd(_cgd)
{
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.pNext = nullptr;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = (uint32)info.attachments.size();
    VkAttachmentDescription* colorAttachments
        = new VkAttachmentDescription[info.attachments.size()];
    for(auto i = 0u; i < info.attachments.size(); i++)
    {
        colorAttachments[i] = Transfer(info.attachments[i]);
    }
    renderPassInfo.pAttachments = colorAttachments;

    renderPassInfo.subpassCount = (uint32)info.subProcs.size();
    VkSubpassDescription* subpasses 
        = new VkSubpassDescription[info.subProcs.size()];
    for(auto i = 0u; i < info.subProcs.size(); i++)
    {
        subpasses[i] = Transfer(info.subProcs[i]);
    }
    renderPassInfo.pSubpasses = subpasses;

    renderPassInfo.dependencyCount = (uint32)info.dependencies.size();
    VkSubpassDependency* deps = 
        new VkSubpassDependency[info.dependencies.size()];
    for(auto i = 0u; i < info.dependencies.size(); i++)
    {
        deps[i] = *(VkSubpassDependency*)&info.dependencies[i];
    }
    renderPassInfo.pDependencies = deps;

    if (vkCreateRenderPass(
        cgd.GetCGDEntity().device, &renderPassInfo, nullptr, &renderPass
            ) != VK_SUCCESS) 
        {
            Sakura::log::error("failed to create render pass!");
            throw std::runtime_error("failed to create render pass!");
        }

    delete[] colorAttachments;
    delete[] subpasses;
    delete[] deps;
}

RenderPass* CGD_Vk::CreateRenderPass(
    const RenderPassCreateInfo& rpInfo) const
{
    auto res = new RenderPassVk(rpInfo, *this);
    return res;
}