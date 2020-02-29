/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 23:35:55
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 21:29:15
 */
#include "../GraphicsCommon/CommandObjects/CommandContext.h"
#include "../GraphicsCommon/CGD.h"
#include "../GraphicsCommon/CommandObjects/Profiling.h"

namespace Sakura::Graphics
{
    void ContextManager::FreeContext(CommandContext* context)
    {
        ASSERT_RUNTIME(context != nullptr);
        std::lock_guard<std::mutex> LockGurad(sm_ContextAllocationMutex);
        sm_AvailableContexts[context->m_Type].push(context);
    }

    void ContextManager::DestoryAllContexts(void)
    {
        for(auto i = 0u; i < 4; i++)
            sm_ContextPools[i].clear();
    }

    CommandContext& CommandContext::Begin(const spmr_string& ID)
    {
        CommandContext* NewContext = 
        CGD::GetCGD()->GetContextManager()
            ->AllocateContext(CommandContext_Graphics);
        NewContext->m_ID = ID;
        if(ID.length() > 0)
            Profiling::BeginBlock(ID, *NewContext);
        return *NewContext;
    }
}