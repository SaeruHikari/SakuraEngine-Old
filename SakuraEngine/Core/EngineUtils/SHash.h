/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-01 01:20:04
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 01:41:12
 */
#include "xxhash.h"

XXH64_hash_t xhash(const void* buffer, size_t size, XXH64_hash_t seed)
{
    return XXH64(buffer, size, seed);
}