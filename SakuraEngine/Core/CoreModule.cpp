/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-23 22:46:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-20 10:46:11
 */
#define API_EXPORTS
#include "CoreModule.h"

void CoreModule::OnLoad()
{
	CoreModule::debug_info
		<Sakura::flags::DEBUG_EDITOR>("CoreModule Loaded!");
	return;
}

void CoreModule::OnUnload()
{
	CoreModule::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CoreModule Unloaded!");
	return;
}
