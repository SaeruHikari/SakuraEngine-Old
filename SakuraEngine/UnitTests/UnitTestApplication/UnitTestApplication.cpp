/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 11:07:17
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-27 13:59:41
 */
#include "Framework/Application/SEngine.h"
#include "gtest/gtest.h"

TEST(UnitTestApplication, LinuxApplication)
{
    Sakura::EngineDesc desc;
    Sakura::SEngine::CreateSEngine(desc);
    Sakura::WindowDesc windDesc;
    Sakura::SEngine::CreateSWindow(windDesc);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}