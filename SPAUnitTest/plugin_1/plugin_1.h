/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-19 11:18:38
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:10:02
 */
#pragma once
#include "../../SPA/include/modulemanager.h"

class SModule1 : public Sakura::SPA::IDynamicModule
{
    #include "meta.h"
    virtual void OnLoad() override;
    virtual void OnUnload() override;
};
IMPLEMENT_DYNAMIC_MODULE(SModule1, plugin_1);