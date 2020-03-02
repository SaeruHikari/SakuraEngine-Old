/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:45
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:36:06
 */
#pragma once
#include "Core.h"
#include "SPA/include/modulemanager.h"

class SAKURA_API CoreModule : public Sakura::SPA::IDynamicModule
{
public:
	#include "meta.h"
	DECLARE_LOGGER("CoreModule")
	virtual void OnLoad() override;
	virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(CoreModule, CoreModule);