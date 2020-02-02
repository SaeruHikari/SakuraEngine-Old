/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:16:38
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 14:04:40
 */
#include "CGD_Vulkan.h"
#include <iostream>

using namespace Sakura::Graphics::Vk;

namespace Sakura::Graphics
{
    extern std::unique_ptr<CGD> g_CGD;
}

void CGD_Vk::Initialize()
{
    g_CGD = std::move(std::make_unique<CGD_Vk>());
}

void CGD_Vk::Render(void)
{
    std::cout << "CGD_Vk: Render" << std::endl;
}

void CGD_Vk::Destroy(void)
{
    std::cout << "CGD_Vk: Destroy" << std::endl;
    g_CGD.reset();
}