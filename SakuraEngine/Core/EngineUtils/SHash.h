/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-01 01:20:04
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 19:22:01
 */
#pragma once
#include "SakuraEngine/Core/CoreMinimal/SDefination.h"
#include "xxhash.h"
#include <memory_resource>
#include <string>

namespace Sakura::hash
{
    using hash_code = uint64;
    static uint64 hash(const void* buffer, size_t size, uint64 seed)
    {
        return XXH64(buffer, size, seed);
    }

    static uint64 hash(const std::string& str, uint64 seed)
    {
        return XXH64(str.c_str(), str.size(), seed);
    }

    static uint64 hash(const std::pmr::string& str, uint64 seed)
    {
        return XXH64(str.c_str(), str.size(), seed);
    }
}
