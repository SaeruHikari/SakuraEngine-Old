/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 23:23:02
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-02 17:59:39
 */
#define API_EXPORTS
#include "../include/modulemanager.h"
#include "../include/confinfo.h"
#include "../../Extern/include/json/json.hpp"
//#define SPA_OUTPUT_LOG
namespace Sakura::SPA
{
    void ModuleManager::RegisterStaticallyLinkedModule(
        const char* moduleName, registerer _register)
    {
        if (InitializeMap.find(moduleName) != InitializeMap.end())
        {
            return;
        }
        InitializeMap[moduleName] = _register;
    }

    IModule* ModuleManager::SpawnStaticModule(const std::pmr::string& name)
    {
        if (ModulesMap.find(name) != ModulesMap.end())
            return ModulesMap[name].get();
        if (InitializeMap.find(name) == InitializeMap.end())
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
        if (ModulesMap.find(name) != ModulesMap.end())
            return ModulesMap[name].get();
        std::unique_ptr<SharedLibrary> sharedLib
            = std::make_unique<SharedLibrary>();
        std::pmr::string initName("InitializeModule");
        std::pmr::string mName(name);
        initName.append(mName);
        std::pmr::string prefix = moduleDir;
#ifdef CONFINFO_PLATFORM_LINUX
    #if defined(DEBUG) || defined(_DEBUG)
        prefix.append("/Debug");
    #else
        prefix.append("/Release");
    #endif
        prefix.append("/lib/lib").append(name);
    #if defined(DEBUG) || defined(_DEBUG)
        prefix.append("d");
    #endif
        prefix.append(".so");
#elif defined(CONFINFO_PLATFORM_WIN32)
    #if defined(DEBUG) || defined(_DEBUG)
        prefix.append("\\Debug");
    #else
        prefix.append("\\Release");
    #endif
        prefix.append("\\bin\\").append(name);
    #if defined(DEBUG) || defined(_DEBUG)
        prefix.append("d");
    #endif
        prefix.append(".dll");
#endif
        if (!sharedLib->load(prefix))
        {
            std::cerr << prefix << std::endl 
                << "Load Shared Lib Error: " << sharedLib->errorString() << std::endl;
            return nullptr;
        }
#ifdef SPA_OUTPUT_LOG
        else
             std::cout << prefix << ": Load dll success!" << std::endl;
#endif
        if (sharedLib->hasSymbol(initName.c_str()))
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
        ModuleInfo info;
        try
        {
            using json = nlohmann::json;
            json tree = json::parse(metadata);
            if (Version(tree.at("api").get<std::string>()).compatible(CoreVersion))
            {
                info.name = tree.at("name").get<std::string>();
                info.prettyname = tree.at("prettyname").get<std::string>();
                info.engine_version = tree.at("api").get<std::string>();
                info.url = tree.at("url").get<std::string>();
                info.copyright = tree.at("copyright").get<std::string>();
                info.license = tree.at("license").get<std::string>();
                info.version = tree.at("version").get<std::string>();
                info.linking = tree.at("linking").get<std::string>();
                json jsonDependecy = tree.at("dependencies");
                for (json& jdep : jsonDependecy)
                {
                    Dependency dep;
                    dep.name = jdep.at("name").get<std::string>();
                    dep.version = jdep.at("version").get<std::string>();
                    info.dependencies.push_back(dep);
                }
                return info;
            }
        }
        catch (const std::exception & e)
        {
            //std::cerr << e.what() << '\n';
        }
        return info;
    }

    IModule* ModuleManager::GetModule(const std::pmr::string& name)
    {
        if (ModulesMap.find(name) == ModulesMap.end())
            return nullptr;
        return ModulesMap.find(name)->second.get();
    }

    ModuleProperty ModuleManager::GetModuleProp(const std::pmr::string& entry)
    {
        if(NodeMap.find(entry) == NodeMap.end())
            assert(0 && "Module Node not found");
        return DAG::get_vertex_property<ModuleProp_t>
            (ModuleNode(NodeMap[entry]), moduleDependecyGraph);
    }

