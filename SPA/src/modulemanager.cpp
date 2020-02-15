/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 23:23:02
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-15 18:59:09
 */
#include "../include/modulemanager.h"

namespace Sakura::SPA
{
    void ModuleManager::RegisterStaticallyLinkedModule(
        const std::pmr::string& moduleName, registerer _register)
    {
        if(InitializeMap.find(moduleName) != InitializeMap.end())
        {
            return;
        }
        InitializeMap[moduleName] = _register;
    }

    IModule* ModuleManager::GetModule(std::string_view name)
    {
        if(ModulesMap.find(name) == ModulesMap.end())
            return nullptr;
        return ModulesMap[name].get();
    }
}