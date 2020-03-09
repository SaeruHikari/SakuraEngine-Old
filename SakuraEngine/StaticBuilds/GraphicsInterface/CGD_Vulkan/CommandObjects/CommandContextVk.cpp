/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Vulkan implementation of CommandContext
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:25:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-09 21:25:53
 */
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "../CGD_Vulkan.h"
#include "CommandContextVk.h"
#include "Core/EngineUtils/log.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

CommandContext& CGD_Vk::AllocateContext(ECommandType type, bool bTransiant)
{
    CommandContextVk context;
    return context;
}

void CommandContextVk::End()
{
    
}