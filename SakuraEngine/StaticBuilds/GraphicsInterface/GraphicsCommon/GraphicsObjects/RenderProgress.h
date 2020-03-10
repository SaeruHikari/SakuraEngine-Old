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
 * @Description: Render Pass Interface of CGD
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-08 22:05:50
 * @LastEditTime: 2020-03-09 12:02:55
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../Flags/Format.h"
#include "../Flags/GraphicsPipelineStates.h"
#include "../Flags/ResourceFlags.h"

namespace Sakura::Graphics
{
    struct AttachmentDescription;
    struct AttachmentReference;
    struct SubpassDescription;
    struct RenderProgressCreateInfo;
}

namespace Sakura::Graphics
{
    SInterface RenderProgress
    {
        virtual ~RenderProgress() = default;
    protected:
        RenderProgress() = default;
    };

    enum AttachmentLoadOp
    {
        AttachmentLoadOpLoad = 0,
        AttachmentLoadOpClear = 1,
        AttachmentLoadOpDontCare = 2
    };

    enum AttachmentStoreOp
    {
        AttachmentStoreOpStore = 0,
        AttachmentStoreOpDontCare = 1
    };

    enum PipelineBindPoint
    {
        BindPointGraphics = 0,
        BindPointCompute = 1,
        BindPointRayTracing = 1000165000
    };

    enum DependencyFlags 
    {
        DependencyByRegion = 0x00000001,
        DependencyDeviceGroup = 0x00000004,
        DependencyViewLocal = 0x00000002
    };
    using DependencyFlag = uint32;

    enum AccessFlags
    {

    };
    using AccessFlag = uint32;

    struct AttachmentDescription
    {
        Format format;
        SampleCountFlag samples = SampleCount_1Bit;
        AttachmentLoadOp loadOp = AttachmentLoadOp::AttachmentLoadOpClear;
        AttachmentStoreOp storeOp = AttachmentStoreOp::AttachmentStoreOpStore;
        AttachmentLoadOp stencilLoadOp 
            = AttachmentLoadOp::AttachmentLoadOpDontCare;
        AttachmentStoreOp stencilStoreOp
            = AttachmentStoreOp::AttachmentStoreOpDontCare;
        ImageLayout initialLayout = ImageLayout::Unknown;
        ImageLayout finalLayout = ImageLayout::PresentSrc;
    };

    struct AttachmentReference
    {
        uint32 attachment = 0;
        ImageLayout layout = ImageLayout::ColorAttachment;
    };

    struct SubprogressDescription
    {
        PipelineBindPoint pipelineBindPoint 
            = PipelineBindPoint::BindPointGraphics;
        std::vector<AttachmentReference> inputAttachments;
        std::vector<AttachmentReference> colorAttachments;
        std::vector<AttachmentReference> resolveAttachments;
        std::vector<AttachmentReference> depthStencilAttachment;
        std::vector<uint32> preserveAttachments;
    };
    
    struct SubProgressDependency
    {
        uint32 srcSubpass;
        uint32 dstSubpass;
        PipelineStageFlag srcStageMask;
        PipelineStageFlag dstStageMask;
        AccessFlag srcAccessMask;
        AccessFlag dstAccessMask;
        DependencyFlag dependencyFlags;
    };
    
    struct RenderProgressCreateInfo
    {
        std::vector<AttachmentDescription> attachments;
        std::vector<SubprogressDescription> subProcs;
        std::vector<SubProgressDependency> dependencies;
    };
}