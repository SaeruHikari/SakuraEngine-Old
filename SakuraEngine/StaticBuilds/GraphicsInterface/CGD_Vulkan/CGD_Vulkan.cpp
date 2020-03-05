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
 * @LastEditTime: 2020-03-06 00:47:18
 */
#define API_EXPORTS
#include "CGD_Vulkan.h"
#include <set>
#include "Core/EngineUtils/ConsoleDesk.h"
#include "ResourceObjects/GpuResourceVk.h"
#include "ResourceObjects/ResourceViewVk.h"
#include "SakuraEngine/Core/Core.h"
#include "vulkan/vulkan.hpp"

using namespace Sakura::Graphics::Vk;
using namespace Sakura::Graphics;

void CGD_Vk::Initialize(Sakura::Graphics::CGDInfo info, CGDEntity& device)
{
    CGD_Vk::debug_info("CGD_Vk: Initialize");  
    VkInit(info, device);    
    if(info.enableDebugLayer)
        setupDebugMessenger(device);
}

void CGD_Vk::Render(CGDEntity& device)
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

void CGD_Vk::setupDebugMessenger(CGDEntity& device)
{
    if(!device.validate)
        return;
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(vkdevice.instance, &createInfo,
            nullptr, &vkdevice.debugMessenger) != VK_SUCCESS) 
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

void CGD_Vk::Destroy(CGDEntity& device)
{
    CGD_Vk::debug_info("CGD_Vk: Destroy");
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    if(vkdevice.validate)
        DestroyDebugUtilsMessengerEXT(vkdevice.instance,
            vkdevice.debugMessenger, nullptr);
    vkDestroyDevice(vkdevice.device, nullptr);
    vkDestroyInstance(vkdevice.instance, nullptr);
}

void CGD_Vk::VkInit(Sakura::Graphics::CGDInfo info, CGDEntity& device)
{
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    uint32 extensionCount = (uint32)VkEnumInstanceExtensions().size();
    if(extensionCount < 0)
        CGD_Vk::debug_error
            ("0 Vulkan extensions supported, check your platform/device!");
    else if(extensionCount < info.extentionNames.size())
        CGD_Vk::debug_error
            ("Do not support so many Vulkan extensions, check your CGDInfo");
    vkdevice.validate = info.enableDebugLayer;
    createVkInstance(static_cast<uint>(info.extentionNames.size()),
        info.extentionNames.data(), device);
    vkdevice.physicalDeviceFeatures = info.physicalDeviceFeatures;
}

void CGD_Vk::createVkInstance(uint pCount, const char** pName, CGDEntity& device)
{
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    if(vkdevice.validate && !VkCheckValidationLayerSupport())
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
    if (vkdevice.validate) 
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

    if (vkCreateInstance(&createInfo, nullptr, &vkdevice.instance) != VK_SUCCESS) 
        Sakura::log::error("Vulkan: failed to create instance!");
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

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice phy_device,
    VkSurfaceKHR surface) 
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(phy_device, 
        &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(phy_device, 
        &queueFamilyCount, queueFamilies.data());
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i, 
            surface, &presentSupport);
        if (presentSupport) 
            indices.presentFamily = i;
        if (indices.isComplete())
            break;
        i++;
    }
    return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice phy_device, 
    CGDEntityVk& vkDevice) 
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(phy_device, nullptr, 
        &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(phy_device, nullptr,
        &extensionCount, availableExtensions.data());
    
    std::set<std::string> 
        requiredExtensions(vkDevice.deviceExtensions.begin(), 
            vkDevice.deviceExtensions.end());
    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);
    return requiredExtensions.empty();
}

VkPhysicalDeviceFeatures getDeviceFeatureVk(
    Sakura::Graphics::PhysicalDeviceFeatures mask)
{
    using Sakura::Graphics::PhysicalDeviceFeatures;
    VkPhysicalDeviceFeatures deviceFeature = {};
    deviceFeature.logicOp = mask.val.test(PhysicalDeviceFeatures::logicOp);
    
    return deviceFeature;
}

#define _CGD_VK_IMPLEMENTATION_
#include "SwapChainVk.inl"

bool isDeviceSuitable(VkPhysicalDevice phy_device, 
        VkSurfaceKHR surface, CGDEntityVk& vkdevice) 
 {
    bool extensionsSupported = 
        checkDeviceExtensionSupport(phy_device, vkdevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) 
    {
        SwapChainSupportDetails swapChainSupport 
            = querySwapChainSupport(phy_device, surface);
        swapChainAdequate = 
            !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return findQueueFamilies(phy_device, surface).isComplete()
            && extensionsSupported && swapChainAdequate;
}

void CGD_Vk::pickPhysicalDevice(VkSurfaceKHR surface, CGDEntity& device)
{
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkdevice.instance, &deviceCount, nullptr);
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
    vkEnumeratePhysicalDevices(vkdevice.instance, &deviceCount, devices.data());

    for (const auto& phy_device : devices) {
        if (isDeviceSuitable(phy_device, surface, vkdevice)) 
        {
            vkdevice.physicalDevice = phy_device;
            break;
        }
    }
    if (vkdevice.physicalDevice == VK_NULL_HANDLE)
    {
        Sakura::log::error("Vulkan: failed to find a suitable GPU!");
        throw std::runtime_error("failed to find a suitable GPU!");
        return;
    }
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(vkdevice.physicalDevice, &deviceProperties);
    CGD_Vk::debug_info("Vulkan: physical device "
        + std::string(deviceProperties.deviceName));
}

std::unique_ptr<Sakura::Graphics::CommandQueue>
    CGD_Vk::InitQueueSet(void* mainSurface, CGDEntity& device)
{
    // Type re-generation
    CGDEntityVk& vkdevice = (CGDEntityVk&)(device);
    VkSurfaceKHR surface = *(VkSurfaceKHR*)mainSurface;

    // Queue Family Find
    pickPhysicalDevice(surface, device);
    QueueFamilyIndices indices = 
        findQueueFamilies(vkdevice.physicalDevice, surface);
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies 
        = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    // Queue Family Check
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    VkPhysicalDeviceFeatures deviceFeature = 
        getDeviceFeatureVk(vkdevice.physicalDeviceFeatures);
    

    // Create Logical Device
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeature;
    createInfo.enabledExtensionCount = 
        static_cast<uint32_t>(vkdevice.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = vkdevice.deviceExtensions.data();

    if (vkdevice.validate) 
    {
        createInfo.enabledLayerCount = 
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } 
    else
        createInfo.enabledLayerCount = 0;
    if (vkCreateDevice(vkdevice.physicalDevice, 
        &createInfo, nullptr, &vkdevice.device) != VK_SUCCESS) 
    {
        Sakura::log::error("Vulkan: failed to create logical device!");
        throw std::runtime_error("Vulkan: failed to create logical device!");
    }
    
    // Create Queue
    auto graphicsQueue = std::make_unique<CommandQueue_Vk>();
    vkGetDeviceQueue(vkdevice.device, indices.graphicsFamily.value(),
        0, &graphicsQueue->VkQueue);
    auto presentQueue = std::make_unique<CommandQueue_Vk>();
    vkGetDeviceQueue(vkdevice.device, indices.presentFamily.value(),
        0, &presentQueue->VkQueue);

    vkdevice.graphicsQueue = std::move(graphicsQueue);
    return std::move(presentQueue);
}
