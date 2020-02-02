/*
 * @Descripttion: Common Defination of Engine Types and typedefs
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 00:13:54
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 10:20:08
 */
#pragma once
#include <string>
#include <map>
#include <unordered_map>

#ifndef sinline
    #if MSVC
        #define sinline __forceinline
    #else
        #define sinline inline
    #endif
#endif

typedef unsigned int uint;
typedef unsigned char uchar;
using byte = uchar;
using sstring = std::string;
using swstring = std::wstring;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using size_t = std::size_t;
using atom = std::size_t;

#ifndef TARGET_PLATFORM
#define TARGET_PLATFORM_WIN32 1
#define TARGET_PLATFORM
#endif

#ifdef TARGET_PLATFORM_WIN32
#include "SWinDefination.h"
 #else
#endif





