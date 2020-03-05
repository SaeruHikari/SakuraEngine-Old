<!--
 * @Author: your name
 * @Date: 2020-03-05 11:22:58
 * @LastEditTime: 2020-03-05 12:45:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \docs\Modules\CGD\CGD_Device.md
 -->
<h1 align="left">CGD设备族</h1>

## 简介
&emsp;&emsp;为了不引入全局状态, 现代图形API往往需要用户创建并维护各式各样的对象, 例如VkInstance, D3D12Device, Queue, SwapChain, Fence等。CGD保持了这一特性, 因此所有的CGD函数都是静态无副作用的(就像您在Vk和D3D12看到的任一函数一样)。

&emsp;&emsp;CGD设备族就是现代图形API设备的一套抽象接口。

![CGD_entity](Modules/CGD/../../CGDEntity.svg)

&emsp;&emsp;通过它您也可以不安全地访问到下层运行着的图形接口的具体设备, 但是并不推荐这样做。

## CGDInfo
为了创建CGDEntity, 您需要在CGDInfo中配置特性并在CGD::Initialize中传入。

- enableDebugLayer: 是否打开调试层, 默认false;
- extentionNames: 创建实体时需要的扩展名, 一般用于Vk;
- physicalDeviceFeatures: 硬件特异的特性们, 例如几何着色器, 光线追踪等。

## CGDEntity
对应着驱动现代图形API运作的一套设备对象们:

- physicalDevice: VkPhysicalDevice;
- instance: VkInstance, DXGIFactory;
- device: VkDevice, D3D12Device;
- debug: VkDebugUtilsMessengerEXT, D3DDebugLayer。

此外, CGDEntity还包含了一组设备唯一的[命令族]对象:

- queues: graphicsQueue, computeQueue, copyQueue各一条;
- contextManager: 用于申请CommandContext。