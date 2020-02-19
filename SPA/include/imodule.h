/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-14 13:57:46
 * @LastEditors: SaeruHikari
 * @LastEditTime : 2020-02-14 14:31:35
 */
#pragma once
#include "confinfo.h"
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
        virtual ~IModule() = default;
        
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
    };
    struct IDynamicModule : public IModule
    {
        SharedLibrary sharedLib;
        virtual const char* GetMetaData(void) override
        {
            return sharedLib.get<const char*>("sp_meta"); 
        }
        virtual std::size_t GetMetaSize(void) override
        {
            return sharedLib.get<std::size_t>("sp_meta_size");
        }
    };
    struct IStaticModule : public IModule
    {

    };
}