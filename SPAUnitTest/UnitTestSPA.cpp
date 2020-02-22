/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 19:44:24
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 16:32:22
 */
#include "../SPA/include/modulemanager.h"
#include "plugin_main/plugin_main.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
    std::pmr::string mainName = "plugin_main";
    std::pmr::string dynName = "plugin_1";
    mng->SpawnStaticModule(mainName);
    auto mainModule = mng->GetModule(mainName);
    mng->SpawnDynamicModule(dynName);
    auto dynModule = mng->GetModule(dynName);
    std::cout << dynModule->GetModuleInfo().name << std::endl;
    std::cout << dynModule->GetModuleInfo().version << std::endl;
    std::cout << dynModule->GetModuleInfo().engine_version << std::endl;
    return 0;
}