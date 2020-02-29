/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Vulkan implementation of CommandContext
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:25:06
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 12:45:08
 */
#include "../GraphicsCommon/CommandObjects/CommandContext.h"
#include "CommandContext_Vk.h"
#include "Core/EngineUtils/log.h"


namespace Sakura::Graphics::Vk
{
    CommandContext* ContextManager_Vk::AllocateContext(ECommandType type, bool bTransiant)
    {
        Sakura::log::debug_info("Vulkan Context Manager Allocate Context!");
        return nullptr;
    }
}