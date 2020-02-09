/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 16:52:45
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 20:51:55
 */
#include <iostream>
#include "gtest/gtest.h"
#include "Core/Core.h"

TEST(UnitTestCoreUtils, TTimeUtil)
{
    auto timeStr = Sakura::now::str();
    std::cout << std::endl << timeStr << std::endl;

    char arr[15];
    Sakura::int32 i32val = 12; 
    Sakura::i32toa(i32val, arr);

    Sakura::spmr_string path = Sakura::os::env("PATH");
    std::cout << std::endl << path << std::endl;

    Sakura::spmr_string str = "null";
    Sakura::sstring_view view = Sakura::sstring_view(str);
    std::cout << Sakura::path::clean(view) <<std::endl;

    //Sakura::sstring p_s = Sakura::path::Sstr<Sakura::spmr_string>("WTF");

}