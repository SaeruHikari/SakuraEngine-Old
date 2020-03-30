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
 * @Date: 2020-03-28 20:31:11
 * @LastEditTime: 2020-03-30 21:21:00
 */
#pragma once
#include "../GraphicsInterface/GraphicsCommon/CGD.h"
#include "GraphPassNode.h"
#include "GraphResourceNode.h"

using namespace Sakura::Graphics;
using namespace Sakura::hash;

namespace Sakura::RenderGraph
{
    class SRenderGraph
    {
        DECLARE_LOGGER("RenderGraph")
    public:
        SRenderGraph(CGD* commanGraphicsDevice);
        
		[[nodiscard]] SGraphBufferNode* AllocateRenderGraphBuffer(
            const sstring& name, const BufferCreateInfo&);
		[[nodiscard]] SGraphBufferNode* AllocateRenderGraphBuffer(
			const sstring& name, const uint64 size,
			BufferUsages bufferUsages, CPUAccessFlags cpuAccess);

        [[nodiscard]] SGraphTextureNode* AllocateRenderGraphTexture(
            const sstring& name, const TextureCreateInfo&);
		[[nodiscard]] SGraphTextureNode* AllocateRenderGraphTexture(
			const sstring& name, const Format format,
			const uint32 width, const uint32 height,
			ImageUsages imageUsages, uint32 mipLevels = 1);
    private:
        Sakura::SUnorderedMap<sstring, std::unique_ptr<SGraphTextureNode>> texturePool;
        Sakura::SUnorderedMap<sstring, std::unique_ptr<SGraphBufferNode>> bufferPool;
        CGD* cgd = nullptr;
    };
}