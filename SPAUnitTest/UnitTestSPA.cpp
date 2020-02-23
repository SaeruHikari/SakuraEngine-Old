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
#ifdef _Linux
    mng->Mount("/home/saeruhikari/Coding/SakuraEngine/build");
#elif defined(_WIN32)
    mng->Mount("D:\\Coding\\SakuraEngine\\build");
#endif
    std::cout << "Root: " << mng->GetRoot() << std::endl;
    std::string mainName = "plugin_main";
    mng->MakeModuleGraph(mainName.c_str());
    mng->InitModuleGraph();
    mng->DestroyModuleGraph();
    return 0;
}