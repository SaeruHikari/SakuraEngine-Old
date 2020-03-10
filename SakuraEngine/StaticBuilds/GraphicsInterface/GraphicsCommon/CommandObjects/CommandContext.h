/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-05 23:50:30
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-10 11:23:41
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
    SInterface GraphicsPipeline;
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

    SInterface CommandContext
    {
        friend SInterface CGD;
    public:
        virtual ~CommandContext() = default;
        
        /**
         * @description: Ends the encoding, close the cmdlist/buffer 
         * @author: SaeruHikari
         */
        virtual void End() = 0;

        /**
         * @description: Begins the encoding, open the cmdlist/buffer 
         * @author: SaeruHikari
         */
        virtual void Begin(const GraphicsPipeline* gp) = 0;

        virtual void Draw(uint32 vertexCount, uint32 instanceCount,
            uint32 firstVertex, uint32 firstInstance) = 0;

        sinline ECommandType GetCommandContextType(void)
        {
            return this->m_Type;
        }
    protected:
        spmr_string m_ID;
        ECommandType m_Type = ECommandType::CommandContext_Graphics;    
    };


}