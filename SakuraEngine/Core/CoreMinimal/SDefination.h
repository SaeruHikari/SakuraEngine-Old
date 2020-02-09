/*
 * @Descripttion: Common Defination of Engine Types and typedefs
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 00:13:54
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 10:59:00
 */
#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include "SKeyWords.h"

#ifndef load8
#define load8(p)     (*(const uint8*) (p))
#endif
#ifndef load16
#define load16(p)    (*(const uint16*)(p))
#endif
#ifndef load32
#define load32(p)    (*(const uint32*)(p))
#endif
#ifndef load64
#define load64(p)    (*(const uint64*)(p))
#endif
#ifndef save8
#define save8(p, v)  (*(uint8*) (p) = (v))
#endif
#ifndef save16
#define save16(p, v) (*(uint16*)(p) = (v))
#endif
#ifndef save32
#define save32(p, v) (*(uint32*)(p) = (v))
#endif
#ifndef save64
#define save64(p, v) (*(uint64*)(p) = (v))
#endif
#ifndef MAX_UINT8
#define MAX_UINT8  ((uint8)  ~((uint8) 0))
#endif
#ifndef MAX_UINT16
#define MAX_UINT16 ((uint16) ~((uint16)0))
#endif
#ifndef MAX_UINT32
#define MAX_UINT32 ((uint32) ~((uint32)0))
#endif
#ifndef MAX_UINT64
#define MAX_UINT64 ((uint64) ~((uint64)0))
#endif
#ifndef MAX_INT8
#define MAX_INT8   ((int8)  (MAX_UINT8  >> 1))
#endif
#ifndef MAX_INT16
#define MAX_INT16  ((int16) (MAX_UINT16 >> 1))
#endif
#ifndef MAX_INT32
#define MAX_INT32  ((int32) (MAX_UINT32 >> 1))
#endif
#ifndef MAX_INT64
#define MAX_INT64  ((int64) (MAX_UINT64 >> 1))
#endif
#ifndef MIN_INT8
#define MIN_INT8   ((int8)  ~MAX_INT8)
#endif
#ifndef MIN_INT16
#define MIN_INT16  ((int16) ~MAX_INT16)
#endif
#ifndef MIN_INT32
#define MIN_INT32  ((int32) ~MAX_INT32)
#endif
#ifndef MIN_INT64
#define MIN_INT64  ((int64) ~MAX_INT64)
#endif


namespace Sakura
{
    enum class ETargetPlatForm : std::uint32_t
    {
        WIN32,
        LINUX,
        OSX,
        COUNT
    };
}

/**
 * @description: put this into a class-body to 
 * delete copy and assign constructors.
 */
#define DISALLOW_COPY_AND_ASSIGN(Type) \
    Type(const Type&) = delete; \
    void operator=(const Type&) = delete


/**
 * @description: A wrapper of C-Style cast. 
 * @param {From}ToCast
 * @return: {To}Casted Val
 */
template<typename To, typename From>
sinline To force_cast(From f) 
{
    return (To)f;
}

#if defined(DEBUG) || defined(_DEBUG)
#else
    #ifdef SAKURA_DEBUG_EDITOR
    #undef SAKURA_DEBUG_EDITOR
    #endif
    #ifdef SAKURA_DEBUG_GAME
    #undef SAKURA_DEBUG_GAME
    #endif
#endif