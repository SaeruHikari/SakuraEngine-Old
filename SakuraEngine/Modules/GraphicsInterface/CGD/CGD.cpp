/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-02 14:58:45
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
    static auto logger = Sakura::log::regist_logger("Graphics");
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