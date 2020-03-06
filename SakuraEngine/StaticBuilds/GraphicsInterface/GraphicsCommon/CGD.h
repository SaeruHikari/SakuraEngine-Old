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
 * @LastEditTime: 2020-03-07 01:18:55
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "Format/CommonFeatures.h"
#include "ResourceObjects/Shader.h"
#include "ResourceObjects/ResourceFlags.h"
#include "GraphicsObjects/GraphicsPipeline.h"

namespace Sakura::Graphics
{
    SInterface SwapChain;
    SInterface ResourceView;
    SInterface GpuResource;
    struct ViewCreateInfo;
}

namespace Sakura::Graphics
{
    struct CGDInfo
    {
        bool enableDebugLayer = false;
        std::vector<const char*> extentionNames;
        PhysicalDeviceFeatures physicalDeviceFeatures;
    };

    enum class TargetGraphicsInterface : std::uint32_t
    {
        CGD_TARGET_DIRECT3D12,
        CGD_TARGET_VULKAN,
        CGD_TARGET_NUMS
    };

    SInterface CGD
    {
        virtual void Initialize(CGDInfo info) = 0;
        virtual std::unique_ptr<SwapChain> CreateSwapChain(
            const int width, const int height, 
            void* mainSurface) = 0;

        virtual void Render() = 0;
        virtual void Destroy() = 0;


        virtual std::unique_ptr<Shader> CreateShader(
            const char*, std::size_t) = 0;
        virtual const char* CompileShader(const char*, std::size_t) = 0;
        virtual std::unique_ptr<CommandQueue> InitQueueSet(
            void* mainSurface) = 0;

        virtual std::unique_ptr<ResourceView>
            ViewIntoImage(const GpuResource&, const ViewCreateInfo&) const = 0;

        template<ResourceType type>
        std::unique_ptr<ResourceView> ViewIntoResource(
            const GpuResource& resource, const ViewCreateInfo& info) const
        {
            if constexpr (type == ResourceType::Texture2D)
                return std::move(ViewIntoImage(resource, info));
        }
    };
}