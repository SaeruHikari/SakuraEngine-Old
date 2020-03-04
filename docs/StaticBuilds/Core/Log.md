<!--
 * @Author: your name
 * @Date: 2020-03-04 08:11:43
 * @LastEditTime: 2020-03-04 08:22:04
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \docs\StaticBuilds\Core\Log.md
 -->
## Log
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
&emsp;&emsp;模板内的flag参数决定了log的输出级别, 不符合级别的log将会在编译时被屏蔽。默认采取DEBUG_GAME_AND_EDITOR, flag明细见: [SFlags.h](./Flags.md)。

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