# SPA
Module/Plugin manager for SakuraEngine. With cross-platform shared lib loading and dispatching.

## Dependencies
  * [[DependencyGraph]](../DependencyGraph)
  * Boost Graph Library
  * python (batchers)
## Introduction
  * Support static and shared build for modules and free-linking.
  * Describe modules with json file and compile modules' meta-data into targets.
  * No build tools dependencies(which means it works with CMake, XMake, or any of your own build tools).
  * Auto Load-Initialize ordered by module dependencies.
## Quick-Start
  All modules are described with json, please keep target name and module name same with "name" in json file.  
  All json should be named with a same name(as meta, in my case).   
  Run these two batchers before building: [[ModuleCollector]](ModuleCollector.py) & [[anyToheader]](anyToheader.py)    
  An auto-execute example in SakuraEngine:
  ```cmake
  #inject json data to meta.h header file
  execute_process(COMMAND python ../SPA/anyToheader.py meta .json ../SakuraEngine)
  #collect all headers of static libs to ensure initialization
  execute_process(COMMAND python ../SPA/ModuleCollector.py meta  ../SakuraEngine)
  ```
  See example here: [[SPAUnitTest]](../SPAUnitTest)
  ### json template
  ```json
  {
	"api": "0.1.0",
	"name": "CoreModule",
	"prettyname": "CoreModule",
	"version": "1.0.0",
	"linking": "static",
	"dependencies": [{}],
	"author": "SaeruHikari",
	"url": "https://github.com/misakaXunyan/SakuraEngine/tree/master/SakuraEngine/Core",
	"license": "MIT",
	"copyright": "SaeruHikari@2020"
}
  ```
  ### Static Module
```cpp
#pragma once
#include "Core.h"
#include "SPA/include/modulemanager.h"

class CoreModule : public Sakura::SPA::IStaticModule
{
    // meta data and some auto-implemented interfaces.
    #include "meta.h"
    virtual void OnLoad() override;
    // When the module is a main module, this will be called after total-initialization
    virtual void MainPluginExec() override;
    virtual void OnUnload() override;
};
IMPLEMENT_STATIC_MODULE(CoreModule, CoreModule);//first param: ClassName, second param: string name of module
```
  ### Shared Module
```cpp
#pragma once
#include "SPA/include/modulemanager.h"

class CGDModule : public Sakura::SPA::IDynamicModule
{
    // meta data and some auto-implemented interfaces.
    #include "meta.h"
    virtual void OnLoad() override;
    // When the module is a main module, this will be called after total-initialization
    virtual void MainPluginExec() override;
    virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(CGDModule, CGDModule);//first param: ClassName, second param: string name of module
```
  ### Dispatch
```cpp
#include "SPAModules.generated.h"//include generated file in the root path
#include "../SPA/include/modulemanager.h"
#include "plugin_main/plugin_main.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
#ifndef _WIN32
    mng->Mount("/home/saeruhikari/Coding/SakuraEngine/build");
#elif defined(_WIN32)
    mng->Mount("D:\\Coding\\SakuraEngine\\build");
#endif
    std::cout << "Root: " << mng->GetRoot() << std::endl;
    std::string mainName = "plugin_main";//Need a module as the entry, this module can be the total dispatcher
    mng->MakeModuleGraph(mainName.c_str());
    mng->InitModuleGraph();
    mng->DestroyModuleGraph();
    return 0;
}
```
