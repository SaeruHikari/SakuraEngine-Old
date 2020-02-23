#pragma once
#include "SPA/include/modulemanager.h"

class CGDModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
	virtual void OnLoad() override;
	virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(CGDModule, CGDModule);