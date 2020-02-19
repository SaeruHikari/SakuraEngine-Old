/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 18:50:54
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-15 18:51:03
 */
#pragma once
#include "../../DependencyGraph/Graph.h"
#include <functional>
#include <memory>
#include <memory_resource>
#include <stdint.h>
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
    namespace ____
    {
        extern ModuleManager* mModuleManager;
    }    
    class ModuleManager
    {
        friend struct IModule;
    public:
        ModuleManager() = default;
        ModuleManager(std::string_view _moduleDir)
            : moduleDir(_moduleDir)
        {
            
        }
        static inline ModuleManager* Get()
        {
            if(____::mModuleManager != nullptr)
                return ____::mModuleManager;
            else
                return new ModuleManager();
        }
        IModule* GetModule(std::string_view name);
        template<typename T,
            std::enable_if<
                std::is_constructible<std::string_view, std::remove_reference<T>>::value
            >::type * = nullptr>
        inline IModule* GetModule(T&& name)
        {
            return GetModule(std::string_view(name));
        }
    private:
        void RegisterStaticallyLinkedModule(
            const std::pmr::string& moduleName, registerer _register);
    private:
        std::string_view moduleDir;
        ModuleGraph moduleDependecyGraph;
    private:
        std::pmr::unordered_map<std::pmr::string, registerer> InitializeMap;
        std::pmr::unordered_map<std::string_view, std::unique_ptr<IModule>>
            ModulesMap;
    };

    template<typename ModuleClass, 
        std::enable_if<
            std::is_base_of<IStaticModule, ModuleClass>::value>
        ::type * = nullptr>
    struct SStaticallyLinkedModuleRegistrant
    {
        SStaticallyLinkedModuleRegistrant(const std::pmr::string& InModuleName)
        {
            std::function<std::unique_ptr<IModule>(void)> func =
                []()
                {
                    return std::make_unique<ModuleClass>();                     
                };
            ModuleManager::Get()
                ->RegisterStaticallyLinkedModule(InModuleName, func);
        }
    };

    #define IMPLEMENT_STATIC_MODULE(ModuleImplClass, ModuleName)\
        static SStaticallyLinkedModuleRegistrant<ModuleImplClass>\
        ModuleRegistrant##ModuleName(##ModuleName);\
        /** static initialization for this lib can be forced by referencing this symbol */ \
        void EmptyLinkFunctionForStaticInitialization##ModuleName(){}\
        virtual const char* GetMetaData(void) override\
        {return __GetMetaData();}\
        virtual std::size_t GetMetaSize(void) override\
        {return __GetMetaSize();}

    #define IMPLEMENT_DYNAMIC_MODULE(ModuleImplClass, ModuleName) \
        extern "C" DLLEXPORT IModule* InitializeModule()\
        {\
            return new ModuleImpleClass();\
        }
}