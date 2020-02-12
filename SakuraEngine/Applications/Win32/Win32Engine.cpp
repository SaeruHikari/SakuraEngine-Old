#include "Win32Window.h"
#include "Win32Engine.h"
#include "Core/EngineUtils/ConsoleDesk.h"

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

Sakura::SEngine* Sakura::____::eEngine;
void Sakura::SEngine::CreateEngine(const EngineDesc& _desc)
{
	Sakura::____::eEngine = new Sakura::SWin32Engine(_desc);
}