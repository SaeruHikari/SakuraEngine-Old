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
 * @Description: Command Context
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-02 15:02:19
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