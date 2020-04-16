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
 * @Date: 2020-03-01 19:46:34
 * @LastEditTime: 2020-04-17 03:38:17
 */
#pragma once
#include "SakuraEngine/Core/CoreMinimal/SDefination.h"
#include "xxHash/xxhash.h"
#ifdef SAKURA_TARGET_PLATFORM_OSX
#include <experimental/memory_resource>
#include <experimental/string>
using namespace std::experimental;
#else
#include <memory_resource>
#endif
#include <string>

namespace Sakura::hash
{
    using hash_code = std::size_t;
    const uint64 defaultseed = 154643214564687;
    inline static hash_code hash(const void* buffer, size_t size, uint64 seed)
    {
        return XXH64(buffer, size, seed);
    }

    inline static hash_code hash(const std::string& str, uint64 seed)
    {
        return XXH64(str.c_str(), str.size(), seed);
    }

    inline static hash_code hash(const pmr::string& str, uint64 seed)
    {
        return XXH64(str.c_str(), str.size(), seed);
    }
}
