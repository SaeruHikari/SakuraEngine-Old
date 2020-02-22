/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-19 11:18:30
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:00:45
 */
#pragma once
#include "../../SPA/include/modulemanager.h"

class SModule0 : public Sakura::SPA::IStaticModule
{
    #include "meta.h"
    virtual void OnLoad() override;
    virtual void OnUnload() override;
};
IMPLEMENT_STATIC_MODULE(SModule0, plugin_0);
