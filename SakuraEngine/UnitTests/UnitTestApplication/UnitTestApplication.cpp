/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 11:07:17
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 19:39:42
 */
#include "Applications/Linux/LinuxEngine.h"
#include "gtest/gtest.h"

TEST(UnitTestApplication, LinuxApplication)
{
    Sakura::EngineDesc desc;
    Sakura::SEngine::CreateEngine(desc);
    Sakura::WindowDesc windDesc;
    Sakura::SEngine::CreateWindow(windDesc);
}