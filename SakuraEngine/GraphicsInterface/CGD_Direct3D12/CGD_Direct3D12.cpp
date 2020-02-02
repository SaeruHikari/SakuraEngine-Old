/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:11
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 16:30:03
 */
#include "CGD_Direct3D12.h"
#include <iostream>

using namespace Sakura::Graphics::Dx12;

namespace Sakura::Graphics
{
    extern std::unique_ptr<CGD> g_CGD;
}

void CGD_Direct3D12::Initialize(void)
{
    g_CGD = std::move(std::make_unique<CGD_Direct3D12>());
}

void CGD_Direct3D12::Render(void)
{
    std::cout << "CGD_Direct3D 12: Render" << std::endl;
}

void CGD_Direct3D12::Destroy(void)
{
    std::cout << "CGD_Direct3D 12: Destroy" << std::endl;
    g_CGD.reset();
}