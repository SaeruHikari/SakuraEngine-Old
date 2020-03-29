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
 * @Date: 2020-03-29 16:40:00
 * @LastEditTime: 2020-03-29 19:38:09
 */
#pragma once
#include <stdint.h>
#include "Vector.h"

using namespace DirectX;

namespace Sakura::Math
{
    struct Matrix4x4f
    {
        inline static Matrix4x4f Identity()
        {
            return Matrix4x4f(XMFLOAT4X4(1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1));
        }

        XM_CONSTEXPR Matrix4x4f(float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33)
        : _11(m00), _12(m01), _13(m02), _14(m03),
          _21(m10), _22(m11), _23(m12), _24(m13),
          _31(m20), _32(m21), _33(m22), _34(m23),
          _41(m30), _42(m31), _43(m32), _44(m33) {}
        
        XM_CONSTEXPR Matrix4x4f(XMFLOAT4X4 mat)
            :__matrix(mat){}

        Matrix4x4f() = default;

        XM_CONSTEXPR Vector3f GetRow3f(uint32_t row) const
        {
            return Vector3f(*((XMFLOAT3*)&__matrix.m[row]));
        }
        XM_CONSTEXPR Vector4f GetRow4f(uint32_t row) const
        {
            return Vector4f(*((XMFLOAT4*)&__matrix.m[row]));
        }
        
        inline Matrix4x4f Matrix4x4f::operator- () const
        {
            Matrix4x4f result;
            XMStoreFloat4x4(&result.__matrix, -XMLoadFloat4x4(&__matrix));
            return result;
        }

        inline Matrix4x4f& Matrix4x4f::operator+= (Matrix4x4f M)
        {
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            R += RR;
            XMStoreFloat4x4(&__matrix, R);
            return *this;
        }

        inline Matrix4x4f& Matrix4x4f::operator-= (Matrix4x4f M)
        {
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            R -= RR;
            XMStoreFloat4x4(&__matrix, RR);
            return *this;
        }

        inline Matrix4x4f& Matrix4x4f::operator*=(Matrix4x4f M)
        {
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            R *= RR;
            XMStoreFloat4x4(&__matrix, R);
            return *this;
        }

        inline Matrix4x4f& Matrix4x4f::operator*=(float S)
        {
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            R *= S;
            XMStoreFloat4x4(&__matrix, R);
            return *this;
        }
        
        inline Matrix4x4f& Matrix4x4f::operator/=(float S)
        {
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            R /= S;
            XMStoreFloat4x4(&__matrix, R);
            return *this;
        }

        inline Matrix4x4f Matrix4x4f::operator+(Matrix4x4f M)
        {
            Matrix4x4f result;
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            XMStoreFloat4x4(&result.__matrix, R + RR);
            return result;
        }

        inline Matrix4x4f Matrix4x4f::operator-(Matrix4x4f M)
        {
            Matrix4x4f result;
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            XMStoreFloat4x4(&result.__matrix, R - RR);
            return result;
        }

        inline Matrix4x4f Matrix4x4f::operator*(Matrix4x4f M)
        {
            Matrix4x4f result;
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            FXMMATRIX RR = XMLoadFloat4x4(&M.__matrix);
            XMStoreFloat4x4(&result.__matrix, R * RR);
            return result;
        }

        inline Matrix4x4f Matrix4x4f::operator*(float S)
        {
            Matrix4x4f result;
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            XMStoreFloat4x4(&result.__matrix, R * S);
            return result;
        }
         
        inline Matrix4x4f Matrix4x4f::operator/(float S)
        {
            Matrix4x4f result;
            XMMATRIX R = XMLoadFloat4x4(&__matrix);
            XMStoreFloat4x4(&result.__matrix, R / S);
            return result;
        }

        // data domain
        union
        {
            XMFLOAT4X4 __matrix;
            struct
            {
                float _11;float _12;float _13;float _14;
                float _21;float _22;float _23;float _24;
                float _31;float _32;float _33;float _34;
                float _41;float _42;float _43;float _44;
            };
        };
    };

    inline Matrix4x4f operator*
    (
        float S,
        Matrix4x4f M
    )
    {
        Matrix4x4f result;
        FXMMATRIX FM = XMLoadFloat4x4(&M.__matrix);
        XMStoreFloat4x4(&result.__matrix, S * FM);
        return result;
    }
    
}