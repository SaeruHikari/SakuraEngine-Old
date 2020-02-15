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

namespace Sakura::SPA
{
    class ModuleManager;
}

namespace Sakura::SPA
{
    namespace ____
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
        protected:
            
        };
    }
    struct IDynamicModule : public ____::IModule
    {
        SharedLibrary sharedLib;
    };
    struct IStaticModule : public ____::IModule
    {
        
    };
}