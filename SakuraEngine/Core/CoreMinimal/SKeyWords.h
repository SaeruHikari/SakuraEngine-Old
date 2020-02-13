/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Key words in Sakura
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:17:20
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 13:26:46
 */
#pragma once
#include <string>
#include <stdint.h>
using namespace std;

namespace Sakura
{
    typedef unsigned int uint;
    typedef unsigned char uchar;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    using size_t = size_t;
    using atom = size_t;

#ifndef sinline
    #if MSVC
        #define sinline __forceinline
    #else
        #define sinline inline
    #endif
#endif

#ifndef snoexcept 
    #if defined(DEBUG) || defined(_DEBUG)
        #define snoexcpet noexcept
    #else
        #define snoexcpt
    #endif
#endif

}