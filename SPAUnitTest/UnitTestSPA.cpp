/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-15 19:44:24
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 01:34:08
 */
#include "SPAModules.generated.h"
#include "../SPA/include/modulemanager.h"
#include "plugin_main/plugin_main.h"
#include <iostream>

int main()
{
    std::cout << "UnitTestSPA" << std::endl;
    auto mng = GetModuleManager();
#ifndef _WIN32
    mng->Mount("/home/saeruhikari/Coding/SakuraEngine/build");
#elif defined(_WIN32)
    mng->Mount("D:\\Coding\\SakuraEngine\\build");
#endif
    std::cout << "Root: " << mng->GetRoot() << std::endl;
    std::string mainName = "plugin_main";
    mng->MakeModuleGraph(mainName.c_str());
    if(!mng->InitModuleGraph())
        std::cerr << "Graph Init Failed!";
    mng->DestroyModuleGraph();
    return 0;
}