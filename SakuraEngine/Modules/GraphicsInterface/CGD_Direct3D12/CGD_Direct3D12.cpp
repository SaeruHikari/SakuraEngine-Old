/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:11
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-04 23:18:02
 */
#include "CGD_Direct3D12.h"
#include "Core/EngineUtils/log.h"

using namespace Sakura::Graphics::Dx12;
using namespace Sakura::Graphics;

void CGD_Direct3D12::Initialize(CGDInfo info, CGDEntity& device)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Initialize");
}

void CGD_Direct3D12::Render(CGDEntity& device)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Render");
}

void CGD_Direct3D12::Destroy(CGDEntity& device)
{
    Sakura::log::debug_info("CGD_Direct3D 12: Destroy");
}