/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 19:44:24
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:59:05
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
    std::cout << std::endl << mainModule->GetMetaSize() << std::endl;
    std::cout << std::endl << dynModule->GetMetaSize() << std::endl;
    return 0;
}