/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 19:44:24
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-23 00:22:07
 */
#include "../SPA/SPAModules.generated.h"
#include "../SPA/include/modulemanager.h"
#include "plugin_main/plugin_main.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
    mng->Root("/home/saeruhikari/Coding/SakuraEngine/build");
    std::cout << "Root: " << mng->GetRoot() << std::endl;
    std::string mainName = "plugin_main";
    std::string_view dynName = "plugin_1";
    mng->MakeModuleGraph(mainName.c_str());
    auto mainModule = mng->GetModule(mainName);
    auto dynModule = mng->GetModule(dynName);
    std::cout << mainModule->GetModuleInfo().name << std::endl;
    std::cout << dynModule->GetModuleInfo().name << std::endl;
    std::cout << dynModule->GetModuleInfo().version << std::endl;
    return 0;
}