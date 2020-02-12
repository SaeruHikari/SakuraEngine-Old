/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 16:52:45
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 13:40:07
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

    sstream strea;
    strea << "\nSStream Test\n";
    std::cout << strea.c_str();
}   

TEST(UnitTestCoreUtils, PathUtil)
{
    // Environment val
    Sakura::spmr_string path = Sakura::os::env("PATH");
    std::cout << std::endl << path << std::endl;
    // clean test
    Sakura::sstring fixpath = "home/env/../home/game.exe";
    Sakura::sstring_view fixpathview = "home/env/../home";

    std::cout << "Sakura::dir: " << Sakura::path::clean(fixpath) << std::endl;
    std::cout << Sakura::path::ext(fixpath) << std::endl;
    
    auto joined = Sakura::path::join("/Joined", fixpathview, fixpath);
    std::cout << std::endl << Sakura::path::clean(joined) << std::endl;
    //std::cout << Sakura::path::base(fixpath);
    std::cout << std::endl << "sstring: " << typeid(Sakura::path::split(fixpath).first).name();
    std::cout << std::endl << "sstring_view: " << typeid(Sakura::path::split(fixpathview).first).name();
    std::cout << std::endl << "spmr_string: " << typeid(Sakura::path::split(path).first).name();
}

TEST(UnitTestCoreUtils, FileSystem)
{
    std::cout << fs::exists("D:/Coding") << fs::exists("D:/Coding/testtxt.txt") << std::endl;
    fs::fstream fst(8192);
    sstream strea;
    strea << "\nSStream Test\n";
    strea << fs::fsize("D:/Coding/testtxt.txt");

    std::cout << strea.c_str();

    fs::file testtxt;
    testtxt.open("D:/Coding/testtxt.txt", 'r');
    auto str = testtxt.read(fs::fsize("D:/Coding/testtxt.txt"));
    std::cout << str;
}