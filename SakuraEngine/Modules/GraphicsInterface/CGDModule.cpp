/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:45
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:41:12
 */
#include "CGDModule.h"
#include "Core/EngineUtils/log.h"

void CGDModule::OnLoad()
{
	CGDModule::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CGDModule Loaded!");
	return;
}

void CGDModule::OnUnload()
{
	CGDModule::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("CGDModule Unloaded!");
	return;
}
