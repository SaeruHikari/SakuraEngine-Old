/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:07:06
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 13:22:23
 */
#pragma once
#include "SPA/include/modulemanager.h"

class EditorModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
	virtual void OnLoad() override;
	virtual void OnUnload() override;
	virtual void MainPluginExec() override;
};
IMPLEMENT_DYNAMIC_MODULE(EditorModule, EditorModule);