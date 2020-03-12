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
 * @Date: 2020-03-08 23:24:08
 * @LastEditTime: 2020-03-11 22:08:40
 */
#pragma once
#include "../../GraphicsCommon/GraphicsObjects/RenderProgress.h"
#include "../Flags/FormatVk.h"
#include "../Flags/GraphicsPipelineStatesVk.h"
#include "vulkan/vulkan.h"

namespace Sakura::Graphics::Vk
{
    class CGD_Vk;
}

namespace Sakura::Graphics::Vk
{
    inline static VkAttachmentLoadOp Transfer(const AttachmentLoadOp op)
    {
        return VkAttachmentLoadOp(op);
    }
    inline static VkAttachmentStoreOp Transfer(const AttachmentStoreOp op)
    {
        return VkAttachmentStoreOp(op);
    }
    inline static VkImageLayout Transfer(const ImageLayout layout)
    {
        return VkImageLayout(layout);
    }
    inline static VkPipelineBindPoint Transfer(const PipelineBindPoint pt)
    {
        return VkPipelineBindPoint(pt);
    }
    inline static VkAttachmentDescription Transfer(
        const AttachmentDescription& attachment)
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = Transfer(attachment.format);
        colorAttachment.samples = Transfer(attachment.samples);
        colorAttachment.loadOp = Transfer(attachment.loadOp);
        colorAttachment.storeOp = Transfer(attachment.storeOp);;
        colorAttachment.stencilLoadOp = Transfer(attachment.stencilLoadOp);
        colorAttachment.stencilStoreOp = Transfer(attachment.stencilStoreOp);;
        colorAttachment.initialLayout = Transfer(attachment.initialLayout);
        colorAttachment.finalLayout = Transfer(attachment.finalLayout);
        return colorAttachment;
    }

    inline static VkAttachmentReference Transfer(const AttachmentReference& ref)
    {
        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = ref.attachment;
        colorAttachmentRef.layout = Transfer(ref.layout);
        return colorAttachmentRef;
    }

    inline static VkSubpassDescription Transfer(const SubprogressDescription& desc)
    {
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = Transfer(desc.pipelineBindPoint);
        
        subpass.colorAttachmentCount = (uint32)desc.colorAttachments.size();
        subpass.pColorAttachments = 
            desc.colorAttachments.size() == 0 ? nullptr :
            (const VkAttachmentReference*)desc.colorAttachments.data();

        subpass.inputAttachmentCount = (uint32)desc.inputAttachments.size();
        subpass.pInputAttachments = 
            desc.inputAttachments.size() == 0 ? nullptr :
            (const VkAttachmentReference*)desc.inputAttachments.data();

        subpass.pResolveAttachments = 
            desc.resolveAttachments.size() == 0 ? nullptr :
            (const VkAttachmentReference*)desc.resolveAttachments.data();
        subpass.pDepthStencilAttachment = 
            desc.depthStencilAttachment.size() == 0 ? nullptr : 
            (const VkAttachmentReference*)desc.depthStencilAttachment.data();

        subpass.preserveAttachmentCount = (uint32)desc.preserveAttachments.size();
        subpass.pPreserveAttachments = desc.preserveAttachments.size() == 0 ?
            nullptr : desc.preserveAttachments.data();
        return subpass;
    }

    class RenderProgressVk final : SImplements RenderProgress
    {
        friend class CGD_Vk;
        friend class GraphicsPipelineVk;
        friend class CommandContextVk;
    public:
        virtual ~RenderProgressVk();
    protected:
        RenderProgressVk(
            const RenderProgressCreateInfo& info, const CGD_Vk& _cgd);
        VkRenderPass renderPass;
        const CGD_Vk& cgd;
    };
}
