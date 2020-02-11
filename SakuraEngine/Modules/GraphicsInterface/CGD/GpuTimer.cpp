/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: A fallback Gpu timer and simply do nothing now.
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 00:51:00
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 10:20:17
 */
#include "../GraphicsCommon/GpuTimer.h"
#include "../GraphicsCommon/CommandObjects/CommandContext.h"

using namespace Sakura;

namespace
{
    uint32 sm_MaxTimers = 0;
    uint32 sm_NumTimers = 0;
}

namespace Sakura::Graphics::GpuTimeManager
{
    void Initialize(uint32 MaxTimers)
    {
        ::sm_MaxTimers = MaxTimers;
        return;
    }
    
    void Shutdown(void)
    {
        return;
    }

    uint32 NewTimer(void)
    {
        return sm_NumTimers++;
    }

    void StartTimer(CommandContext& context, uint32 timerIdx)
    {
        return;
    }

    void StopTimer(CommandContext& context, uint32 timerIdx)
    {
        return;
    }

    void BeginReadBack(void)
    {
        return;
    }

    void EndReadBack(void)
    {
        return;
    }

    float GetTime(uint32 timerIndex)
    {
        return -1.f;
    }
}