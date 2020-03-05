<!--
 * @Author: your name
 * @Date: 2020-03-04 08:07:31
 * @LastEditTime: 2020-03-05 16:50:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \docs\Modules\CGD\CGD_Vulkan.md
 -->
<h1 align="left">CGD Vulkan</h1>

## 说明
&emsp;&emsp;这是CGD的Vulkan实现。

## 明细与差异
### 设备族

#### CGDEntityVk

&emsp;&emsp;和其他现代图形API不同的是, Vulkan具有专有的对象surface以及PhysicalDevice。这是由Vulan的跨平台性质决定的。

```cpp
    struct CGDEntityVk : public CGDEntity
    {
        const auto GetVkInstance(){return instance;}
        Sakura::Graphics::PhysicalDeviceFeatures physicalDeviceFeatures;
        VkInstance instance;
        VkDevice device;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger;
        const std::vector<const char*> deviceExtensions = 
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
```

### 命令族

### 资源族





