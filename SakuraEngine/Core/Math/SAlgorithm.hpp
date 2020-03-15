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
 * @Date: 2020-03-13 18:31:44
 * @LastEditTime: 2020-03-13 18:36:42
 */
#pragma once
#ifdef min
#undef min
#endif

/**
 * @description: TO DO: SIMD optimize
 * @author: SaeruHikari
 */
namespace Sakura::Math
{
    template<typename T>
    T __internal__min(T a, T b)
    {
        return (a < b) ? a : b;
    }
    
    template<typename T, typename... Ts>
    T __internal__min(T a, T b, Ts... ts)
    {
        return __internal__min(__internal__min(a, b), ts...);
    }

    template<typename... Ts>
    auto min(Ts... ts)
    {
        return __internal__min(ts...); 
    }

        template<typename T>
    T __internal__max(T a, T b)
    {
        return (a > b) ? a : b;
    }
    
    template<typename T, typename... Ts>
    T __internal__max(T a, T b, Ts... ts)
    {
        return __internal__max(__internal__max(a, b), ts...);
    }

    template<typename... Ts>
    auto max(Ts... ts)
    {
        return __internal__max(ts...); 
    }
}