/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:45
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:40:56
 */
#pragma once
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "SPA/include/modulemanager.h"

class CGDModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
	DECLARE_LOGGER("CGDModule")
	virtual void OnLoad() override;
	virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(CGDModule, CGDModule);