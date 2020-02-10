/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 00:02:45
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 00:35:20
 */
#include "GraphicsInterface/GraphicsCommon/CommandObjects/Profiling.h"
#include "GraphicsInterface/GraphicsCommon/CommandObjects/CommandContext.h"
#include "Core/EngineUtils/ConsoleDesk.h"

namespace Sakura::Graphics
{
    void Profiling::BeginBlock(const spmr_string& ID,
        const CommandContext& context)
    {
        #ifdef SAKURA_GRAPHICS_PROFILING
        {
        
        };
        #else
        {};
        #endif
    }

    void Profiling::EndBlock(const spmr_string& ID,
        const CommandContext& context)
    {
        #ifdef SAKURA_GRAPHICS_PROFILING
        {
        
        };
        #else
        {};
        #endif
    }
}
