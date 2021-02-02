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
 * @Description: Prev-Definitions of Engine. 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-07-30 20:41:25
 * @LastEditTime: 2020-08-10 20:33:55
 */ 
#pragma once
#include <cstddef>
#include <cassert>
#include <stdint.h>

#define SAKURA_ERROR(node,...) printf_s(node, __VA_ARGS__)
#define SAKURA_FATAL(node,...) printf_s(node, __VA_ARGS__)

#if defined(__MINGW32__)
# define SAKURA_ISSUE_BREAK() DebugBreak();
#elif defined(_MSC_VER)
# define SAKURA_ISSUE_BREAK() __debugbreak();
#elif defined(__powerpc64__)
# define SAKURA_ISSUE_BREAK() asm volatile ("tw 31,1,1");
#elif defined(__i386__) || defined(__ia64__) || defined(__x86_64__)
# define SAKURA_ISSUE_BREAK() asm("int $3");
#else
# include <stdlib.h>
# define SAKURA_ISSUE_BREAK() abort();
#endif

#ifndef NDEBUG
#ifndef SAKURA_ASSERT_ENABLED
#define SAKURA_ASSERT_ENABLED
#endif
#endif

#ifdef SAKURA_ASSERT_ENABLED
#define SAKURA_BREAK() \
  do { \
    SAKURA_FATAL("BREAKPOINT HIT\n\tfile = %s\n\tline=%d\n", __FILE__, __LINE__); \
    SAKURA_ISSUE_BREAK() \
  } while (false)

#define SAKURA_ASSERT(cond) \
  do { \
    if (!(cond)) { \
      SAKURA_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n", __FILE__, __LINE__, #cond); \
      SAKURA_ISSUE_BREAK() \
    } \
  } while (false)

#define SAKURA_ASSERT_MSG(cond, ...) \
  do { \
    if (!(cond)) { \
	  SAKURA_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n\tmessage = ", __FILE__, __LINE__, #cond); \
      SAKURA_FATAL(__VA_ARGS__); \
      SAKURA_FATAL("\n"); \
      SAKURA_ISSUE_BREAK(); \
    } \
  } while (false)

#define SAKURA_ASSERT_CMD(cond, cmd) \
  do { \
    if (!(cond)) { \
      cmd; \
    } \
  } while (false)


#else
#define SAKURA_BREAK()
#define SAKURA_ASSERT(cond)
#define SAKURA_ASSERT_MSG(cond, ...)
#define SAKURA_ASSERT_CMD(cond, cmd)
#endif


#define SAKURA_CACHELINE_SIZE 64 // TODO: actually determine this.

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4251)//template dll interface
    #pragma warning(disable:5030)//unknown-attributes
    #pragma warning(disable:26812)//enum -> enum class

    #pragma warning(disable: 4244)
    #pragma warning(disable: 4267)
#endif

#ifndef FORCEINLINE
    #ifdef _MSC_VER
        #define FORCEINLINE __forceinline
    #else
        #define FORCEINLINE inline
    #endif
#endif

#ifndef restrict
    #define RESTRICT __restrict
#endif

namespace sakura
{
  typedef unsigned int uint;
  typedef unsigned char uchar;

  using char8_t = char;

  using uint8 = uint8_t;
  using uint8_t = uint8_t;
  using uint16 = uint16_t;
  using uint16_t = uint16_t;
  using uint32 = uint32_t;
  using uint32_t = uint32_t;
  using uint64 = uint64_t;
  using uint64_t = uint64_t;
  using int8 = int8_t;
  using int8_t = int8_t;
  using int16 = int16_t;
  using int16_t = int16_t;
  using int32 = int32_t;
  using int32_t = int32_t;
  using int64 = int64_t;
  using int64_t = int64_t;
  using size_t = std::size_t;
  using float32 = float;
  using float32_t = float;
  using float64 = double;
  using float64_t = double;
  using nullptr_t = std::nullptr_t;

	struct pos2d
	{
        uint32 x = 0;
        uint32 y = 0;
	};
	struct pos3d
	{
        uint32 x;
        uint32 y;
        uint32 z;
	};
    struct extent2d
    {
        uint32 width = 0;
        uint32 height = 0;
    };
    struct extent3d
    {
        uint32 width = 0;
        uint32 height = 0;
        uint32 depth = 0;
    };
	struct double4
	{
        double4(double _x = 0, double _y = 0, double _z = 0, double _w = 0)
	        :r(_x), g(_y), b(_z), a(_w)
        {
	        
        }
		union
		{
			struct{ double r, g, b, a;  }; //rgba view
			struct{ double x, y, z, w;  }; //xyzw view
		};
		constexpr double operator[](int index) const
		{
			switch (index)
			{
				case 0: return r;
				case 1: return g;
				case 2: return b;
				case 3: return a;
                default: return a;
			}
		}
	};

#define KINDA_SMALL_NUMBER	(1.e-4)
#define SMALL_NUMBER		(1.e-8)
#define THRESH_VECTOR_NORMALIZED 0.01
}

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

template<typename To, typename From>
FORCEINLINE To force_cast(From f) 
{
    return (To)f;
}

#define sakura_constexpr constexpr
#define sakura_noexcept noexcept

/*
#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#define SHAREDEXPORT EMSCRIPTEN_KEEPALIVE
#define API EMSCRIPTEN_KEEPALIVE
#define SHAREDLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#define SHAREDEXPORT __declspec(dllexport)
#ifdef API_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define API __attribute__((visibility("default")))
#define SHAREDEXPORT __attribute__((visibility("default")))
#define SHAREDLOCAL __attribute__((visibility("hidden")))
#define __stdcall 
#else
#define SHAREDEXPORT __declspec(dllexport)
#ifdef API_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#endif
*/
//#define RenderGraphAPI __declspec(dllexport)
