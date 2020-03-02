/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 11:46:54
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:40:18
 */
#include "ApplicationModule.h"
#include "SakuraEngine/Core/Core.h"

void ApplicationModule::OnLoad()
{
    ApplicationModule::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("ApplicationModule Loaded!");
	return;
}

void ApplicationModule::OnUnload()
{
    ApplicationModule::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("ApplicationModule Unloaded!");
	return;
}
