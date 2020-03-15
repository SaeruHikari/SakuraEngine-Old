/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:24:40
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-13 21:25:41
 */
#include "SakuraEngine/SPAModules.generated.h"//include generated file in the root path
#include "SPA/include/modulemanager.h"
#include "SakuraEngine/Core/CoreModule.h"
#include "SakuraEngine/StaticBuilds/TaskSystem/TaskSystem.h"
#include "StaticBuilds/PixelOperators/ImageUtils.h"
#include <iostream>
#include <thread>

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
    mng->DestroyModuleGraph();
    return 0;
}
