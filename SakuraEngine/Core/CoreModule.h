#pragma once
#include "Core.h"
#include "SPA/include/modulemanager.h"

class CoreModule : public Sakura::SPA::IStaticModule
{
	#include "meta.h"
	virtual void OnLoad() override;
	virtual void OnUnload() override;
};
IMPLEMENT_STATIC_MODULE(CoreModule, CoreModule);