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
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-23 22:38:04
 */
#define API_EXPORTS     
#include "../GraphicsCommon/CGD.h" 
#include "../GraphicsCommon/ResourceObjects/Resource.h"
#include "../GraphicsCommon/ResourceObjects/ResourceViews.h"
#include "Core/EngineUtils/log.h" 
  
using namespace Sakura::Graphics;  

GpuBuffer* CGD::CreateUploadBuffer(const std::size_t bufferSize) const
{
    BufferCreateInfo bufferInfo = {};
    bufferInfo.usage = BufferUsage::TransferSrcBuffer;
    bufferInfo.cpuAccess = CPUAccessFlags::ReadWrite;
    bufferInfo.size = bufferSize;
    return (GpuBuffer*)CreateGpuResource(bufferInfo);
}

ResourceView* CGD::ViewIntoTexture(const GpuTexture& tex, const Format format,
    const ImageAspectFlags aspect,
    uint32 mipLevels, uint32 baseMip, uint32 layerCount, uint32 baseArrayLayer) const
{
    ResourceViewCreateInfo tvinfo = {};
    tvinfo.viewType = ResourceViewType::ImageView2D;
    tvinfo.format = format;
    tvinfo.view.texture2D.baseMipLevel = baseMip;
    tvinfo.view.texture2D.mipLevels = mipLevels;
    tvinfo.view.texture2D.baseArrayLayer = baseArrayLayer;
    tvinfo.view.texture2D.layerCount = layerCount;
    tvinfo.view.texture2D.aspectMask = aspect;
    return ViewIntoResource(tex, tvinfo);
}

GpuTexture* CGD::CreateGpuTexture(const Format format,
    const uint32 width, const uint32 height,
    ImageUsages imageUsages, uint32 mipLevels) const
{
    TextureCreateInfo imgInfo = {};
    imgInfo.width = width;
    imgInfo.height = height;
    imgInfo.format = format;
    imgInfo.arrayLayers = 1;
    imgInfo.depth = 1;
    imgInfo.mipLevels = mipLevels;
    imgInfo.usage = imageUsages;
    return (GpuTexture*)CreateGpuResource(imgInfo);
}

GpuBuffer* CGD::CreateGpuBuffer(const uint64 size, 
    BufferUsages bufferUsages, CPUAccessFlags cpuAccess) const
{
    BufferCreateInfo bufferInfo = {};
    bufferInfo.usage = bufferUsages;
    bufferInfo.cpuAccess = cpuAccess;
    bufferInfo.size = size;
    return (GpuBuffer*)CreateGpuResource(bufferInfo);
}