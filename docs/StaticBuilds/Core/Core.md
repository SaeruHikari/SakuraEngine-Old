<!--
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-02 11:56:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-04 08:19:51
 -->
<h1 align="left">Core / CoreModule</h1>

## 核心
- [最小核心](./CoreMinimal): 纯头的核心关键字, 定义及函数。
- [容器](./Containers): 纯头的pmr/boost容器wrapper, 引擎的常用容器。
- [Math](./Math): 引擎的数学库, 尽量保持纯模板/纯内联。当前有[GeoMath](./Math/GeoMath/)支持。
- [EngineUtils](./EngineUtils/): 引擎工具集, 包含跨平台的OS基础操作支持, 高速文件流以及日志输出等。

Core会输出一个无状态的静态库Core.lib, 可以随意链接使用。

### CoreMinimal
&emsp;&emsp;CoreMinimal是一个纯头的包含集, 包含着引擎关键字, UUID以及各个平台的定义集等。全部的头被统一包含至总集CoreMinimal.h中。


### Containers
&emsp;&emsp;Containers选用了boost以及pmr下的容器。这些alias以及辅助使用的工具函数们被统一包含至[Containers.h](./Containers/Containers.h)下。


### Math
&emsp;&emsp;目前有[GeoMath](./Math/GeoMath/)的支持, 用以提供几何数学中的各种原子运算。GeoMath是一个Eigen3的Wrapper, 因此保持了纯模板的特性, 在使用时也就无需链接, 包含[GeoMath.h](./Math/GeoMath/GeoMath.h)即可。


### EngineUtils
&emsp;&emsp;[引擎工具集](./EngineUtils/), 此目录下有少量的 .cc/.cpp源文件。鉴于Utils无状态, 所有的源被链接并输出到静态库EngineUtils.lib下, 可以随意链接使用。这个工具集提供了跨平台的引擎基本操作。


