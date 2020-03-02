/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:07:06
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:38:48
 */
#pragma once
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "SPA/include/modulemanager.h"

class EditorModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
	DECLARE_LOGGER("EditorModule")
	virtual void OnLoad() override;
	virtual void OnUnload() override;
	virtual void MainPluginExec() override;
};
IMPLEMENT_DYNAMIC_MODULE(EditorModule, EditorModule);