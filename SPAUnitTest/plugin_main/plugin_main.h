/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-19 11:19:30
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-19 13:52:31
 */
#pragma once
#include "../../SPA/include/modulemanager.h"
//#include "../../SPAModules.generated.h"

class SModule : public Sakura::SPA::IStaticModule
{
    #include "meta.h"
    virtual void OnLoad() override
    {
        std::cout << "SMoudle!";
    }
    virtual void OnUnload() override
    {
        
    }
    virtual const char* GetMetaData(void) override
    {return SModule::__GetMetaData();}
    virtual std::size_t GetMetaSize(void) override
    {return SModule::__GetMetaSize();}
};
IMPLEMENT_STATIC_MODULE(SModule, plugin_main)

