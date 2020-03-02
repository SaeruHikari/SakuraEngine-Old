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
 * @Date: 2020-02-25 22:25:45
 * @LastEditTime: 2020-03-02 15:00:55
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