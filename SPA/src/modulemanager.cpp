/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 23:23:02
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:58:34
 */
#include "../include/modulemanager.h"
#include "../include/confinfo.h"

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
        return ModulesMap[name].get();
    }

    IModule* ModuleManager::SpawnDynamicModule(const std::pmr::string& name)
    {
        std::unique_ptr<SharedLibrary> sharedLib = std::make_unique<SharedLibrary>();
        #ifdef CONFINFO_PLATFORM_LINUX
        sharedLib->load("../lib/lib"+name);
        #elif CONFINFO_PLATFORM_WIN32
        sharedLib.load("../lib/"+name);
        #endif
        if(sharedLib->hasSymbol("InitializeModule"))
        {
            auto func = sharedLib->get<IModule*()>("InitializeModule");
            ModulesMap[name] = std::move(std::unique_ptr<IModule>(func()));
            IDynamicModule* module = (IDynamicModule*)ModulesMap[name].get();
            module->sharedLib = std::move(sharedLib);
            module->OnLoad();
            return module;
        }
        return nullptr;
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