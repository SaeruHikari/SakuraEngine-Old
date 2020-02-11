/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 10:19:46
 */
#pragma once
#include "../GraphicsCommon/CommandObjects/CommandContext.h"

namespace Sakura::Graphics::Vk
{
    class ContextManager_Vk : Implements ContextManager
    {
    public:
        virtual CommandContext* 
            AllocateContext(ECommandType type, bool bTransiant = true);
    };
} // namespace Sakura::Graphics

