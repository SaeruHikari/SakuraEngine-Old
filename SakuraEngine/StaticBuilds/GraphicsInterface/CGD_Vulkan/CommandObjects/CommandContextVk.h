/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-09 21:24:20
 */
#pragma once
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"

namespace Sakura::Graphics::Vk
{
    class CommandContextVk : SImplements CommandContext
    {
        friend class CGD_Vk;
    public:
        virtual void End() override final;
    };
} // namespace Sakura::Graphics

