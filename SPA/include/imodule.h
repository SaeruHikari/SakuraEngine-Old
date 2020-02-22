/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-14 13:57:46
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 16:07:37
 */
#pragma once
#include "confinfo.h"
#include "moduleininfo.h"
#include "sharedlibrary.h"
#include <cstddef>

namespace Sakura::SPA
{
    class ModuleManager;
}

namespace Sakura::SPA
{
    /**
     * @description: Base of all plugins 
     * @author: SaeruHikari
     */
    struct IModule
    {
        friend class ModuleManager;
    public:
        IModule() = default;
        IModule(const IModule& rhs) = delete;
        IModule& operator=(const IModule& rhs) = delete;
        virtual ~IModule(){};
        /**
         * @description: Called by the plugin manager when the plugin
         * is loaded. All dependencies are available at this call.
         * @author: SaeruHikari
         */
        virtual void OnLoad() = 0;

        /**
        * @description: Called by the plugin manager when the plugin 
        * is unloaded. All dependencies remains valide at this call.
        * @author: SaeruHikari
        */
        virtual void OnUnload() = 0;    

        /**
         * @description: Called when the plugin is registered 
         * as the main plugin.
         * Main plugins can send request to all other plugins,
         * even if thery are not in the dependencies' list.
         * @note Always called after every plugins loaded() function.
         * @author: SaeruHikari
         */
        virtual void MainPluginExec() {}

        virtual const char* GetMetaData(void) = 0;
        virtual std::size_t GetMetaSize(void) = 0;

        virtual const ModuleInfo& GetModuleInfo()
        {
            return _information;
        }
    private:
        ModuleInfo _information;
    };
    struct IDynamicModule : public IModule
    {
        std::unique_ptr<SharedLibrary> sharedLib;
        virtual const char* GetMetaData(void) override
        {
            return sharedLib->get<const char*>("sp_meta"); 
        }
        virtual std::size_t GetMetaSize(void) override
        {
            return sharedLib->get<std::size_t>("sp_meta_size");
        }
    };
    struct IStaticModule : public IModule
    {

    };
}