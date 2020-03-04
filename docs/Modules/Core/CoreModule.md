<!--
 * @Author: your name
 * @Date: 2020-03-04 08:00:26
 * @LastEditTime: 2020-03-04 08:12:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \docs\Modules\Core\CoreModule.md
 -->

## 核心模块
&emsp;&emsp;为了兼容引擎的模块化开发, 并引入一些最基本的状态(例如基本的logger等), 将Core和[一组SPA Moduel接口](./CoreModule.h)链接并输出一个动态模块CoreModule.dll。在使用Core功能时, 更推荐直接包含[CoreModule.h](./CoreModule.h)并任意链接至CoreModule或Core之中的任一个。

&emsp;&emsp;核心模块作为dll导出的符号表, 可以在[CoreModule.h](./CoreModule.h)下自由查阅。原则上, 动态模块们只导出有状态的功能接口们(例如使用一个默认的logger进行log)。
