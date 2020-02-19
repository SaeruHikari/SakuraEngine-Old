/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-19 11:18:38
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-19 11:35:16
 */
#pragma once
#include "../../include/modulemanager.h"

class SModule : public Sakura::SPA::IDynamicModule
{
    
    
    IMPLEMENT_DYNAMIC_MODULE(SModule, plugin_1);
};

#include "meta.h"