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
 * @Date: 2020-03-30 11:29:58
 * @LastEditTime: 2020-03-30 21:40:54
 */
#pragma once
#include "SakuraEngine/Core/EngineUtils/EngineUtils.h"
#include "../GraphicsInterface/GraphicsCommon/CGD.h"

using namespace Sakura::Graphics;
using namespace Sakura::hash;

namespace Sakura::RenderGraph
{
    class SRenderGraph;
}

namespace Sakura::RenderGraph
{
	/**
	 * @description: A node that holds graph resource reference.
	 * @author: SaeruHikari
	 */
	struct SGraphResourceNode
	{
        virtual ~SGraphResourceNode() = default;
        inline GpuResource* GetResource(void) const
        {
            return resource.get();
        }
    protected:
		SGraphResourceNode(GpuResource* _resource, sstring _name);
        sstring name;
        std::unique_ptr<GpuResource> resource;
	};

    struct SGraphBufferNode final : public SGraphResourceNode
    {
        friend class SRenderGraph;
    protected:
        SGraphBufferNode(GpuBuffer* buf, sstring _name);
    };

    struct SGraphTextureNode final : public SGraphResourceNode
    {
        friend class SRenderGraph;
    protected:
        SGraphTextureNode(GpuTexture* tex, sstring _name);
    };
}