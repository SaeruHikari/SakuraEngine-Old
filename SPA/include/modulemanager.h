/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 18:50:54
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:09:05
 */
#pragma once
#include "../../DependencyGraph/Graph.h"
#include <functional>
#include <memory>
#include <memory_resource>
#include <stdint.h>
#include <iostream>
#include <string>
#include <string_view>
#include "moduleininfo.h"
#include "imodule.h"
#include <unordered_map>
#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#define DLLEXPORT EMSCRIPTEN_KEEPALIVE
#define DLLLOCAL __attribute__((visibility("hidden")))
#elif defined(__GNUC__)
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLLOCAL __attribute__((visibility("hidden")))
#else
#define DLLEXPORT
#define DLLLOCAL
#endif

namespace Sakura::SPA
{
    class ModuleManager;
}

extern "C" DLLEXPORT Sakura::SPA::ModuleManager* GetModuleManager();

namespace Sakura::SPA
{
    using namespace boost;
    struct ModuleProp_t
    {
        using kind = vertex_property_tag; 
    };
    struct ModuleProperty
    {
        ModuleInfo info;
    };
    using registerer = std::function<std::unique_ptr<IModule>(void)>;
    using ModuleProp = property<ModuleProp_t, ModuleProperty>;
    using ModuleGraph = DAG::Graph<ModuleProp>;
    using ModuleNode = DAG::GraphVertex<ModuleProp>;
 
    class ModuleManager
    {
        friend struct IModule;
    public:
        ModuleManager() = default;
        ModuleManager(std::string_view _moduleDir)
            : moduleDir(_moduleDir)
        {
            
        }

        virtual IModule* GetModule(std::string_view name);
        
        template<typename T,
            std::enable_if<
                std::is_constructible<std::string_view, std::remove_reference<T>>::value
            >::type * = nullptr>
        inline IModule* GetModule(T&& name)
        {
            return GetModule(std::string_view(name));
        }

        virtual IModule* SpawnStaticModule(const std::pmr::string& name);
        virtual IModule* SpawnDynamicModule(const std::pmr::string& name);

    public:
        virtual void RegisterStaticallyLinkedModule(
            const std::pmr::string& moduleName, registerer _register);
    private:
        static ModuleManager* mModuleManager;
        std::string_view moduleDir;
        ModuleGraph moduleDependecyGraph;
    private:
        std::pmr::unordered_map<std::pmr::string, registerer> InitializeMap;
        std::pmr::unordered_map<std::string_view, std::unique_ptr<IModule>>
            ModulesMap;
    };

    template<typename ModuleClass>
    struct SStaticallyLinkedModuleRegistrant
    {
        SStaticallyLinkedModuleRegistrant(const char* InModuleName)
        {
            std::function<std::unique_ptr<IModule>(void)> func =
                []()
                {
                    return std::make_unique<ModuleClass>();                     
                };
            GetModuleManager()
                ->RegisterStaticallyLinkedModule(InModuleName, func);
        }
    };

    #define IMPLEMENT_STATIC_MODULE(ModuleImplClass,ModuleName) \
        inline static const Sakura::SPA::SStaticallyLinkedModuleRegistrant\
        <ModuleImplClass> ModuleRegistrant##ModuleName(#ModuleName);

    #define IMPLEMENT_DYNAMIC_MODULE(ModuleImplClass, ModuleName) \
        extern "C" DLLEXPORT Sakura::SPA::IModule* InitializeModule()\
        {\
            return new ModuleImplClass();\
        }
}
