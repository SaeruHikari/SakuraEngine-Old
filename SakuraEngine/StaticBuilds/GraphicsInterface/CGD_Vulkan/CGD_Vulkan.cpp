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
 * @LastEditTime: 2020-03-17 11:11:48
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

void CGD_Vk::Initialize(Sakura::Graphics::CGDInfo info)
{
    CGD_Vk::debug_info("CGD_Vk: Initialize");  
    VkInit(info);    
    if(info.enableDebugLayer)
        setupDebugMessenger();
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
    if(!entityVk.validate)
        return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(entityVk.instance, &createInfo,
            nullptr, &entityVk.debugMessenger) != VK_SUCCESS) 
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

void CGD_Vk::DestroyCommandObjects()
{
    CGD_Vk::debug_info("CGD_Vk: Destroy Command Objects");
    for(auto j = 0 ; j < 4; j++)
    { 
        for(auto i = 0u; i < contextPools[j].size(); i++)
            contextPools[j][i].reset();
        contextPools[j].clear();
    }
}

void CGD_Vk::Destroy()
{
    CGD_Vk::debug_info("CGD_Vk: Destroy");
    vmaDestroyAllocator(entityVk.vmaAllocator);
    if(entityVk.validate)
        DestroyDebugUtilsMessengerEXT(entityVk.instance,
            entityVk.debugMessenger, nullptr);
    vkDestroyDevice(entityVk.device, nullptr);
    vkDestroyInstance(entityVk.instance, nullptr);
}

void CGD_Vk::VkInit(Sakura::Graphics::CGDInfo info)
{
    //VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
    //VkEnum
    auto exts = VkEnumInstanceExtensions();
    uint32 extensionCount = (uint32)VkEnumInstanceExtensions().size();
    if(extensionCount < 0)
        CGD_Vk::error
            ("0 Vulkan extensions supported, check your platform/device!");
    else if(extensionCount < info.extentionNames.size())
        CGD_Vk::error
            ("Do not support so many Vulkan extensions, check your CGDInfo");
    entityVk.validate = info.enableDebugLayer;
    
    createVkInstance(static_cast<uint>(info.extentionNames.size()),
        info.extentionNames.data());
    entityVk.physicalDeviceFeatures = info.physicalDeviceFeatures;
}

void CGD_Vk::Present(SwapChain* chain)
{
    SwapChainVk* vkChain = (SwapChainVk*)chain;
    VkSwapchainKHR* swapChains = &vkChain->swapChain;
    vkChain->currentPresent = vkChain->nextPresent;
    vkAcquireNextImageKHR(
        entityVk.device,
        vkChain->swapChain,
        0,
        vkChain->imageAvailableSemaphores[vkChain->currentFrame],
        VK_NULL_HANDLE,
        &vkChain->nextPresent
    );
    VkPresentInfoKHR info = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    info.swapchainCount = 1;
    info.pSwapchains = swapChains;
    info.pImageIndices = &vkChain->currentPresent;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores 
        = &vkChain->imageAvailableSemaphores[vkChain->currentFrame];
    if(vkQueuePresentKHR(presentQueue, &info) != VK_SUCCESS)
    {
        Sakura::log::error("Vulkan: failed to present Vulkan graphics queue!");
        throw std::runtime_error("Vulkan:failed to present Vulkan graphics queue!");
    }
    vkChain->lastFrame = vkChain->currentFrame;
    vkChain->currentFrame = (vkChain->currentFrame + 1) % vkChain->swapChainCount;
}

CommandQueue* CGD_Vk::GetGraphicsQueue() const
{
    return entityVk.graphicsQueue.get();
}

CommandQueue* CGD_Vk::GetComputeQueue() const
{
    return entityVk.computeQueue.get();
}

CommandQueue* CGD_Vk::GetCopyQueue() const
{
    return entityVk.copyQueue.get();
}

void CGD_Vk::WaitIdle() const
{
    vkDeviceWaitIdle(entityVk.device);
}

std::unique_ptr<CommandQueue> CGD_Vk::AllocQueue(ECommandType type) const 
{
    auto graphicsQueue = new CommandQueueVk(*this);
    uint32 family; 
    switch (type)
    {
    case ECommandType::CommandContext_Compute:
        family = queueFamilyIndices.computeFamily.value();
        break;
    case ECommandType::CommandContext_Copy:
        family = queueFamilyIndices.copyFamily.value();
        break;
    case ECommandType::CommandContext_Graphics:
        family = queueFamilyIndices.graphicsFamily.value();
        break;
    default:
        CGD_Vk::error("AllocQueue: type not supported!");
        break;
    }
    vkGetDeviceQueue(entityVk.device,
        family,
        0, &graphicsQueue->vkQueue);
    return std::move(std::unique_ptr<CommandQueue>(graphicsQueue)); 
}

