/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:24:35
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 16:33:42
 */
#include "CGD.h"
#include <iostream>
#include "GraphicsInterface/CGD_Vulkan/CGD_Vulkan.h"
#include "GraphicsInterface/CGD_Direct3D12/CGD_Direct3D12.h"

using namespace Sakura::Graphics;

std::unique_ptr<CGD> Sakura::Graphics::g_CGD;

void CGD::Initialize(TargetGraphicsInterface targetGI)
{
    //assert()
    switch (targetGI)
    {
    case (TargetGraphicsInterface::CGD_TARGET_DIRECT3D12):
        std::cout << "Direct3D12 CGD Not Supported now! " << std::endl;
        Sakura::Graphics::Dx12::CGD_Direct3D12::Initialize();
        break;
    case (TargetGraphicsInterface::CGD_TARGET_VULKAN):
        std::cout << "Vulkan CGD Initializing: " << std::endl;
        Sakura::Graphics::Vk::CGD_Vk::Initialize();
        break;
    default:
        std::cout << "Target Graphics Interface Not valid!" << std::endl;
        break;
    }
}