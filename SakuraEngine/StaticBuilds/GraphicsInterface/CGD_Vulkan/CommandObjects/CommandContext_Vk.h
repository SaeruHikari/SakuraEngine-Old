/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-07 00:03:57
 */
#pragma once
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"

namespace Sakura::Graphics::Vk
{
    class ContextManager_Vk : SImplements ContextManager
    {
    public:
        virtual CommandContext* AllocateContext(
                ECommandType type, bool bTransiant = true) override final;
    };
} // namespace Sakura::Graphics

