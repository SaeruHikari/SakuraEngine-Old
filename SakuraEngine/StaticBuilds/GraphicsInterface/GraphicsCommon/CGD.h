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
 * @LastEditTime: 2020-03-17 22:40:57
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "Flags/CommonFeatures.h"
#include "ResourceObjects/Shader.h"
#include "Flags/ResourceFlags.h"
#include "GraphicsObjects/GraphicsPipeline.h"
#include "GraphicsObjects/RenderProgress.h"

namespace Sakura::Graphics
{
    SInterface SwapChain;
    SInterface ResourceView;
    SInterface GpuResource;
    SInterface Fence;
    struct ResourceCreateInfo;
    struct ResourceViewCreateInfo;
}

namespace Sakura::Graphics
{
    struct CGDInfo
    {
        bool enableDebugLayer = false;
        std::vector<const char*> extentionNames;
        PhysicalDeviceFeatures physicalDeviceFeatures;
    };

    enum TargetGraphicsInterface 
    {
        CGD_TARGET_DIRECT3D12,
        CGD_TARGET_VULKAN,
        CGD_TARGET_METAL,
        CGD_TARGET_NUMS
    };

    SInterface CGD
    {
        virtual ~CGD() = default;
        virtual void Initialize(CGDInfo info) = 0;
        virtual std::unique_ptr<SwapChain> CreateSwapChain(
            const int width, const int height, 
            void* mainSurface) = 0;

        virtual void Present(SwapChain* chain) = 0;

        virtual void Destroy() = 0;
        virtual void DestroyCommandObjects() = 0;

        virtual std::unique_ptr<Shader> CreateShader(
            const char*, std::size_t) = 0;
        virtual const char* CompileShader(const char*, std::size_t) = 0;
        virtual void InitQueueSet(void* mainSurface) = 0;

        virtual std::unique_ptr<RenderProgress> CreateRenderProgress(
            const RenderProgressCreateInfo& info) const = 0;
        
        virtual std::unique_ptr<GraphicsPipeline> CreateGraphicsPipeline(
            const GraphicsPipelineCreateInfo& info,
            const RenderProgress& progress) const = 0;
            
        // Create & Destroy Command Contexts
        virtual CommandContext* AllocateContext(
            ECommandType type, bool bTransiant = true) = 0;
        virtual void FreeContext(CommandContext* context) = 0;
        virtual void FreeAllContexts(ECommandType typeToDestroy) = 0;

        virtual std::unique_ptr<ResourceView> ViewIntoImage(
            const GpuResource&, const ResourceViewCreateInfo&) const = 0;
        
        virtual CommandQueue* GetGraphicsQueue(void) const = 0;
        virtual CommandQueue* GetComputeQueue(void) const = 0;
        virtual CommandQueue* GetCopyQueue(void) const = 0;
        
        virtual std::unique_ptr<CommandQueue> AllocQueue(ECommandType type) const = 0;

        virtual std::unique_ptr<GpuResource> CreateResource(
            const ResourceCreateInfo&) const = 0;

        virtual void Wait(Fence* toWait, uint64 until) const = 0;
        virtual void WaitIdle() const = 0;
        virtual std::unique_ptr<Fence> AllocFence(void) = 0;

        //virtual void BindCBV();

        virtual const TargetGraphicsInterface GetBackEndAPI(void) const = 0;
    public:
        const uint64 contextNum() const {return contextPools[0].size();}

    protected:
        std::vector<std::unique_ptr<CommandContext>> 
            contextPools[ECommandType::CommandContext_Count];
        std::queue<CommandContext*> 
            availableContexts[ECommandType::CommandContext_Count];
        std::mutex contextAllocationMutex;
    public:
        template<ResourceType type>
        std::unique_ptr<ResourceView> ViewIntoResource(
            const GpuResource& resource, const ResourceViewCreateInfo& info) const
        {
            if constexpr (type == ResourceType::Texture2D)
                return std::move(ViewIntoImage(resource, info));
        }
    };
}