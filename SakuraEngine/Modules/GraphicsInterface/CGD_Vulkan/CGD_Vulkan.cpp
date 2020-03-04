/*
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
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-04 19:12:49
 */
#define API_EXPORTS
#include "CGD_Vulkan.h"
#include <set>
#include "Core/EngineUtils/ConsoleDesk.h"
#include "SakuraEngine/Core/Core.h"
#include "vulkan/vulkan.hpp"

using namespace Sakura::Graphics::Vk;

void CGD_Vk::Initialize(Sakura::Graphics::CGD_Info info)
{
    CGD_Vk::debug_info("CGD_Vk: Initialize");
    auto reCGD = new CGD_Vk();    
    reCGD->VkInit(info);    
    if(info.enableDebugLayer)
        reCGD->setupDebugMessenger();
    eCGD = reCGD;
}

void CGD_Vk::Render(void)
{
    CGD_Vk::debug_info("CGD_Vk: Render");
}

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else 
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void CGD_Vk::setupDebugMessenger()
{
    if(!validate)
        return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo,
            nullptr, &debugMessenger) != VK_SUCCESS) 
    {
        Sakura::log::error("failed to set up debug messenger!");
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) 
    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

void CGD_Vk::Destroy(void)
{
    CGD_Vk::debug_info("CGD_Vk: Destroy");
    if(validate)
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    delete eCGD;
}

void CGD_Vk::VkInit(Sakura::Graphics::CGD_Info info)
{
    uint32 extensionCount = (uint32)VkEnumInstanceExtensions().size();
    if(extensionCount < 0)
        CGD_Vk::debug_error
            ("0 Vulkan extensions supported, check your platform/device!");
    else if(extensionCount < info.extentionNames.size())
        CGD_Vk::debug_error
            ("Do not support so many Vulkan extensions, check your CGD_Info");
            
    const std::vector<const char*> deviceExtensions = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    validate = info.enableDebugLayer;
    createVkInstance(static_cast<uint>(info.extentionNames.size()),
        info.extentionNames.data());
    deviceFeatures = info.deviceFeatures;
}

bool CGD_Vk::validate = false;
void CGD_Vk::createVkInstance(uint pCount, const char** pName)
{
    if(validate && !VkCheckValidationLayerSupport())
    {
        CGD_Vk::debug_error(
            "Vulkan: validation layers requested, \
            but not available!");
    }
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "CGD_Vk";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;

    // Validation Layer
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (validate) 
    {
        CGD_Vk::debug_info(
            "Vulkan: Enable Validation Layer!");
        createInfo.enabledLayerCount 
            = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames 
            = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = 
            (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } 
    else 
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    createInfo.enabledExtensionCount = pCount;
    createInfo.ppEnabledExtensionNames = pName;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
        Sakura::log::error("Vulkan: failed to create instance!");
    else
        CGD_Vk::debug_info(
            "Vulkan: create instance succeed!");
}

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
    VkSurfaceKHR surface) 
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) 
            indices.presentFamily = i;
        if (indices.isComplete())
            break;
        i++;
    }
    return indices;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) 
 {
    return findQueueFamilies(device, surface).isComplete();
}

void CGD_Vk::pickPhysicalDevice(VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) 
    {
        Sakura::log::error("Vulkan: failed to find GPUs with Vulkan support!");
        throw std::runtime_error("Vulkan: failed to find GPUs with Vulkan support!");
        return;
    }
    else
        CGD_Vk::debug_info("Vulkan: "+ 
            std::to_string(deviceCount) + " Physical Devices support");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, surface)) 
        {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
    {
        Sakura::log::error("Vulkan: failed to find a suitable GPU!");
        throw std::runtime_error("failed to find a suitable GPU!");
        return;
    }
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    CGD_Vk::debug_info("Vulkan: physical device "
        + std::string(deviceProperties.deviceName));
}

VkPhysicalDeviceFeatures getDeviceFeatureVk(
    Sakura::Graphics::DeviceFeatures mask)
{
    using Sakura::Graphics::DeviceFeatures;
    VkPhysicalDeviceFeatures deviceFeature = {};
    deviceFeature.logicOp = mask.val.test(DeviceFeatures::logicOp);
    
    return deviceFeature;
}

std::unique_ptr<Sakura::Graphics::CommandQueue>
    CGD_Vk::InitQueueSet(void* mainSurface)
{
    VkSurfaceKHR surface = *(VkSurfaceKHR*)mainSurface;
    pickPhysicalDevice(surface);
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies 
        = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    VkPhysicalDeviceFeatures deviceFeature = getDeviceFeatureVk(deviceFeatures);
    
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeature;
    createInfo.enabledExtensionCount = 0;
    if (validate) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } 
    else
        createInfo.enabledLayerCount = 0;
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
    {
        Sakura::log::error("Vulkan: failed to create logical device!");
        throw std::runtime_error("Vulkan: failed to create logical device!");
    }
    else
        CGD_Vk::debug_info("Vulkan: Create logical device");
    
    graphicsQueue = std::make_unique<CommandQueue_Vk>();
    vkGetDeviceQueue(device, indices.graphicsFamily.value(),
        0, &graphicsQueue->VkQueue);
    auto presentQueue = std::make_unique<CommandQueue_Vk>();
    vkGetDeviceQueue(device, indices.presentFamily.value(),
        0, &presentQueue->VkQueue);
    return std::move(presentQueue);
}
