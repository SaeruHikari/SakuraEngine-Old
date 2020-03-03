<!--
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-02 11:56:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-04 01:08:02
 -->
<h1 align="left">Core / CoreModule</h1>


## 核心
- [最小核心](./CoreMinimal): 纯头的核心关键字, 定义及函数。
- [容器](./Containers): 纯头的std::pmr/boost容器wrapper, 引擎的常用容器。
- [Math](./Math): 引擎的数学库, 尽量保持纯模板/纯内联。当前有[GeoMath](./Math/GeoMath/)支持。
- [EngineUtils](./EngineUtils/): 引擎工具集, 包含跨平台的OS基础操作支持, 高速文件流以及日志输出等。

Core会输出一个无状态的静态库Core.lib, 可以随意链接使用。

### CoreMinimal
&emsp;&emsp;CoreMinimal是一个纯头的包含集, 包含着引擎关键字, UUID以及各个平台的定义集等。全部的头被统一包含至总集CoreMinimal.h中。


### Containers
&emsp;&emsp;Containers选用了boost以及std::pmr下的容器。这些alias以及辅助使用的工具函数们被统一包含至[Containers.h](./Containers/Containers.h)下。


### Math
&emsp;&emsp;目前有[GeoMath](./Math/GeoMath/)的支持, 用以提供几何数学中的各种原子运算。GeoMath是一个Eigen3的Wrapper, 因此保持了纯模板的特性, 在使用时也就无需链接, 包含[GeoMath.h](./Math/GeoMath/GeoMath.h)即可。


### EngineUtils
&emsp;&emsp;[引擎工具集](./EngineUtils/), 此目录下有少量的 .cc/.cpp源文件。鉴于Utils无状态, 所有的源被链接并输出到静态库EngineUtils.lib下, 可以随意链接使用。这个工具集提供了跨平台的引擎基本操作。

#### Log
&emsp;&emsp;[Log](./EngineUtils/log.h)下提供了一系列log使用的函数。提供了无状态的debug_info/warn/error系列, 它们可以直接把信息输出至控制台。然而, 无状态的logger将无法提供筛选器信息。要使用筛选器, 必须使用有状态的logger。具体实例如下:
    
```cpp
    Sakura::log::debug_info<Sakura::flags::BuildVar::DEBUG_GAME_AND_EDITOR>
        ("SDL Vulkan: get total 2 extensions.");
    //[2020-03-02 13:41:30.669] [info] SDL Vulkan: get total 2 extensions.
    Sakura::log::regist_logger("EditorModule");
    Sakura::log::debug_info_l<Sakura::flags::BuildVar::DEBUG_GAME_AND_EDITOR>
        ("Editor Loaded!");
    //[2020-03-02 13:41:30.461] [EditorModule] [info] EditorModule Loaded!   
```
&emsp;&emsp;模板内的flag参数决定了log的输出级别, 不符合级别的log将会在编译时被屏蔽。默认采取DEBUG_GAME_AND_EDITOR, flag明细见: [SFlags.h](./CoreMinimal/SFlags.h)。

&emsp;&emsp;额外地, 为了方便进行模块化的调试, log内提供了一个方便快捷的logger注册宏, 用于类内。在任意一个类内(这里以EditorModule为例):
```cpp
#pragma once
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "SPA/include/modulemanager.h"

class SAKURA_API EditorModule : public Sakura::SPA::IDynamicModule
{
	#include "meta.h"
    //辅助注册宏
	DECLARE_LOGGER("EditorModule")
	//...
};
IMPLEMENT_DYNAMIC_MODULE(EditorModule, EditorModule);
```
&emsp;&emsp;则名为EditorModule的logger将会被申请, 可用EditorModule::debug_info/warn/error进行便捷的log。需要留意的是, 此宏将引入一个静态logger成员。

## 核心模块
&emsp;&emsp;为了兼容引擎的模块化开发, 并引入一些最基本的状态(例如基本的logger等), 将Core和[一组SPA Moduel接口](./CoreModule.h)链接并输出一个动态模块CoreModule.dll。在使用Core功能时, 更推荐直接包含[CoreModule.h](./CoreModule.h)并任意链接至CoreModule或Core之中的任一个。

&emsp;&emsp;核心模块作为dll导出的符号表, 可以在[CoreModule.h](./CoreModule.h)下自由查阅。原则上, 动态模块们只导出有状态的功能接口们(例如使用一个默认的logger进行log)。
