#include "CGD.h"
#include <iostream>
#include "GraphicsInterface/CGD_Vulkan/CGD_Vulkan.h"

using namespace Sakura::Graphics;

std::unique_ptr<CGD> Sakura::Graphics::g_CGD;

void CGD::Initialize(TargetGraphicsInterface targetGI)
{
    switch (targetGI)
    {
    case (TargetGraphicsInterface::CGD_TARGET_DIRECT3D12):
        std::cout << "Direct3D12 CGD Not Supported now! " << std::endl;
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