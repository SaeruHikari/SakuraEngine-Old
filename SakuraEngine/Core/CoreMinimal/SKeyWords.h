/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Key words in Sakura
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:17:20
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-23 23:49:49
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
}

#define snoexcept noexcept

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#define DLLEXPORT EMSCRIPTEN_KEEPALIVE
#define SAKURA_API EMSCRIPTEN_KEEPALIVE
#define DLLLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#elif defined(__GNUC__)
#define SAKURA_API __attribute__((visibility("default")))
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#else
#define DLLEXPORT __declspec(dllexport)
#ifdef API_EXPORTS
#define SAKURA_API __declspec(dllexport)
#else
#define SAKURA_API __declspec(dllimport)
#endif
#endif