    void ModuleManager::SetModuleProp(const std::pmr::string& entry, const ModuleProperty& prop)
    {
        DAG::set_vertex_property<ModuleProp_t>(
            DAG::vertex(NodeMap.find(entry)->second,
            moduleDependecyGraph),
            moduleDependecyGraph, prop);
    }

    bool ModuleManager::__internal_InitModuleGraph(const std::pmr::string& nodename)
    {
        if(GetModuleProp(nodename).bActive) 
            return true;
        for (auto&& iter : GetModule(nodename)->GetModuleInfo().dependencies)
        {
            if (GetModuleProp(iter.name).bActive)
                continue;
            if (!__internal_InitModuleGraph(iter.name))
                return false;
        }
        GetModule(nodename)->OnLoad();
        ModuleProperty prop;
        prop.bActive = true;
        prop.name = nodename;
        SetModuleProp(nodename, prop);
        return true;
    }

    void ModuleManager::__internal_MakeModuleGraph(const std::pmr::string& entry,
        bool shared)
    {
        if(NodeMap.find(entry) != NodeMap.end())
            return;
        IModule* mainModule = shared ?
            SpawnDynamicModule(entry)
            : SpawnStaticModule(entry);
        static int nodeNum = 0;
        NodeMap[entry] = nodeNum++;
        ModuleProperty prop;
        prop.name = entry;
        prop.bActive = false;
        DAG::add_vertex(prop, moduleDependecyGraph);
        if (mainModule->GetModuleInfo().dependencies.size() == 0)
            roots.push_back(entry);
        for (auto i = 0u; i < mainModule->GetModuleInfo().dependencies.size(); i++)
        {
            const char* iterName
                = mainModule->GetModuleInfo().dependencies[i].name.c_str();
            // Static
            if (InitializeMap.find(iterName) != InitializeMap.end())
                __internal_MakeModuleGraph(iterName, false);
            else
                __internal_MakeModuleGraph(iterName, true);
            DAG::add_edge(NodeMap[entry], NodeMap[iterName], moduleDependecyGraph);
        }
    }

    bool ModuleManager::__internal_DestroyModuleGraph(const std::pmr::string& nodename)
    {
        if(!GetModuleProp(nodename).bActive) 
            return true;
        auto nexts = DAG::inv_adjacent_vertices(
            ModuleNode(NodeMap.find(nodename)->second), moduleDependecyGraph);
        for (auto iter = nexts.first; iter != nexts.second; iter++)
        {
            auto name = DAG::get_vertex_property<ModuleProp_t>
                (*iter, moduleDependecyGraph).name;
            auto n = GetModuleProp(name);
            __internal_DestroyModuleGraph(n.name);
        }
        GetModule(nodename)->OnUnload();
        //DAG::remove_vertex(NodeMap[nodename], moduleDependecyGraph);
        ModuleProperty prop;
        prop.bActive = false;
        prop.name = GetModuleProp(nodename).name;
        SetModuleProp(nodename, prop);
        return true;
    }

    bool ModuleManager::InitModuleGraph(void)
    {
        if(GetModuleProp(mainModuleName).bActive)
            return true;
        if (!__internal_InitModuleGraph(mainModuleName))
            return false;
        GetModule(mainModuleName)->MainPluginExec();
        return true;
    }

    bool ModuleManager::DestroyModuleGraph(void)
    {
        for (auto& iter : roots)
        {
            if (!__internal_DestroyModuleGraph(iter))
                return false;
        }
        return true;
    }

    const ModuleGraph& ModuleManager::MakeModuleGraph
        (const std::pmr::string& entry, bool shared/*=false*/)
    {
        mainModuleName = entry;
        __internal_MakeModuleGraph(entry, shared);
        return moduleDependecyGraph;
    }

    void ModuleManager::Mount(const std::pmr::string& rootdir)
    {
        moduleDir = rootdir;
    }

    std::string_view ModuleManager::GetRoot(void)
    {
        return std::string_view(moduleDir);
    }
}

SPA_API Sakura::SPA::ModuleManager* __stdcall GetModuleManager()
{
    static Sakura::SPA::ModuleManager mModuleManager;
    return &mModuleManager;
}