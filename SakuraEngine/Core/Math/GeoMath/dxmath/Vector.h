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
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-03-29 16:53:38
 * @LastEditTime: 2020-03-29 17:45:29
 */
#pragma once
#include <stdint.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Sakura::Math
{
    struct Vector2f
    {
        union
        {
            XMFLOAT2 __vector;
            struct
            {
                float x;
                float y;
            };
        };
        Vector2f() = default;
        Vector2f(const Vector2f&) = default;
        Vector2f& operator=(const Vector2f&) = default;
        Vector2f(Vector2f&&) = default;
        Vector2f& operator=(Vector2f&&) = default;

        XM_CONSTEXPR Vector2f(XMFLOAT2 vec)
            :__vector(vec){}
        XM_CONSTEXPR Vector2f(float _x, float _y) 
            : __vector(_x, _y) {}
    };

    struct Vector3f
    {
        union
        {
            XMFLOAT3 __vector;
            struct
            {
                float x;
                float y;
                float z;
            };
        };
        Vector3f() = default;
        XM_CONSTEXPR Vector3f(XMFLOAT3 vec)
            :__vector(vec){}
        XM_CONSTEXPR Vector3f(float _x, float _y, float _z) 
            : __vector(_x, _y, _z) {}
    };

    struct Vector4f
    {
        union
        {
            XMFLOAT4 __vector;
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };
        Vector4f() = default;
        XM_CONSTEXPR Vector4f(XMFLOAT4 vec)
            :__vector(vec){}
        XM_CONSTEXPR Vector4f(float _x, float _y, float _z, float _w) 
            : __vector(_x, _y, _z, _w) {}
    };
}