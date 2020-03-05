<!--
 * @Author: your name
 * @Date: 2020-03-04 08:05:51
 * @LastEditTime: 2020-03-05 17:25:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \docs\StaticBuilds\README.md
 -->
 <h1 align="center">StaticBuilds</h1>
 <div align="center">
静态构建的功能块
</div>
&emsp;&emsp;静态构建栏目下会对引擎功能的静态导出版本进行介绍。这些静态导出们将会保证完全不引入状态。可以随意地链接它们获得引擎功能, 如此做会有以下的好处:

- 可复用引擎功能, 而不需要在运行时去装载那个模块;
- 省去dll解析符号跳转的overhead;
- 可以避免基层库版本迭代而带来的依赖性问题, 详见[Dll Hell](https://zh.wikipedia.org/wiki/DLL%E5%9C%B0%E7%8D%84)。
  
&emsp;&emsp;Sakura Engine以及第一方开发的所有模块, 都会遵循如下规则, 来确保可以最大程度地复用静态代码:

- 在功能实现中(Function.lib/a)不引入状态;
- 必要的状态, 链接至对应的模块中(FunctionLib.dll/so);