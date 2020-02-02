/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 12:58:52
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 14:04:03
 */
#pragma once
#include <memory>

namespace Sakura::Graphics
{
    struct CGD
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
    };
    extern std::unique_ptr<CGD> g_CGD;
}