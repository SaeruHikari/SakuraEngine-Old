/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:32:40
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 19:38:03
 */
#pragma once
#include "Framework/Application/SEngine.h"

namespace Sakura
{
    class SLinuxEngine : SImplements SEngine
    {
        friend class SEngine;
        SLinuxEngine(const EngineDesc& _desc)
        {
            
        }
    public:
        virtual bool Initialize(void) override;
        virtual int Run(void) override;
        virtual bool Destroy(void) override;
    };
}