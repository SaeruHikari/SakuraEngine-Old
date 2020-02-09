/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 16:52:45
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 00:04:19
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
}   

struct what
{
    std::string val;
};

TEST(UnitTestCoreUtils, PathUtil)
{
    // Environment val
    Sakura::spmr_string path = Sakura::os::env("PATH");
    std::cout << std::endl << path << std::endl;
    // clean test
    Sakura::sstring fixpath = "home/env/../home";
    Sakura::sstring_view view = Sakura::sstring_view(fixpath);


    
    double curr = Sakura::now::ms();
    for(auto i = 0u; i < 10000000; i++)
        Sakura::path::clean(fixpath);
    double end1kw = Sakura::now::ms();
    
    double total1 = end1kw - curr;
    //std::cout << pmrPath.as<std::string>();
    curr = Sakura::now::ms();
    for(auto i = 0u; i < 10000000; i++)
    {
        
    }
    end1kw = Sakura::now::ms();

    double total2 = end1kw - curr;

    std::cout << std::endl << std::endl << total1 << std::endl 
        << total2 << std::endl;
}