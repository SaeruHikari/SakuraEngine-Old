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
#include "Base/CDefinitions.h"
#include <cstddef>
#include <stdint.h>

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
    using pos2d = ::pos2d;
    using pos3d = ::pos3d;
    using extent2d = ::extent2d;
    using extent3d = ::extent3d;

	struct double4 : public ::double4
	{
        double4(double _x = 0, double _y = 0, double _z = 0, double _w = 0)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
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

template<typename To, typename From>
FORCEINLINE To force_cast(From f) 
{
    return (To)f;
}

#define sakura_constexpr constexpr
#define sakura_noexcept noexcept
