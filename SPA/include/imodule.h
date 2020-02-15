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
#include "modulemanager.h"

namespace Sakura::SPA
{
    /**
     * @description: Base of all plugins 
     * @author: SaeruHikari
     */
    struct IModule
    {
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

    template<typename ModuleClass>
    struct SStaticallyLinkedModuleRegistrant
    {
        SStaticallyLinkedModuleRegistrant(const std::pmr::string& InModuleName)
        {
            std::funtion<std::unique_ptr<IModule>(void)> func =
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
    /** static initialization for this lib can be \
     forced by referencing this symbol */ \
    void EmptyLinkFunctionForStaticInitialization##ModuleName(){}
}