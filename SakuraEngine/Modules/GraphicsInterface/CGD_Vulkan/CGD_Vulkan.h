/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Vulkan Version CGD
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 13:16:31
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 10:18:58
 */
#pragma once
#include "../GraphicsCommon/CGD.h"
#include <iostream>
namespace Sakura::Graphics::Vk
{
    class CGD_Vk : public Sakura::Graphics::CGD
    {
    public:
        CGD_Vk() = default;
        static void Initialize(void);
        virtual void Render(void) override;   
        virtual void Destroy(void) override; 
    };
}