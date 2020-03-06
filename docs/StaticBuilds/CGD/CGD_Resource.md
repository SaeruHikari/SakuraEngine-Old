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
 * @Date: 2020-03-05 15:49:13
 * @LastEditTime: 2020-03-06 12:17:56
 -->
<h1 align="left">CGD资源族</h1>

## 说明
&emsp;&emsp;执行图形任务所需的各种Gpu资源操作, 由资源族接口提供。资源族对象均被设计为支持RAII, 可以放心地接入任何您熟悉的内存管理系统中。

## 现代图形API的资源管理概要
&emsp;&emsp;由于CGD抛弃了旧图形API, 所以CGD的资源管理部分可以解开束缚, 向现代图形API靠拢。和旧式的API相比, CGD会有以下的不同点:

- 需要手动地进行一些资源管理, 而不是全权交给Driver进行申请和回收;
- 资源不再和类型硬性绑定, 而是需要您创建View来对资源进行访问;
- 资源的读写状态转换需要由用户进行显式的声明。
  
### 资源管理
&emsp;&emsp;经过CGD包裹的资源族对象遵循RAII原则, 当一个资源族对象析构时, 它所映射到的CPU以及GPU资源都会被完全回收掉。

&emsp;&emsp;需要注意的是, 由于资源管理和图形API创建的设备有硬性绑定关系, 每个资源族对象都需要CGDEntity来创建, 并都存有一份CGDEntity的引用。

### 视图
&emsp;&emsp;View在D3D12中被称作Descriptor, 这二词被译作"视图", "描述符"。从此名称可以看出, View旨在对无类型的资源进行描述, 为程序的访问提供一个视图。

!>&emsp;&emsp;一个形象的比喻是, 一个64bit可以被看作无符号整型, 有符号浮点, 有符号整型等, 它们会给这串bits带来不一样的语义。这种情况下, 我们就可以说我们以float64/uint64/int64视图对数据进行了不同意义的访问。 

&emsp;&emsp;这种设计毫无疑问是非常灵活的, 相当于弱化了类型到值, 即通过ViewInfo的配置您可以脱离类型束缚, 自由地(甚至在运行时)配置您的资源访问。

&emsp;&emsp;然而大部分的通用图形接口顾虑于兼容性, 依然将View进行硬性的类型划分。如此一来view并不能发挥出其优势, 甚至会引入混乱。CGD会保持基于值类型的资源访问, 并极大程度简化访问前的配置, 具体的调用模板如下:
```cpp
    // 旧式API
    Resource res = CreateImage(width, height);//硬性绑定类型
    //CGD
    CGDResource& resource;
    CGDView view(IMAGE_VIEW_TYPE_2D, width, height);
    view.Attach(resource);
    //...
    view.Detach();
```
&emsp;&emsp;您可以向一个资源绑定多个视图, 进行不同配置的访问, 或是作为别名绑定。

### 状态转换
&emsp;&emsp;