/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-23 22:46:32
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 11:53:29
 */
#include "CoreModule.h"

void CoreModule::OnLoad()
{
	Sakura::log::debug_info
		<Sakura::flags::DEBUG_EDITOR>("CoreModule Loaded!");
	return;
}

void CoreModule::OnUnload()
{
	Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CoreModule Unloaded!");
	return;
}
