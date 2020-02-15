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
#include <unordered_map>

namespace Sakura::SPA
{
    struct IModule;
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
    public:
        ModuleManager() = default;
        static inline ModuleManager* Get()
        {
            if(____::mModuleManager != nullptr)
                return ____::mModuleManager;
            else
                return new ModuleManager();
        }
        void RegisterStaticallyLinkedModule(
            const std::pmr::string& moduleName, registerer _register);
    private:
        ModuleGraph moduleDependecyGraph;
    private:
        std::pmr::unordered_map<std::pmr::string, registerer> InitializeMap;
        std::pmr::unordered_map<std::string_view, std::unique_ptr<IModule>>
            ModulesMap;
    };
}