/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 23:23:02
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 16:41:06
 */
#define API_EXPORTS
#include "../include/modulemanager.h"
#include "../include/confinfo.h"
#include "../../Extern/include/json/json.hpp"

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
        ModulesMap[name]->_information
            = ParseMetaData(ModulesMap[name]->GetMetaData());
        // Delay onload call to initialize time(with dependency graph)
        //ModulesMap[name]->OnLoad();
        return ModulesMap[name].get();
    }

    IModule* ModuleManager::SpawnDynamicModule(const std::pmr::string& name)
    {
        std::unique_ptr<SharedLibrary> sharedLib 
            = std::make_unique<SharedLibrary>();
        std::pmr::string initName = "InitializeModule" + name;
        #ifdef CONFINFO_PLATFORM_LINUX
        sharedLib->load("../lib/lib"+name);
        #elif defined(CONFINFO_PLATFORM_WIN32)
        sharedLib->load("../bin/"+name);
        #endif
        if(sharedLib->hasSymbol(initName.c_str()))
        {
            auto func = 
                sharedLib->get<IModule*()>(initName);
            ModulesMap[name] = std::move(std::unique_ptr<IModule>(func()));
            IDynamicModule* module = (IDynamicModule*)ModulesMap[name].get();
            module->sharedLib = std::move(sharedLib);
            module->_information = ParseMetaData(module->GetMetaData());
            // Delay onload call to initialize time(with dependency graph)
            //module->OnLoad();
            return module;
        }
        return nullptr;
    }

    ModuleInfo ModuleManager::ParseMetaData(const char* metadata)
    {
        try
        {
            using json = nlohmann::json;
            json tree = json::parse(metadata);
            if(Version(tree.at("api").get<std::string>()).compatible(CoreVersion))
            {
                ModuleInfo info;
                info.name = tree.at("name").get<std::string>();
                info.prettyname = tree.at("prettyname").get<std::string>();
                info.engine_version = tree.at("api").get<std::string>();
                info.url = tree.at("url").get<std::string>();
                info.copyright = tree.at("copyright").get<std::string>();
                info.license = tree.at("license").get<std::string>();
                info.version = tree.at("version").get<std::string>();
                json jsonDependecy = tree.at("dependencies");
                for(json& jdep : jsonDependecy)
                {
                    Dependency dep;
                    dep.name = jdep.at("name").get<std::string>();
                    dep.version = jdep.at("version").get<std::string>();
                    info.dependencies.push_back(dep);
                }
                return info;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return ModuleInfo();
    }

    IModule* ModuleManager::GetModule(std::string_view name)
    {
        if(ModulesMap.find(name) == ModulesMap.end())
            return nullptr;
        return ModulesMap[name].get();
    }
}

SPA_API Sakura::SPA::ModuleManager* __stdcall GetModuleManager()
{
    static Sakura::SPA::ModuleManager mModuleManager;
    return &mModuleManager;
}