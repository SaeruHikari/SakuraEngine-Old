/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:38:56
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 11:04:28
 */
#include "LinuxEngine.h"
#include <iostream>

using namespace Sakura;

bool LinuxEngine::Initialize()
{
    return true;
}

int LinuxEngine::Run()
{
    return 0;
}

bool LinuxEngine::Destroy()
{
    return true;
}

void LinuxEngine::CreateWindow(const WindowDesc& desc)
{
#ifdef SAKURA_DEBUG_EDITOR
    std::cout << std::endl << desc.title << std::endl;
#endif
}