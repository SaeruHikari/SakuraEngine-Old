/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-14 00:31:40
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 22:30:04
 */
#pragma once
#include <memory_resource>
#include <string>
#include <vector>
#include <cstdlib>

namespace Sakura::SPA
{
    /**
     * @description: Represents a dependency as specified in the 
     * plugin.json file. A list of dependencies is stored in each
     * PluginInfo Object. 
     * @author: SaeruHikari
     */
    struct Dependency
    {
        std::string name; //!< The name of the dependency
        std::string version; //!< The version of the dependency
    };

    /**
     * @description: Struct that contains all plugin metadata
     * @author: SaeruHikari
     */
    struct ModuleInfo
    {
        std::string name; //!< name of the plugin
        std::string prettyname;//!< formatted name of the plugin
        std::string engine_version;//!< version of the engine
        std::string version;// !< version of the plugin
        std::string linking;// !< linking of the plugin
        std::string license;//!< license of the plugin
        std::string url; //!< url of the plugin 
        std::string copyright;//!< copyright of the plugin
        std::string make;//!<making method

        // Dependencies array
        std::pmr::vector<Dependency> dependencies;
    };
}