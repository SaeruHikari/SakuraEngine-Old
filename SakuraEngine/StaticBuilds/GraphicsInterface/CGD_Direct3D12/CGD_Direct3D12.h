/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:20
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-06 21:46:19
 */
#pragma once
#include "../GraphicsCommon/CGD.h"
#include <iostream>

namespace Sakura::Graphics::Dx12
{
    class CGD_Direct3D12 
    {
    public:
        CGD_Direct3D12() = default;
        static void Initialize(CGDInfo info);
        static void Render();
        static void Destroy();
    };
}