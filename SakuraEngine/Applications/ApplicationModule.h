/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 11:45:27
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 11:50:56
 */
#pragma once
#include "SPA/include/modulemanager.h"

class ApplicationModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
	virtual void OnLoad() override;
	virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(ApplicationModule, ApplicationModule);