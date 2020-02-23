/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:16:38
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 00:19:31
 */
#include "CGD_Vulkan.h"
#include "Core/EngineUtils/ConsoleDesk.h"

using namespace Sakura::Graphics::Vk;

void CGD_Vk::Initialize(void)
{
    eCGD = new CGD_Vk();    
}

void CGD_Vk::Render(void)
{
    sout << "CGD_Vk: Render" << std::endl;
}

void CGD_Vk::Destroy(void)
{
    sout << "CGD_Vk: Destroy" << std::endl;
    delete eCGD;
}