/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:11
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 12:49:43
 */
#include "CGD_Direct3D12.h"
#include "Core/EngineUtils/log.h"


using namespace Sakura::Graphics::Dx12;

void CGD_Direct3D12::Initialize(void)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Initialize");
    eCGD = new CGD_Direct3D12();
}

void CGD_Direct3D12::Render(void)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Render");
}

void CGD_Direct3D12::Destroy(void)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Destroy");
    delete eCGD;
}