/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-22 11:26:19
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:53:04
 */
#include "plugin_1.h"

void SModule1::OnLoad()
{
    std::cout << "SMoudle 1 Loaded!" << std::endl;
} 

void SModule1::OnUnload()
{
    std::cout << "SModule 1 Unloaded!" << std::endl;
}
