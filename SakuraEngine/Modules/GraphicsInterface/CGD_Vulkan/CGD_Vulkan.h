/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Vulkan Version CGD
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:16:31
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 22:26:00
 */
#pragma once
#include "../GraphicsCommon/CGD.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include "SakuraEngine/Core/EngineUtils/log.h"

namespace Sakura::Graphics::Vk
{
    class CGD_Vk : public Sakura::Graphics::CGD
    {
        friend SInterface CGD;
    public:
        CGD_Vk() = default;
        virtual void Render(void) override;   
        virtual void Destroy(void) override; 
        inline SAKURA_API static void OpenValidateLayer()
        {
            CGD_Vk::validate = true;
        }
        // Vulkan functions
    private:
        static void Initialize(CGD_Info info);
        /**
         * @description: Initial Vulkan Device
         * @author: SaeruHikari
         */
        void VkInit(CGD_Info info);

        void setupDebugMessenger();
    private:
        static bool validate;
        void __createVkInstance(uint pCount, const char** pName);
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
    };
    
    /**
    * @description: Enumerate Instance Extension Properties
    * @return: Num of Vulkan extensions supported  
    * @author: SaeruHikari
    */
    static std::vector<VkExtensionProperties> VkEnumInstanceExtensions()
    {
        uint32 extensionCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        return extensions;
    }
    
    inline static const std::vector<const char*> validationLayers =
    {
        "VK_LAYER_KHRONOS_validation"
    };

    static bool VkCheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) 
        {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) 
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;break;
                }
            }
            if (!layerFound) 
                return false;
        }
        return true;
    } 
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData) 
{
    Sakura::log::debug_error("validation layer: " + 
        std::string(pCallbackData->pMessage));
    return VK_FALSE;
}