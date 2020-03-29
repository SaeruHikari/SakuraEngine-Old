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
 * @Date: 2020-03-29 18:26:06
 * @LastEditTime: 2020-03-29 18:39:14
 */
#pragma once
#include "Matrix.h"

using namespace DirectX;

namespace Sakura::Math
{
    inline static Vector3f GetScale(const Matrix4x4f& transform) 
    {
        XMVECTOR S, R, T;
        XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&transform.__matrix)); 
        Vector3f scale;
        XMStoreFloat3(&scale.__vector, S);
        return scale;
    }

    inline static XM_CONSTEXPR Vector3f GetLocation(const Matrix4x4f& transform) 
    {
        return transform.GetRow3f(3);
    }

    inline static Vector4f GetRotation(const Matrix4x4f& transform) 
    {
        XMVECTOR S, R, T;
        XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&transform.__matrix)); 
        Vector4f rotation;
        XMStoreFloat4(&rotation.__vector, R);
        return rotation;
    }
    
    inline static void Translate(Matrix4x4f& transform, const Vector3f& value)
    {
        XMVECTOR trans = XMLoadFloat3(&value.__vector);
        XMStoreFloat4x4(&transform.__matrix,
            XMMatrixTranslationFromVector(trans) * XMLoadFloat4x4(&transform.__matrix));
    }

    inline static Matrix4x4f MatrixScalingFromVector(const Vector3f& scale)
    {
        Matrix4x4f result;
        XMVECTOR s = XMLoadFloat3(&scale.__vector);
        XMStoreFloat4x4(&result.__matrix, XMMatrixScalingFromVector(s));
        return result;
    }

    inline static Matrix4x4f MatrixScalingFromVector(const Vector4f& scale)
    {
        Matrix4x4f result;
        XMVECTOR s = XMLoadFloat4(&scale.__vector);
        XMStoreFloat4x4(&result.__matrix, XMMatrixScalingFromVector(s));
        return result;
    }

    inline static Matrix4x4f MatrixRotationFromQuad(const Vector4f& rotation)
    {
        Matrix4x4f result;
        XMVECTOR r = XMLoadFloat4(&rotation.__vector);
        XMStoreFloat4x4(&result.__matrix, XMMatrixRotationQuaternion(r));
        return result;
    }
}