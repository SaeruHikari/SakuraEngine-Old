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
 * @Date: 2020-03-13 16:46:06
 * @LastEditTime: 2020-03-14 01:31:52
 */
#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "SakuraEngine/Core/Math/Math.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include <vector>
#include <algorithm>

using namespace Sakura::Math;

namespace Sakura::Images
{
    struct Image 
    {
        Image(std::byte* _data)
        {
            data = std::unique_ptr<std::byte>(_data);
        }
        enum PixelChannel
        {
            X = 0, R = 0,
            Y = 1, G = 1,
            Z = 2, B = 2,
            W = 3, A = 3 
        };
        template<typename channelFormat = Sakura::Math::Unorm>
        channelFormat ViewIntoPixel(const int channelNum,
            const int X, const int Y, const PixelChannel channel = 0) const
        {
            channelFormat* fullView = ((channelFormat*)data.get());
            return *(fullView + channelNum * (X + Y * X) + channel);
        }
        std::byte* GetData() const
        {
            return data.get();
        }
    private:
        std::unique_ptr<std::byte> data;
    };

    template<typename channelStride = Sakura::Math::Unorm>
    channelStride* LoadFromDisk(const char* path, int& width, int& height, int& n)
    {
        channelStride* idata = 
            (channelStride*)stbi_load(path, &width, &height, &n, 0);
        if(n < 0)
            Sakura::log::error("ImageLoader: no texture of this name found!");
        return idata;
    }

    void WirtePNGToDisk(char const *path, int x, int y,
        int comp, const void *data, int stride_bytes)
    {
        stbi_write_png(path, x, y, comp, data, stride_bytes);
    }

    Image LoadFromDisk(const char* path, int& width, int& height, int& n)
    {
        auto data = LoadFromDisk<std::byte>(path, width, height, n);
        return Image(data);
    }

    template<typename channelStride = Sakura::Math::Unorm>
    auto ViewIntoPixel(const channelStride* data,
        const int channelNum, const int X, const int Y)
    {
        return data + channelNum * (X + Y * X);
    }
}