/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 23:28:31
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-12 12:31:20
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "Core/Containers/SString.h"

namespace Sakura::Graphics
{
    SInterface CommandContext;
}

namespace Sakura::Graphics
{
    SInterface DebugLayer
    {
    public:
        virtual void Enable() = 0;
        virtual void Disable() = 0;
    };

    struct Profiling
    {
    public:
        static void BeginBlock(const spmr_string& ID,
            const CommandContext& context);
        static void EndBlock(const spmr_string& ID,
            const CommandContext& context);
    };
} // Sakura::Graphics