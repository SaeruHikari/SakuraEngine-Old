<!--
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
 * @Date: 2020-03-03 12:13:48
 * @LastEditTime: 2020-03-04 11:56:06
 -->
<h1 align="left">Sakura Package Administer(SPA)</h1>

&emsp;&emsp;SakuraEngine的模块/插件管理器。可以跨平台加载共享库, 并根据依赖进行加载调度。

## 依赖
- [DependencyGraph](../DependencyGraph/Graph.h)头
- Boost Graph Library
- python (批处理需要)
  

## 宗旨
&emsp;&emsp;由于游戏引擎业务太过庞大繁杂, 且功能模块之间依赖严重, 手动进行模块加载以及初始化管理就变成了一件几乎不可能完成的事情。为了更加简便且鲁棒地管理功能模块, 就需要一个自动处理依赖的模块/插件管理器。

&emsp;&emsp;SPA就是一个轻量级的跨平台模块管理器, 可以极简地描述模块, 并将模块链入到运行时依赖链中。


## 特性
- 支持静态/动态构建的插件, 并且可以自由进行链接;
- 使用json文件来描述模块元信息(依赖, 名称等), 并在编译时注入到目标库中;
- 不依赖于任何构建工具(你可以使用xmake, cmake, premake等任何你喜欢的构建工具);
- 基于依赖图的依赖保序性自动加载/卸载。


## 类明细


### ModuleManager
&emsp;&emsp;这个类提供了插件集初始化/卸载/按名获取的基本接口。需要在运行时将Manager挂载到一个目录 (后称rt) 下, 管理器将会在此目录下进行工作。需要将动态库按照json中的名称放在rt/bin下, 以便管理器进行运行时装载。


### IModule
&emsp;&emsp;模块可以继承自IDynamicModule或IStaticModule。为了注册插件到管理器, 需要写注册宏(在全局空间下):
``` cpp
IMPLEMENT_DYNAMIC_MODULE(DynModule, DynModule);
IMPLEMENT_STATIC_MODULE(StcModule, StcModule);
```

&emsp;&emsp;为了把模块构造子注册到管理器中, 静态模块需要被显式包含。提供了批处理ModuleCollector来完成此项工作, 具体用例见下:
``` bash
# meta: json文件的名称  folder: 要递归搜集的目录
python ModuleCollector.py meta folder
```
&emsp;&emsp;将生成一个SPAModules.generated.h, 请将它包含到程序入口的最顶端。

### ModuleInfo
&emsp;&emsp;模块的详细元信息。可通过IModule->GetModuleInfo(void)获得只读引用。


## 快速入门
&emsp;&emsp;可以在这里找到完整的示例: [SPAUnitTest](../SPAUnitTest)。

### 执行批处理
&emsp;&emsp;所有的模块由json文件描述, 所以请保持编译目标(debug后缀为d)名称, 模块名称和包含头名称与json中的"name"相一致。

&emsp;&emsp;在编译链接前, 请执行下面两个批处理(您可以嵌入到任何的构建工具中):
```cmake
#inject json data to meta.h header file
execute_process(COMMAND python ../SPA/anyToheader.py meta .json ../SakuraEngine)
#collect all headers of static libs to ensure initialization
execute_process(COMMAND python ../SPA/ModuleCollector.py meta  ../SakuraEngine)
```


### json模板
``` json
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

### 静态模块
``` cpp
#pragma once
#include "Core.h"
#include "SPA/include/modulemanager.h"

class CoreModule : public Sakura::SPA::IStaticModule
{
    // 元数据和自动实现的接口
    #include "meta.h"
    virtual void OnLoad() override;
    // 当模块为主模块时, 此接口将会在全部模块加载完毕后调用
    virtual void MainPluginExec() override;
    virtual void OnUnload() override;
};
//参数1: 注册的类名  参数2: 注册的名称 (推荐都与json中name相一致)
IMPLEMENT_STATIC_MODULE(CoreModule, CoreModule);
```

### 动态模块
``` cpp
#pragma once
#include "SPA/include/modulemanager.h"

class CGDModule : public Sakura::SPA::IDynamicModule
{
    // 元数据和自动实现的接口
    #include "meta.h"
    // 如果需要为模块注册logger, 使用以下宏:
    DECLARE_LOGGER("CGDModule")
    virtual void OnLoad() override;
    // 当模块为主模块时, 此接口将会在全部模块加载完毕后调用
    virtual void MainPluginExec() override;
    virtual void OnUnload() override;
};
//参数1: 注册的类名  参数2: 注册的名称 (推荐都与json中name相一致)
IMPLEMENT_DYNAMIC_MODULE(CGDModule, CGDModule);
```


### 调用例
``` cpp
//包含根目录下生成的包含头
#include "SPAModules.generated.h"
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
    //主模块, 也是依赖构建和调用的入口
    std::string mainName = "plugin_main";
    mng->MakeModuleGraph(mainName.c_str());
    mng->InitModuleGraph();
    mng->DestroyModuleGraph();
    return 0;
}
```