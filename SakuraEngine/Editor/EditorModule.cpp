/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:05:07
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 13:23:16
 */
#include "EditorModule.h"
#include "SakuraEngine/Core/Core.h"

void EditorModule::OnLoad()
{
    Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("EditorModule Loaded!");
	return;
}

void EditorModule::OnUnload()
{
    Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("EditorModule Unloaded!");
	return;
}

void EditorModule::MainPluginExec()
{
    Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("Editor Module: Main Plugin Exec!");
    return;
}
