/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:32:40
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 01:35:55
 */
#pragma once
#include "Framework/Application/SApplication.h"

namespace Sakura
{
    class LinuxEngine : Implements SApplication
    {
    public:
        virtual bool Initialize(void) override;
        virtual int Run(void) override;
        virtual bool Destroy(void) override;
        virtual std::unique_ptr<SWindow> CreateWindow(const WindowDesc& desc) override;
    };
}