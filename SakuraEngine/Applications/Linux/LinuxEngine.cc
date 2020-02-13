/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:38:56
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 16:29:13
 */
#include <iostream>
#include "LinuxEngine.h"
#include "LinuxWindow.h"

using namespace Sakura;

bool SLinuxEngine::Initialize()
{
    return true;
}

int SLinuxEngine::Run()
{
    return 0;
}

bool SLinuxEngine::Destroy()
{
    return true;
}

std::unique_ptr<SWindow>
    SEngine::CreateSWindow(const WindowDesc& desc)
{
    std::unique_ptr<SWindow> result = std::make_unique<SLinuxWindow>(desc);
    result->OnCreate();
    return std::move(result);
}

Sakura::SEngine* Sakura::____::eEngine;
void SEngine::CreateSEngine(const EngineDesc& _desc)
{
    Sakura::____::eEngine = new Sakura::SLinuxEngine(_desc);
}