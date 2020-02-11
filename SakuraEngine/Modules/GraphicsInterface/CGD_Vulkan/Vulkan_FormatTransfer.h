/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 12:36:02
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 10:21:22
 */
#pragma once
#include "../GraphicsCommon/Format/PixelFormat.h"

namespace Sakura
{
    namespace Graphics
    {
        namespace Vk
        {
            std::uint32_t Transfer(PixelFormat format)
            {
                return (std::uint32_t)format;
            }   
        }
    }
}
