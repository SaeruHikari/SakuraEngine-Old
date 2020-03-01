/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:45
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 23:56:15
 */
#include "CGDModule.h"
#include "Core/EngineUtils/log.h"

void CGDModule::OnLoad()
{
	Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CGDModule Loaded!");
	return;
}

void CGDModule::OnUnload()
{
	Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CGDModule Unloaded!");
	return;
}
