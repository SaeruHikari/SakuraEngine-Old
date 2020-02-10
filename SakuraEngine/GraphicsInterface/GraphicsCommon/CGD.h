/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 12:58:52
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 20:49:30
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include <memory>
#include <stdint.h>

namespace Sakura::Graphics
{
    Interface CGD;
}

namespace Sakura
{
    namespace ____
    {
        extern Sakura::Graphics::CGD* eCGD;
    }
}

namespace Sakura::Graphics
{
    Interface CGD
    {
        enum class TargetGraphicsInterface : std::uint32_t
        {
            CGD_TARGET_DIRECT3D12,
            CGD_TARGET_VULKAN,
            CGD_TARGET_NUMS
        };
        static void Initialize(TargetGraphicsInterface targetGI);
        virtual void Render(void) = 0;
        virtual void Destroy(void) = 0;
        sinline const static CGD* GetCGD(void)
        {
            return Sakura::____::eCGD;
        }
    };
}