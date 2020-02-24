/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:38:56
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 11:39:01
 */
#define API_EXPORTS
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

SAKURA_API std::unique_ptr<SWindow>
    SEngine::CreateSWindow(const WindowDesc& desc)
{
    std::unique_ptr<SWindow> result = std::make_unique<SLinuxWindow>(desc);
    result->OnCreate();
    return std::move(result);
}

Sakura::SEngine* Sakura::____::eEngine;
SAKURA_API void SEngine::CreateSEngine(const EngineDesc& _desc)
{
    Sakura::____::eEngine = new Sakura::SLinuxEngine(_desc);
}

SAKURA_API Sakura::EngineDesc Sakura::SEngine::GetEngineDesc(void)
{
	if(____::eEngine == nullptr)
		assert(0 && "Error! Engine not Created!");
	else
	{
		return ____::eEngine->desc;                
	}
	return EngineDesc();
}

SAKURA_API Sakura::SEngine* Sakura::SEngine::GetEngine(void)
{
	if(____::eEngine == nullptr)
		assert(0 && "Error! Engine not Created!");
	else
	{
		return ____::eEngine;
	}
	return nullptr;
}