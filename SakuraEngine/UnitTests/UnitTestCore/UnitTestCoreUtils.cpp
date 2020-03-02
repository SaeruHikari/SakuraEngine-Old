/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-02 15:02:30
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

    Sakura::log::debug_info<Sakura::flags::DEBUG_EDITOR>
        ("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
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

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}