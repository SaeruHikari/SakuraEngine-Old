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
#include "Core/CoreMinimal/sinterface.h"
#include "Core/Containers/SString.h"

namespace Sakura::Graphics
{
    sinterface CommandContext;
}

namespace Sakura::Graphics
{
    sinterface DebugLayer
    {
    public:
        virtual void Enable() = 0;
        virtual void Disable() = 0;
    };

    struct Profiling
    {
    public:
        static void BeginBlock(const sstring& ID,
            const CommandContext& context);
        static void EndBlock(const sstring& ID,
            const CommandContext& context);
    };
} // Sakura::Graphics