/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 23:23:02
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-19 14:52:25
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

    IModule* ModuleManager::SpawnStaticModule(const std::pmr::string& name)
    {
        if(ModulesMap.find(name) != ModulesMap.end())
            return ModulesMap[name].get();
        if(InitializeMap.find(name) == InitializeMap.end())
            return nullptr;
        auto func = InitializeMap[name];
        ModulesMap[name] = func();
        ModulesMap[name]->OnLoad();
        std::cout << ModulesMap[name]->GetMetaSize();
        return ModulesMap[name].get();
    }

    IModule* ModuleManager::GetModule(std::string_view name)
    {
        if(ModulesMap.find(name) == ModulesMap.end())
            return nullptr;
        return ModulesMap[name].get();
    }
}

extern "C" DLLEXPORT Sakura::SPA::ModuleManager* GetModuleManager()
{
    static Sakura::SPA::ModuleManager mModuleManager;
    return &mModuleManager;
}