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
 * @Date: 2020-03-30 13:46:34
 * @LastEditTime: 2020-03-31 01:01:27
 */
#pragma once
#include "SakuraEngine/Core/EngineUtils/EngineUtils.h"
#include "../GraphicsInterface/GraphicsCommon/CGD.h"

using namespace Sakura::Graphics;

namespace Sakura::RenderGraph
{
    class SRenderGraph;
}

namespace Sakura::RenderGraph
{
    struct GraphPassBuilder
    {
        template<typename... _SamplerCreateInfos>
        void AddStaticSampler(_SamplerCreateInfos&&... samplers)
        {
            return sigInfo.PushStaticSampler(std::forward<_SamplerCreateInfos>(samplers)...);
        }
        
        template<typename... _SignatureSlots>
        void AddParameter(_SignatureSlots&&... params)
        {
            return sigInfo.PushSignatureSlot(std::forward<_SignatureSlots>(params)...);
        }
        
        template<SignatureSlotType slotType, typename... _ShaderStageFlags>
        static SignatureSlot ShaderParameter(_ShaderStageFlags&&... flags)
        {
            SignatureSlot slot;
            slot.type = slotType;
            slot.stageFlags 
                = Sakura::flags::Or(std::forward<_ShaderStageFlags>(flags)...);
            return slot;
        }
        template<SignatureSlotType slotType>
        static SignatureSlot ShaderParameter(void)
        {
            SignatureSlot slot;
            slot.type = slotType;
            slot.stageFlags = ShaderStageFlags::AllGraphics;
            return slot;
        }

    protected:
        sstring name;
        RootSignatureCreateInfo sigInfo;
        RenderPassCreateInfo rpInfo;
        GraphicsPipelineCreateInfo gpInfo;
        std::function<void(CommandContext* commandContext)> renderCall;
    };
    
    struct SGraphPassNode
    {
        friend class SRenderGraph;
    protected:
        SGraphPassNode(const GraphPassBuilder& builder);

        GraphPassBuilder builder;
        std::shared_ptr<RootSignature> rootSignature;
        std::unique_ptr<RootParameter> rootParameter;
        std::unique_ptr<GraphicsPipeline> graphicsPipeline;
        std::unique_ptr<RenderPass> renderPass;
    };
}