/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 11:07:17
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 01:41:02
 */
#include "Framework/Application/SEngine.h"
#include "gtest/gtest.h"
#include "SakuraEngine/Core/EngineUtils/SHash.h"

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
    std::string val = "plugin_main";
    XXH64_hash_t _h = xhash(val.data(), val.size(), XXH64_hash_t(12));
    return RUN_ALL_TESTS();
}