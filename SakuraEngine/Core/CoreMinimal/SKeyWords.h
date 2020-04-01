/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Key words in Sakura
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:17:20
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-04-01 17:50:42
 */
#pragma once
#include <string>
#include <stdint.h>

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
}

#ifndef sinline
    #if MSVC
        #define sinline __forceinline
    #else
        #define sinline inline
    #endif
#endif

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

#if defined(_MSC_VER)
# define SAKURA_COMPILER_MSVC _MSC_FULL_VER
#else
# define SAKURA_COMPILER_MSVC 0
#endif

#if defined(__clang__)
# define SAKURA_COMPILER_CLANG (__clang_major__ * 10000 + \
                                 __clang_minor__ * 100 + \
                                 __clang_patchlevel__)
#else
# define SAKURA_COMPILER_CLANG 0
#endif

#if defined(__GNUC__)
# define SAKURA_COMPILER_GCC (__GNUC__ * 10000 + \
                               __GNUC_MINOR__ * 100 + \
                               __GNUC_PATCHLEVEL__)
#else
# define SAKURA_COMPILER_GCC 0
#endif

/// \def SAKURA_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
/// Defined to 1 if the compiler supports returning a coroutine_handle from
/// the await_suspend() method as a way of transferring execution
/// to another coroutine with a guaranteed tail-call.
#if SAKURA_COMPILER_CLANG
# if __clang_major__ >= 7
#  define SAKURA_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER 1
# endif
#endif
#ifndef SAKURA_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
# define SAKURA_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER 0
#endif

#if SAKURA_COMPILER_MSVC
# define SAKURA_ASSUME(X) __assume(X)
#else
# define SAKURA_ASSUME(X)
#endif

#if SAKURA_COMPILER_MSVC
# define SAKURA_NOINLINE __declspec(noinline)
#elif SAKURA_COMPILER_CLANG || SAKURA_COMPILER_GCC
# define SAKURA_NOINLINE __attribute__((noinline))
#else
# define SAKURA_NOINLINE
#endif

#if SAKURA_COMPILER_MSVC
# define SAKURA_FORCE_INLINE __forceinline
#elif SAKURA_COMPILER_CLANG
# define SAKURA_FORCE_INLINE __attribute__((always_inline))
#else
# define SAKURA_FORCE_INLINE inline
#endif

#if defined(_WIN32_WINNT) || defined(_WIN32)
# if !defined(_WIN32_WINNT)
// Default to targeting Windows 10 if not defined.
#  define _WIN32_WINNT 0x0A00
# endif
# define SAKURA_OS_WINNT _WIN32_WINNT
#else
# define SAKURA_OS_WINNT 0
#endif

#if defined(__linux__)
# define SAKURA_OS_LINUX 1
#else
# define SAKURA_OS_LINUX 0
#endif

#if SAKURA_COMPILER_MSVC
# if defined(_M_IX86)
#  define SAKURA_CPU_X86 1
# endif
#elif SAKURA_COMPILER_GCC || SAKURA_COMPILER_CLANG
# if defined(__i386__)
#  define SAKURA_CPU_X86 1
# endif
#endif
#if !defined(SAKURA_CPU_X86)
# define SAKURA_CPU_X86 0
#endif

#if SAKURA_COMPILER_MSVC
# if defined(_M_X64)
#  define SAKURA_CPU_X64 1
# endif
#elif SAKURA_COMPILER_GCC || SAKURA_COMPILER_CLANG
# if defined(__x86_64__)
#  define SAKURA_CPU_X64 1
# endif
#endif
#if !defined(SAKURA_CPU_X64)
# define SAKURA_CPU_X64 0
#endif

#if SAKURA_CPU_X86
# define SAKURA_CPU_32BIT 1
#else
# define SAKURA_CPU_32BIT 0
#endif

#if SAKURA_COMPILER_MSVC
# define SAKURA_CPU_CACHE_LINE std::hardware_destructive_interference_size
#else
// On most architectures we can assume a 64-byte cache line.
# define SAKURA_CPU_CACHE_LINE 64
#endif