/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:27:08
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 19:08:04
 */
#pragma once
#include "Core/CoreMinimal/SInterface.h"
#include "SWindow.h"

namespace Sakura
{
    Interface SApplication
    {
        virtual bool Initialize(void) = 0;
        virtual int Run(void) = 0;
        virtual bool Destroy(void) = 0;
    };
}