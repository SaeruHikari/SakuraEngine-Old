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
 * @LastEditTime: 2020-03-19 21:14:42
 */
#pragma once
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "Flags/CommonFeatures.h"
#include "ResourceObjects/Shader.h"
#include "Flags/ResourceFlags.h"
#include "GraphicsObjects/GraphicsPipeline.h"
#include "GraphicsObjects/RenderPass.h"
#include "GraphicsObjects/RootSignature.h"

namespace Sakura::Graphics
{
    sinterface SwapChain;
    sinterface ResourceView;
    sinterface GpuResource;
    sinterface GpuBuffer;
    sinterface GpuTexture;
    sinterface Fence;
    struct BufferCreateInfo;
    struct TextureCreateInfo;
    struct ResourceViewCreateInfo;
    struct SamplerCreateInfo;
}

namespace Sakura::Graphics
{
    struct CGDInfo
    {
        bool enableDebugLayer = false;
        std::vector<const char*> extentionNames;
        PhysicalDeviceFeatures physicalDeviceFeatures;
    };

    enum TargetGraphicsinterface 
    {
        CGD_TARGET_DIRECT3D12,
        CGD_TARGET_VULKAN,
        CGD_TARGET_METAL,
        CGD_TARGET_NUMS
    };

    sinterface CGD
    {
        virtual ~CGD() = default;
        virtual void Initialize(CGDInfo info) = 0;
        virtual [[nodiscard]] SwapChain* CreateSwapChain(
            const int width, const int height, void* mainSurface) = 0;

        virtual void Present(SwapChain* chain) = 0;

        virtual void Destroy() = 0;

        virtual std::unique_ptr<Shader> CreateShader(
            const char*, std::size_t) = 0;
        virtual const char* CompileShader(const char*, std::size_t) = 0;
        virtual void InitQueueSet(void* mainSurface) = 0;

        virtual [[nodiscard]] RenderPass* CreateRenderPass(
            const RenderPassCreateInfo& info) const = 0;
        
        virtual [[nodiscard]] GraphicsPipeline* CreateGraphicsPipeline(
            const GraphicsPipelineCreateInfo& info,
            const RenderPass& progress) const = 0;
            
        // Create & Destroy Command Contexts
        virtual CommandContext* AllocateContext(
            ECommandType type, bool bTransiant = true) = 0;
        virtual void FreeContext(CommandContext* context) = 0;
        virtual void FreeAllContexts(ECommandType typeToDestroy) = 0;

        virtual [[nodiscard]] ResourceView* ViewIntoResource(
            const GpuResource&, const ResourceViewCreateInfo&) const = 0;
        
        virtual CommandQueue* GetGraphicsQueue(void) const = 0;
        virtual CommandQueue* GetComputeQueue(void) const = 0;
        virtual CommandQueue* GetCopyQueue(void) const = 0;
        
        virtual [[nodiscard]] CommandQueue* AllocQueue(ECommandType type) const = 0;

        virtual [[nodiscard]] GpuBuffer* CreateResource(
            const BufferCreateInfo&) const = 0;
        
        virtual [[nodiscard]] GpuTexture* CreateResource(
            const TextureCreateInfo&) const = 0;

        virtual void Wait(Fence* toWait, uint64 until) const = 0;
        virtual void WaitIdle() const = 0;
        virtual [[nodiscard]] Fence* AllocFence(void) = 0;

        virtual [[nodiscard]] RootSignature*
            CreateRootSignature(const RootSignatureCreateInfo& sigInfo) const  = 0;

        virtual const TargetGraphicsinterface GetBackEndAPI(void) const = 0;

        virtual [[nodiscard]] GpuBuffer* CreateUploadBuffer(
            const std::size_t bufferSize) const;

        virtual [[nodiscard]] Sampler* CreateSampler(
            const SamplerCreateInfo&) const = 0;
    public:
        const uint64 contextNum() const {return contextPools[0].size();}

    protected:
        std::vector<std::unique_ptr<CommandContext>> 
            contextPools[ECommandType::CommandContext_Count];
        std::queue<CommandContext*> 
            availableContexts[ECommandType::CommandContext_Count];
        std::mutex contextAllocationMutex;
    };
}