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
 * @LastEditTime: 2020-03-05 12:39:53
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "Format/CommonFeatures.h"

namespace Sakura::Graphics
{
    struct CGDInfo
    {
        bool enableDebugLayer = false;
        std::vector<const char*> extentionNames;
        PhysicalDeviceFeatures physicalDeviceFeatures;
    };
    
    SInterface CGDEntity
    {
        ContextManager* GetContextManager(void)
        {
            return contextManager.get();
        }
        virtual std::string_view GetTargetInterface(void)
        {
            static std::pmr::string target = "null";
            return std::string_view(target);
        }
        bool validate = false;
    protected:
        std::unique_ptr<CommandQueue> graphicsQueue;
        std::unique_ptr<ContextManager> contextManager;
    };
    
    enum class TargetGraphicsInterface : std::uint32_t
    {
        CGD_TARGET_DIRECT3D12,
        CGD_TARGET_VULKAN,
        CGD_TARGET_NUMS
    };
}