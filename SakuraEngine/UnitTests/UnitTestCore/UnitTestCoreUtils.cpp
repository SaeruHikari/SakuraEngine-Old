/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 16:52:45
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 23:57:19
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
    Sakura::SAutoString pmrPath = "home/env/../home";
    Sakura::spmr_string fixpath = "home/env/../home";
    Sakura::sstring_view view = Sakura::sstring_view(fixpath);

    spmr_string pmrS = "PMR";
    Sakura::SAutoString pmrStr = "PMR";
    std::cout << pmrStr.as<std::string_view>() << std::endl;
    std::cout << pmrStr.as<std::pmr::string>() << std::endl;
    std::cout << pmrStr.as<std::string>() << std::endl;
    
    double curr = Sakura::now::ms();
    for(auto i = 0u; i < 1000; i++)
        Sakura::path::clean(view);
    double end1kw = Sakura::now::ms();
    
    double total1 = end1kw - curr;
    //std::cout << pmrPath.as<std::string>();
    curr = Sakura::now::ms();
    for(auto i = 0u; i < 1000; i++)
    {
        Sakura::path::clean(pmrPath);
    }
    end1kw = Sakura::now::ms();

    double total2 = end1kw - curr;

    std::cout << std::endl << std::endl << total1 << std::endl 
        << total2 << std::endl;
}