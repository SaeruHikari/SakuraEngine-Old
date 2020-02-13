/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-12 16:25:00
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:23:07
 */
#include "Win32Window.h"
#include "Win32Engine.h"
#include "Core/EngineUtils/ConsoleDesk.h"
using namespace Sakura;
bool Sakura::SWin32Engine::Initialize(void)
{
	return true;
}

int Sakura::SWin32Engine::Run(void)
{
#ifdef SAKURA_DEBUG_EDITOR
	sout << "SWin32 Engine Run!" << std::endl;
#endif
	return 0;
}

bool Sakura::SWin32Engine::Destroy(void)
{
	return true;
}

std::unique_ptr<Sakura::SWindow>
	Sakura::SEngine::CreateSWindow(const Sakura::WindowDesc& desc)
{
	std::unique_ptr<Sakura::SWindow> result 
		= std::make_unique<Sakura::SWin32Window>(desc);
 	auto engineAsWin32 = force_cast<SWin32Engine*>( ____::eEngine);
	if (desc.bRegisterEx)
	{
		force_cast<SWin32Window*>(result.get())->
			____create(engineAsWin32->hInstance, engineAsWin32->hPrevInstance,
				engineAsWin32->lpCmdLine, engineAsWin32->nCmdShow);
	}
    result->OnCreate();
    return std::move(result);
}

Sakura::SEngine* Sakura::____::eEngine;
void Sakura::SEngine::CreateSEngine(const EngineDesc& _desc)
{
	Sakura::____::eEngine = new Sakura::SWin32Engine(_desc);
}