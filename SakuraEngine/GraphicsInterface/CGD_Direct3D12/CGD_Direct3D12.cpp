/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:23:11
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 00:19:01
 */
#include "CGD_Direct3D12.h"
#include "Core/EngineUtils/ConsoleDesk.h"

using namespace Sakura::Graphics::Dx12;

void CGD_Direct3D12::Initialize(void)
{
    Sakura::____::eCGD = new CGD_Direct3D12();
}

void CGD_Direct3D12::Render(void)
{
    sout << "CGD_Direct3D 12: Render" << std::endl;
}

void CGD_Direct3D12::Destroy(void)
{
    sout << "CGD_Direct3D 12: Destroy" << std::endl;
    delete Sakura::____::eCGD;
}