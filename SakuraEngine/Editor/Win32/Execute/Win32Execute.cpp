/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:24:40
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-02 13:25:39
 */
#include "SakuraEngine/SPAModules.generated.h"//include generated file in the root path
#include "SPA/include/modulemanager.h"
#include "SakuraEngine/Core/CoreModule.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
#ifndef _WIN32
    mng->Mount("/home/saeruhikari/Coding/SakuraEngine/build");
#elif defined(_WIN32)
    mng->Mount("D:\\Coding\\SakuraEngine\\build");
#endif
    std::string mainName = "EditorModule";//Need a module as the entry, this module can be the total dispatcher
    mng->MakeModuleGraph(mainName.c_str(), true);
    auto edit = mng->GetModule("EditorModule");
    mng->InitModuleGraph();
    CoreModule::debug_info("EditorModule");
    mng->DestroyModuleGraph();
    return 0;
}