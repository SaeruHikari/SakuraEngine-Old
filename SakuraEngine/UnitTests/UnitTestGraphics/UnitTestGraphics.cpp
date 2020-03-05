/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 22:07:52
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-05 19:01:28
 */
#include <iostream>
#include "gtest/gtest.h"
#include "StaticBuilds/GraphicsInterface/GraphicsCommon/Format/PixelFormat.h"
#include "StaticBuilds/GraphicsInterface/CGD_Vulkan/Vulkan_FormatTransfer.h"
#include "StaticBuilds/GraphicsInterface/GraphicsCommon/CGD.h"
#include "StaticBuilds/GraphicsInterface/CGD_Vulkan/CommandObjects/CommandContext_Vk.h"

TEST(UnitTestGraphics, FirstPrint)
{
    std::cout << "UnitTestGraphics";
}

TEST(UnitTestGraphics, PixelFormat)
{
    Sakura::Graphics::PixelFormat pf;
    pf = Sakura::Graphics::PixelFormat::R64G64B64A64_UINT;
    //std::cout << "uint Graphics Format: " << Sakura::Graphics::Vk::Transfer(pf) << std::endl;
}

TEST(UnitTestGraphics, CGDInterface)
{

}

TEST(UnitTestGraphics, CommandObjects)
{
    Sakura::Graphics::ContextManager* mng
        = new Sakura::Graphics::Vk::ContextManager_Vk();
    mng->AllocateContext(
        Sakura::Graphics::ECommandType::CommandContext_Graphics,
        true);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}