void CGD_Vk::createVkInstance(uint pCount, const char** pName)
{
    if(entityVk.validate && !VkCheckValidationLayerSupport())
    {
        CGD_Vk::debug_error(
            "Vulkan: validation layers requested, \
            but not available!");
    }
	vk::ApplicationInfo appInfo;
	appInfo.apiVersion = VK_API_VERSION_1_2;

	vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;

    // Validation Layer
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (entityVk.validate) 
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

    entityVk.instance = vk::createInstance(createInfo);
}
using QueueFamilyIndices = CGD_Vk::QueueFamilyIndices;
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
    for (const auto& queueFamily : queueFamilies) 
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        else if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            indices.computeFamily = i;
        }
        else if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            indices.copyFamily = i;
        }
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

void CGD_Vk::pickPhysicalDevice(VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(entityVk.instance, &deviceCount, nullptr);
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
    vkEnumeratePhysicalDevices(entityVk.instance, &deviceCount, devices.data());
    
    for (const auto& phy_device : devices) {
        if (isDeviceSuitable(phy_device, surface, entityVk)) 
        {
            entityVk.physicalDevice = phy_device;
            break;
        }
    }
    if (entityVk.physicalDevice == VK_NULL_HANDLE)
    {
        Sakura::log::error("Vulkan: failed to find a suitable GPU!");
        throw std::runtime_error("failed to find a suitable GPU!");
        return;
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(entityVk.physicalDevice, &deviceProperties);
    CGD_Vk::debug_info("Vulkan: physical device "
        + std::string(deviceProperties.deviceName));
    queueFamilyIndices = findQueueFamilies(entityVk.physicalDevice, surface);
}

void CGD_Vk::InitQueueSet(void* mainSurface)
{
    // Type re-generation
    VkSurfaceKHR surface = *(VkSurfaceKHR*)mainSurface;

    // Queue Family Find
    pickPhysicalDevice(surface);
    QueueFamilyIndices indices = 
        findQueueFamilies(entityVk.physicalDevice, surface);
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies 
        = {indices.graphicsFamily.value(),
           indices.computeFamily.value(),
           indices.copyFamily.value()
           };
    
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
        getDeviceFeatureVk(entityVk.physicalDeviceFeatures);
    
	vk::PhysicalDeviceTimelineSemaphoreFeatures timelineSemaphore;
	timelineSemaphore.timelineSemaphore = true;

	vk::PhysicalDeviceFeatures2 deviceFeatures;
	deviceFeatures.pNext = &timelineSemaphore;
    deviceFeatures.features = deviceFeature;

    // Create Logical Device
    VkDeviceCreateInfo createInfo = {};
    createInfo.pNext = &deviceFeatures;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.enabledExtensionCount = 
        static_cast<uint32_t>(entityVk.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = entityVk.deviceExtensions.data();

    if (entityVk.validate) 
    {
        createInfo.enabledLayerCount = 
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } 
    else
        createInfo.enabledLayerCount = 0;
    if (vkCreateDevice(entityVk.physicalDevice, 
        &createInfo, nullptr, &entityVk.device) != VK_SUCCESS) 
    {
        Sakura::log::error("Vulkan: failed to create logical device!");
        throw std::runtime_error("Vulkan: failed to create logical device!");
    }

    // Create Queue
    auto graphicsQueue = new CommandQueueVk(*this);
    auto computeQueue = new CommandQueueVk(*this);
    auto copyQueue = new CommandQueueVk(*this);
    vkGetDeviceQueue(entityVk.device, indices.graphicsFamily.value(),
        0, &graphicsQueue->vkQueue);
    vkGetDeviceQueue(entityVk.device, indices.presentFamily.value(),
        0, &presentQueue);
    vkGetDeviceQueue(entityVk.device, indices.computeFamily.value(),
        0, &computeQueue->vkQueue);
    vkGetDeviceQueue(entityVk.device, indices.copyFamily.value(),
        0, &copyQueue->vkQueue);
    entityVk.graphicsQueue = 
        std::move(std::unique_ptr<CommandQueueVk>(graphicsQueue));
    entityVk.computeQueue
        = std::move(std::unique_ptr<CommandQueueVk>(computeQueue));
    entityVk.copyQueue 
        = std::move(std::unique_ptr<CommandQueueVk>(copyQueue));

    createAllocator();
}

const TargetGraphicsInterface CGD_Vk::GetBackEndAPI(void) const
{
    return TargetGraphicsInterface::CGD_TARGET_VULKAN;
}
