/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 11:07:17
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 11:10:45
 */
#include "Applications/Linux/LinuxEngine.h"
#include "gtest/gtest.h"

TEST(UnitTestApplication, LinuxApplication)
{
    auto engine = new Sakura::LinuxEngine();
    engine->Initialize();
    Sakura::WindowDesc desc;
    engine->CreateWindow(desc);
    engine->Run();
    engine->Destroy();
}