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
 * @Date: 2020-03-13 21:24:54
 * @LastEditTime: 2020-03-14 01:48:22
 */
#include "SakuraEngine/StaticBuilds/PixelOperators/DigitalImageProcess.h"
#include <iostream>
#include <thread>

int main(void)
{
    using namespace Sakura::Images;
    bool gray = true;
    int width, height, nc; 
    Image png = LoadFromDisk("D:\\pics\\grey.png", width, height, nc);
    if(!gray)
    {
        RGB2HSI<std::uint8_t>((std::uint8_t*)png.GetData(), width * height, nc);
        Sakura::Images::WirtePNGToDisk("D:\\pics\\grey_hsi.png",
                width, height, nc, png.GetData(), 0);
    }

    
    auto mapper =
        HistNormolize<true>(
            (std::uint8_t*)png.GetData(), width, height, nc, !gray);
    Sakura::Images::WirtePNGToDisk("D:\\pics\\grey_hsi_he.png",
        width, height, nc, png.GetData(), 0);

        
    if(!gray)
    {
        HSI2RGB<std::uint8_t>((std::uint8_t*)png.GetData(), width * height, nc);
            Sakura::Images::WirtePNGToDisk("D:\\pics\\grey_he.png",
        width, height, nc, png.GetData(), 0);
    }
    return 0;
}


