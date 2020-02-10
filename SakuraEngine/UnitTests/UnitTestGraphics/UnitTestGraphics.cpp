/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 22:07:52
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 01:13:14
 */
#include <iostream>
#include "gtest/gtest.h"
#include "GraphicsInterface/GraphicsCommon/Format/PixelFormat.h"
#include "GraphicsInterface/CGD_Vulkan/Vulkan_FormatTransfer.h"
#include "GraphicsInterface/GraphicsCommon/CGD.h"

TEST(UnitTestGraphics, FirstPrint)
{
    std::cout << "UnitTestGraphics";
}

TEST(UnitTestGraphics, PixelFormat)
{
    Sakura::Graphics::PixelFormat pf;
    pf = Sakura::Graphics::PixelFormat::R64G64B64A64_UINT;
    std::cout << "uint Graphics Format: " << Sakura::Graphics::Vk::Transfer(pf) << std::endl;
}

TEST(UnitTestGraphics, CGDInterface)
{
    Sakura::Graphics::CGD::Initialize(
        Sakura::Graphics::CGD::TargetGraphicsInterface::CGD_TARGET_VULKAN
    );
    Sakura::____::eCGD->Render();
    Sakura::____::eCGD->Destroy();

    Sakura::Graphics::CGD::Initialize(
        Sakura::Graphics::CGD::TargetGraphicsInterface::CGD_TARGET_DIRECT3D12
    );
    Sakura::____::eCGD->Render();
    Sakura::____::eCGD->Destroy();
}
