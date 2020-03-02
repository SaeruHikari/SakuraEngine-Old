/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 12:58:52
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 12:31:10
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "CommandObjects/CommandContext.h"
#include "SakuraEngine/Core/EngineUtils/log.h"


namespace Sakura::Graphics
{
    template<typename... Ts>
    void debug_info(Ts... params)
    {
        Sakura::log::debug_info_l("Graphics", params...);
    }
    template<typename... Ts>
    void debug_warn(Ts... params)
    {
        Sakura::log::debug_warn_l("Graphics", params...);
    }
    template<typename... Ts>
    void debug_error(Ts... params)
    {
        Sakura::log::debug_error_l("Graphics", params...);
    }
    struct CGD_Info
    {
        bool enableDebugLayer = false;
        std::vector<const char*> extentionNames;
    };
    SInterface CGD
    {
        virtual ~CGD() = default;
        enum class TargetGraphicsInterface : std::uint32_t
        {
            CGD_TARGET_DIRECT3D12,
            CGD_TARGET_VULKAN,
            CGD_TARGET_NUMS
        };
        SAKURA_API static void Initialize(TargetGraphicsInterface targetGI,
            CGD_Info info = {});
        virtual void Render(void) = 0;
        virtual void Destroy(void) = 0;
        SAKURA_API static CGD* GetCGD(void)
        {
            ASSERT_RUNTIME(eCGD != nullptr);
            return eCGD;
        }
        SAKURA_API static ContextManager* GetContextManager(void)
        {
            ASSERT_RUNTIME(GetCGD()->m_ContextManager.get() != nullptr);
            return GetCGD()->m_ContextManager.get();
        }
    protected:
        SAKURA_API static Sakura::Graphics::CGD* eCGD;
        std::unique_ptr<ContextManager> m_ContextManager;
    };
}