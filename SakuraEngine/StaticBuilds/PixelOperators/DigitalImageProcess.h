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
 * @Date: 2020-03-14 01:30:54
 * @LastEditTime: 2020-03-17 16:11:19
 */
#pragma once
#include "ImageUtils.h"
#include <map>

namespace Sakura::Images
{
    template<typename channelStride>
    void RGB2HSI(channelStride* _src, int pixels = 1, int gap = 4,
        int from = 0, int to = -1)
    {
        for(auto it = from; it < ((to == -1) ? pixels : to); it++)
        {   
            auto src = _src + gap * it;
            double r = *src;
            double g = *(src + 1);
            double b = *(src + 2);
            
            double rn = r / (r + g + b);
            double gn = g / (r + g + b);
            double bn = b / (r + g + b);

            auto h = (src + 0);
            auto s = (src + 1);
            auto i = (src + 2);
            *i = (channelStride)((r + g + b) / 3.f);
            r /= 255.f;
            g /= 255.f;
            b /= 255.f;

            double _h = std::acos((0.5 * ((rn - gn) + (rn - bn))) /
                (std::sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
            if(b > g)
            {
                _h = 2.f * PI - _h;	
            }
            *h = (channelStride)(255.f * _h / (2.f * PI)); 
            
            *s = (channelStride)((1 - 3 * min(rn, gn, bn)) * 255.f);
        }
    }
    
    template<typename channelStride>
    void HSI2RGB(channelStride* _src, int pixels = 1,
        int gap = 4, int from = 0, int to = -1)
    {
        for(auto it = from; it < ((to == -1) ? pixels : to); it++)
        {   
            auto src = _src + gap * it;
            float h = *src;
            float s = *(src + 1);
            float i = *(src + 2);
            h /= 255.f;
            h = 2.f * PI * h;
            s /= 255.f;
            i /= 255.f;
            
            double x = i * (1 - s);
            auto r = (src + 0);
            auto g = (src + 1);
            auto b = (src + 2);
            // For black, white and grayscale h is NaN. Conversion works incorrectly.
            if(std::isnan(h))
            {
                *(src + 0) = (channelStride)(i * 255.f);
                *(src + 1) = (channelStride)(i * 255.f);
                *(src + 2) = (channelStride)(i * 255.f);
            }
            else if(h < 2 * PI / 3)
            {
                double y = i * (1 + (s * cos(h)) / (cos(PI / 3 - h)));
                double z = 3 * i - (x + y);
                *b = (channelStride)(x * 255.f);
                *r = (channelStride)(y * 255.f);
                *g = (channelStride)(z * 255.f);
            }
            else if(h < 4 * PI / 3)
            {
                double y = i * (1 + (s * cos(h - 2 * PI / 3)) / (cos(PI / 3 - (h  - 2 * PI / 3))));
                double z = 3 * i - (x + y);
                *r = (channelStride)(x * 255.f);
                *g = (channelStride)(y * 255.f);
                *b = (channelStride)(z * 255.f);
            }
            else
            {
                double y = i * (1 + (s * cos(h - 4 * PI / 3)) / (cos(PI / 3 - (h  - 4 * PI / 3))));
                double z = 3 * i - (x + y);
                *r = (channelStride)(z * 255.f);
                *g = (channelStride)(x * 255.f);
                *b = (channelStride)(y * 255.f);
            }
        }
    }

    template<bool bOverride = true, typename channelStride = Sakura::Math::Unorm>
    auto HistNormolize(channelStride* mat, int width, int height, int gap = 4,
        bool hsi = true)
    {
        std::map<channelStride, std::size_t, std::less<>> hist;
        std::map<channelStride, float, std::less<>> pc;
        std::map<channelStride, float, std::less<>> dstpc;
        std::map<channelStride, float, std::less<>> pct;
        std::map<channelStride, channelStride, std::less<>> mapper;
        // 统计频数
        for (auto i = 0; i < height; i++) 
            for (auto j = 0; j < width; j++)
            {
                int IOffset = (gap == 1) ? 0 : 2;
                channelStride GrayIndex = mat[(i * width + j) * gap + IOffset];
                hist[GrayIndex] = 
                    hist.find(GrayIndex) == hist.end() 
                    ? 1 : hist[GrayIndex] + 1;
            }
        // 计算频率
        for (auto& iter : hist)
            pct[iter.first] = (float)iter.second / (float)(width * height);
        pc = pct;
        float last = 0;
        // 计算加权
        for (auto& iter : pct)
        {
            auto rate = iter.second;
            pct[iter.first] += last;
            last += rate;
        }
        // 均衡化
        for (auto&& iter : pct)
            mapper[iter.first] = (channelStride)(255.0 * iter.second + 0.5);
        
        // 将结果覆盖到目标源
        if constexpr (bOverride)
        {
            if(hsi)
            {
                for (auto i = 0; i < height; i++) 
                    for (auto j = 0; j < width; j++)
                    {
                        int IOffset = (gap == 1) ? 0 : 2;
                        auto srcval = mat[(i * width + j) * gap + IOffset];
                        *(mat + (i * width + j) * gap + IOffset) = mapper[srcval];
                    }
            }
            else
                for (auto i = 0; i < height; i++) 
                    for (auto j = 0; j < width; j++)
                    {
                        auto srcval = mat[(i * width + j) * gap];
                        for(auto w = 0; w < gap; w++)
                            *(mat + (i * width + j) * gap + w) 
                                = (w==3 ? 255 : mapper[srcval]);
                    }
        }
        for(auto& iter : pc)
        {
            auto dstVal = mapper[iter.first];
            if(dstpc.find(dstVal) == dstpc.end())
                dstpc[dstVal] = iter.second;
            else 
                dstpc[dstVal] += iter.second;
        }
        return std::tuple(hist, pc, dstpc, mapper);
    } 

    template<bool bOverride = true, typename channelStride = Sakura::Math::Unorm>
    auto HistSpecify(channelStride* mat, 
        std::map<channelStride, float, std::less<>> dstPct,
        int width, int height, int gap = 4, bool hsi = true)
    {
        auto mappers = HistNormolize(mat, width, height, gap, hsi);
        std::map<channelStride, channelStride> Lut;
        // 遍历均衡化后src的灰度频率表, 按频率映射到目标LUT上
        auto srcPct = std::get<2>(mappers);
        for(auto& iters : srcPct)
        {
            float sub = 9999999;
            channelStride dst = iters.first;
            for(auto& iterd : dstPct)
            {
                auto temp = std::abs(iterd.second - iters.second);
                if(temp < sub)
                {
                    sub = temp;
                    dst = iterd.first;
                }
            }
            // 记录Lut
            Lut[iters.first] = dst; 
        }
        // 将结果覆盖到目标源
        if constexpr (bOverride)
        {
            if(hsi)
            {
                for (auto i = 0; i < height; i++) 
                    for (auto j = 0; j < width; j++)
                    {
                        int IOffset = (gap == 1) ? 0 : 2;
                        auto srcval = mat[(i * width + j) * gap + IOffset];
                        *(mat + (i * width + j) * gap + IOffset) 
                            = Lut[srcval];
                    }
            }
            else
                for (auto i = 0; i < height; i++) 
                    for (auto j = 0; j < width; j++)
                    {
                        auto srcval = mat[(i * width + j) * gap];
                        for(auto w = 0; w < gap; w++)
                            *(mat + (i * width + j) * gap + w) =
                                (w==3 ? 255 : Lut[srcval]);
                    }
        }
        std::map<channelStride, std::size_t, std::less<>> hist;
        std::map<channelStride, float, std::less<>> pct;
        // 统计频数
        for (auto i = 0; i < height; i++) 
            for (auto j = 0; j < width; j++)
            {
                int IOffset = (gap == 1) ? 0 : 2;
                channelStride GrayIndex = mat[(i * width + j) * gap + IOffset];
                hist[GrayIndex] = 
                    hist.find(GrayIndex) == hist.end() 
                    ? 1 : hist[GrayIndex] + 1;
            }
        // 计算频率
        for (auto& iter : hist)
            pct[iter.first] = (float)iter.second / (float)(width * height);

        return pct;
    }
}