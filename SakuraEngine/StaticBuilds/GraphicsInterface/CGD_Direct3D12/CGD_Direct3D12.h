/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:20
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-26 17:49:07
 */
#pragma once
#include "../GraphicsCommon/CGD.h"
#include <iostream>

namespace Sakura::Graphics::D3D12
{
    struct CGDEntityD3D12
    {

    };
    
    class CGDD3D12 final : public CGD
    {
        DECLARE_LOGGER("CGD_D3D12")
        friend struct SwapChainD3D12;
    public:

    };
}