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
 * @Description: Vulkan implementation of CGD
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-15 23:27:21
 */
#pragma once
#include "../GraphicsCommon/CGD.h"
#include "CommandObjects/CommandQueueVk.h"
#include "GraphicsObjects/SwapChainVk.h"
#include "vulkan/vulkan.h"
#include <iostream>
#include "vk_mem_alloc.h"

using namespace Sakura::flags;

namespace Sakura::Graphics::Vk
{
    struct CGDEntityVk
    {
        Sakura::Graphics::PhysicalDeviceFeatures physicalDeviceFeatures;
        VkInstance instance;
        VkDevice device;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger;
        const std::vector<const char*> deviceExtensions = 
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
        };
        bool validate = false;
        std::unique_ptr<CommandQueueVk> graphicsQueue;
        std::unique_ptr<CommandQueueVk> computeQueue;
        std::unique_ptr<CommandQueueVk> copyQueue;
        VmaAllocator vmaAllocator;
        bool bTripleBuffering = true;
    };

    class CGD_Vk final : public CGD
    {
        DECLARE_LOGGER("CGD_Vk")
    public:
        CGD_Vk() = default;
        virtual void Destroy() override final; 
        virtual void DestroyCommandObjects() override final;
        virtual void InitQueueSet(void* mainSurface) override final;
        // Vulkan functions
        virtual void Initialize(CGDInfo info) override final;
        virtual std::unique_ptr<Sakura::Graphics::SwapChain>
            CreateSwapChain(const int width, const int height, 
                void* mainSurface) override final;
        virtual void Present(SwapChain* chain) override final;
        virtual CommandQueue* GetGraphicsQueue() const override final;
        virtual CommandQueue* GetComputeQueue() const override final;
        virtual CommandQueue* GetCopyQueue() const override final;
        virtual std::unique_ptr<CommandQueue> 
            AllocQueue(ECommandType type) const override final;
        const auto GetVkInstance() const {return entityVk.instance;}
        const CGDEntityVk& GetCGDEntity() const {return entityVk;}
    public:
        // Implements: See ResourceObjects/ShaderVk.cpp
        virtual std::unique_ptr<Shader> CreateShader(
            const char*, std::size_t) override final;
        virtual const char* CompileShader(
            const char*, std::size_t) override final;
    public:
        // Implements: See GraphicsObjects/GraphicsPipelineVk.cpp
        virtual std::unique_ptr<GraphicsPipeline> CreateGraphicsPipeline(
            const GraphicsPipelineCreateInfo& info,
            const RenderProgress& progress) const override final;
    public: 
        // Implements: See ResourceObjects/ResourceViewVk.cpp
        virtual std::unique_ptr<ResourceView> ViewIntoImage(
            const GpuResource&, const ResourceViewCreateInfo&) const override final;
    public:
    // Implements: See GraphicsObjects/RenderPassVk.cpp
        virtual std::unique_ptr<RenderProgress> CreateRenderProgress(
            const RenderProgressCreateInfo& rpInfo) const override final;
    public:
    // Implements: See CommandObjects/CommandContextVk.cpp
        virtual CommandContext* AllocateContext(
            ECommandType type, bool bTransiant = true) override final;
        virtual void FreeContext(CommandContext* context) override final;
        virtual void FreeAllContexts(ECommandType typeToDestroy) override final;
    public:
    // Implements: See GraphicsObjects/FenceVk.cpp
        virtual std::unique_ptr<Fence> AllocFence(void) override final;
        virtual void Wait(Fence* toWait, uint64 until) const override final;
        virtual void WaitIdle() const override final;
    public:
    // Implements: See ResourceObjects/GpuResourceVk.cpp
        virtual std::unique_ptr<GpuResource> CreateResource(
            const ResourceCreateInfo&) const override final;
    private:
         /**
         * @description: Initial Vulkan Device
         * @author: SaeruHikari
         */
        void VkInit(CGDInfo info);
        void setupDebugMessenger();
        void createVkInstance(uint pCount, const char** pName);
        void createAllocator();
        void pickPhysicalDevice(VkSurfaceKHR surface);
        uint32_t findMemoryType(
            uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    public:
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;
            std::optional<uint32_t> computeFamily;
            std::optional<uint32_t> copyFamily;
            bool isComplete() const
            {
                return graphicsFamily.has_value() 
                && presentFamily.has_value()
                && computeFamily.has_value()
                && copyFamily.has_value();
            }
            inline const uint32_t GetSize() const
            {   
                return 3;
            }
            inline const std::vector<uint32_t> GetIndices() const
            {
                static const std::vector<uint32_t> indices = 
                {
                    graphicsFamily.value(),
                    computeFamily.value(),
                    copyFamily.value()
                };    
                return indices;
            }
        };
        const QueueFamilyIndices& GetQueueFamily() const
        {
            return queueFamilyIndices;
        };
    private:
        QueueFamilyIndices queueFamilyIndices;
        CGDEntityVk entityVk;
        VkQueue presentQueue;
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
    Sakura::Graphics::Vk::CGD_Vk::debug_error("validation layer: " + 
        std::string(pCallbackData->pMessage));
    return VK_FALSE;
}
