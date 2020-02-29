/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 18:50:54
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-27 13:41:29
 */
#pragma once
#include "../../Extern/include/version/version.h"
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
#define SPA_API EMSCRIPTEN_KEEPALIVE
#define DLLLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#elif defined(__GNUC__)
#define SPA_API __attribute__((visibility("default")))
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#else
#define DLLEXPORT __declspec(dllexport)
#ifdef API_EXPORTS
#define SPA_API __declspec(dllexport)
#else
#define SPA_API __declspec(dllimport)
#endif
#endif

namespace Sakura::SPA
{
    class ModuleManager;
}

extern "C" SPA_API Sakura::SPA::ModuleManager* __stdcall GetModuleManager();

namespace Sakura::SPA
{
    using namespace boost;
    struct ModuleProp_t
    {
        using kind = vertex_property_tag; 
    };
    struct ModuleProperty
    {
        bool bActive = false;
        std::string_view name;
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
        virtual IModule* GetModule(std::string_view name);
        
        template<typename T,
            typename std::enable_if<
                std::is_constructible<std::string_view, std::remove_reference<T>>::value
            >::type * = nullptr>
        inline IModule* GetModule(T&& name)
        {
            return GetModule(std::string_view(name));
        }
        virtual const ModuleGraph& MakeModuleGraph(const char* entry, 
            bool shared = false);
        virtual bool InitModuleGraph(void);
        virtual bool DestroyModuleGraph(void);
    public:
        virtual void Mount(const std::pmr::string& rootdir);
        virtual std::string_view GetRoot(void);
        virtual void RegisterStaticallyLinkedModule(
            std::string_view moduleName, registerer _register);
        virtual IModule* SpawnStaticModule(std::string_view name);
        virtual IModule* SpawnDynamicModule(std::string_view name);
        virtual ModuleProperty& GetModuleProp(std::string_view name);
        virtual void SetModuleProp(std::string_view name, const ModuleProperty& prop);
    private:
        bool __internal_DestroyModuleGraph(std::string_view nodename);
        void __internal_MakeModuleGraph(std::string_view entry,
            bool shared = false);
        bool __internal_InitModuleGraph(std::string_view nodename);
        Version CoreVersion{"0.1.0"};
        ModuleInfo ParseMetaData(const char* metadata);
    private:
        std::pmr::string moduleDir;
        std::vector<std::string_view> roots;
        std::pmr::string mainModuleName;
        ModuleGraph moduleDependecyGraph;
        std::pmr::unordered_map<std::string_view, int> NodeMap;
        std::pmr::unordered_map<std::string_view, registerer> InitializeMap;
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
        extern "C" DLLEXPORT Sakura::SPA::IModule* InitializeModule##ModuleName()\
        {\
            return new ModuleImplClass();\
        }
    
}
