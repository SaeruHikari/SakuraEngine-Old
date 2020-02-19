/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 19:44:24
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-19 14:50:44
 */
#include "../SPA/include/modulemanager.h"
#include "plugin_main/plugin_main.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
    std::pmr::string mainName = "plugin_main";
    auto mainMod = mng->SpawnStaticModule(mainName);
    if(mainMod == nullptr)
        return 0;
    std::cout << "!!!!";
    std::cout << std::endl << mainMod->GetMetaSize() << std::endl;
    //mng->GetModule()
    return 0;
}