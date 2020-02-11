/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 12:58:52
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 23:51:09
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"

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
        sinline static CGD* GetCGD(void)
        {
            ASSERT_RUNTIME(Sakura::____::eCGD != nullptr);
            return Sakura::____::eCGD;
        }
        sinline static ContextManager* GetContextManager(void)
        {
            ASSERT_RUNTIME(GetCGD()->m_ContextManager.get() != nullptr);
            return GetCGD()->m_ContextManager.get();
        }
    private:
        std::unique_ptr<ContextManager> m_ContextManager;
    };
}