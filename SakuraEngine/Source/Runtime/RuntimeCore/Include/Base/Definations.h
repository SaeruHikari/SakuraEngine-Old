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
 * @Description: Prev-Definations of Engine. 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-07-30 20:41:25
 * @LastEditTime: 2020-08-10 20:33:55
 */ 
#pragma once
#include <cstddef>
#include <cassert>
#include <stdint.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
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
    struct extent2d
    {
        uint32 width = 0;
        uint32 height = 0;
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
#define KINDA_SMALL_NUMBER	(1.e-4f)
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
#elif defined(SAKURA_TARGET_PLATFORM_PROSPERO)
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
#pragma warning(disable:4251)//template dll interface
#pragma warning(disable:5030)//unknown-attributes
#pragma warning(disable:26812)//enum -> enum class