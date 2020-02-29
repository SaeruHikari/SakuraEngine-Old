/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:24:35
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 17:34:53
 */
#define API_EXPORTS
#include "../GraphicsCommon/CGD.h"
#include <iostream>
#include "../CGD_Vulkan/CGD_Vulkan.h"
#include "CGDExport.h"
#include "Core/EngineUtils/log.h"
#include "../CGD_Direct3D12/CGD_Direct3D12.h"


using namespace Sakura::Graphics;

Sakura::Graphics::CGD* CGD::eCGD = nullptr;

void CGD::Initialize(TargetGraphicsInterface targetGI, CGD_Info info)
{
    switch (targetGI)
    {
    case (TargetGraphicsInterface::CGD_TARGET_DIRECT3D12):
        Sakura::log::debug_warn("Direct3D12 CGD Not Supported now! ");
        Sakura::Graphics::Dx12::CGD_Direct3D12::Initialize();
        break;
    case (TargetGraphicsInterface::CGD_TARGET_VULKAN):
        Sakura::log::debug_info("Vulkan CGD Initializing: ");
        Sakura::Graphics::Vk::CGD_Vk::Initialize(info);
        break;
    default:
        Sakura::log::debug_error("Target Graphics Interface Not valid!");
        break;
    }
}

extern "C" SAKURA_API Sakura::Graphics::CGD * __stdcall GetCGD(void)
{
    return Sakura::Graphics::CGD::GetCGD();
}