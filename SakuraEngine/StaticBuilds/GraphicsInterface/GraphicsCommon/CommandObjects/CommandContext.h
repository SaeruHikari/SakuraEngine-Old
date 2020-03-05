/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-05 23:50:30
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-04 23:19:27
 */
#pragma once
#include <mutex>
#include <vector>
#include <memory>
#include <queue>
#include "Core/Containers/SString.h"
#include "Core/CoreMinimal/SInterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "Core/CoreMinimal/SAssert.h"

namespace Sakura::Graphics
{
    SInterface CommandQueue;
    SInterface CommandContext;
    SInterface ContextManager;
}

namespace Sakura::Graphics
{
    enum ECommandType   
    {
        CommandContext_Graphics = 0,
        CommandContext_Compute = 1,
        CommandContext_Copy = 2,
        CommandContext_Indirect = 3,
        CommandContext_Count = 4
    };

    SInterface ContextManager
    {
    public:
        virtual ~ContextManager() = default;
        ContextManager(void){}
        /**
         * @description: Allocate a command context, Type for d3d12  
         * and transiant only for vulkan API.  
         * @param {ECommandType type, bool bTransiant = true} 
         * @return: Allocated Command Context
         * @author: SaeruHikari
         */
        virtual CommandContext* 
            AllocateContext(ECommandType type, bool bTransiant = true) = 0;
        void FreeContext(CommandContext* context);
        void DestoryAllContexts();
    protected:
        std::vector<std::unique_ptr<CommandContext>> sm_ContextPools[4];
        std::queue<CommandContext*> sm_AvailableContexts[ECommandType::CommandContext_Count];
        std::mutex sm_ContextAllocationMutex;
    };
    SInterface CommandContext
    {
        friend SInterface ContextManager;
    public:
        /**
         * @description: Get a usable context from the manager 
         * @return: a command context ref
         * @author: SaeruHikari
         */
        static CommandContext& Begin(ContextManager*, const spmr_string& ID = "");
        
        /**
         * @description: Ends the encoding 
         * @param {type} 
         * @return: 
         * @author: SaeruHikari
         */
        virtual void End() = 0;

        sinline ECommandType GetCommandContextType(void)
        {
            return this->m_Type;
        }
    protected:
        spmr_string m_ID;
        ECommandType m_Type = ECommandType::CommandContext_Graphics;    
    };


}