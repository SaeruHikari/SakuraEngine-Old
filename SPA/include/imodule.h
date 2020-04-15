/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-01 19:46:34
 * @LastEditTime: 2020-04-15 18:17:34
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
    };
    struct IStaticModule : public IModule
    {

    };
}