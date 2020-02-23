/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-22 12:05:59
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-22 12:55:34
 */
#include "plugin_main.h"

void SModule::OnLoad()
{
    std::cout << "SMoudle Loaded!" << std::endl;
}

void SModule::OnUnload()
{
    std::cout << "SMoudle UnLoad!" << std::endl;
}