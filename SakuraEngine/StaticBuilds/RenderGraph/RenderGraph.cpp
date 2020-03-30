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
 * @Date: 2020-03-28 20:30:27
 * @LastEditTime: 2020-03-30 20:21:56
 */
#include "RenderGraph.h"
#include "GraphResourceNode.h"
#include "GraphPassNode.h"

using namespace Sakura::Graphics;

namespace Sakura::RenderGraph
{
	Sakura::StringHasher rghasher;
	SRenderGraph::SRenderGraph(CGD* commanGraphicsDevice)
		:cgd(commanGraphicsDevice)
	{

	}

	inline SGraphBufferNode* SRenderGraph::AllocateRenderGraphBuffer(
        const sstring& name, const BufferCreateInfo& bufInfo)
	{
        assert(cgd != nullptr);
        if (bufferPool.find(name) != bufferPool.end())
            SRenderGraph::warn(
                "AllocateRGBuffer: Rename & Allocation of an existed buffer node!");
        bufferPool[name].reset(new SGraphBufferNode(
				cgd->CreateGpuResource(bufInfo), name));
		return bufferPool[name].get();
	}

	SGraphBufferNode* SRenderGraph::AllocateRenderGraphBuffer(const sstring& name,
        const uint64 size, BufferUsages bufferUsages, CPUAccessFlags cpuAccess)
	{
		assert(cgd != nullptr);
		if (bufferPool.find(name) != bufferPool.end())
			SRenderGraph::warn(
				"AllocateRGBuffer: Rename & Allocation of an existed buffer node!");
		bufferPool[name].reset(new SGraphBufferNode(
				cgd->CreateGpuBuffer(size, bufferUsages, cpuAccess), name));
		return bufferPool[name].get();
	}

	inline SGraphTextureNode* SRenderGraph::AllocateRenderGraphTexture(
        const sstring& name, const TextureCreateInfo& texInfo)
	{
        assert(cgd != nullptr);
		if (texturePool.find(name) != texturePool.end())
			SRenderGraph::warn(
                "AllocateRGTexture: Rename & Allocation of an existed texture node!");
        texturePool[name].reset(new SGraphTextureNode(
			cgd->CreateGpuResource(texInfo), name));
		return texturePool[name].get();
	}

	SGraphTextureNode* SRenderGraph::AllocateRenderGraphTexture(const sstring& name,
        const Format format, const uint32 width, const uint32 height,
        ImageUsages imageUsages, uint32 mipLevels /*= 1*/)
	{
		assert(cgd != nullptr);
		if (texturePool.find(name) != texturePool.end())
			SRenderGraph::warn(
				"AllocateRGTexture: Rename & Allocation of an existed texture node!");

		texturePool[name].reset(new SGraphTextureNode(
            cgd->CreateGpuTexture(format, width, height, imageUsages, mipLevels), name));
        return texturePool[name].get();
	}

}

