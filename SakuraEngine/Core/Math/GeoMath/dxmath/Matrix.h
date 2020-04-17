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
 * @LastEditTime: 2020-04-17 04:21:37
 */
#pragma once
#include <stdint.h>
#include <assert.h>
#include <memory.h>
#include "Vector.h"

using namespace DirectX;

namespace Sakura::Math
{
    struct Matrix4x4 : XMFLOAT4X4
    {
        Matrix4x4() noexcept
            : XMFLOAT4X4(1.f, 0, 0, 0,
                            0, 1.f, 0, 0,
                            0, 0, 1.f, 0,
                            0, 0, 0, 1.f) {}
                            
        constexpr Matrix4x4(float m00, float m01, float m02, float m03,
                             float m10, float m11, float m12, float m13,
                             float m20, float m21, float m22, float m23,
                             float m30, float m31, float m32, float m33) noexcept
            : XMFLOAT4X4(m00, m01, m02, m03,
                        m10, m11, m12, m13,
                        m20, m21, m22, m23,
                        m30, m31, m32, m33) {}
        explicit Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
                            r1.x, r1.y, r1.z, 0,
                            r2.x, r2.y, r2.z, 0,
                            0, 0, 0, 1.f) {}
        explicit Matrix4x4(const Vector4& r0, const Vector4& r1,
            const Vector4& r2, const Vector4& r3) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
                            r1.x, r1.y, r1.z, r1.w,
                            r2.x, r2.y, r2.z, r2.w,
                            r3.x, r3.y, r3.z, r3.w) {}
        Matrix4x4(const XMFLOAT4X4& M) noexcept 
        {
            #ifdef _WIN32
            memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); 
            #else
            memcpy(this, &M, sizeof(XMFLOAT4X4));
            #endif
        }
        Matrix4x4(const XMFLOAT3X3& M) noexcept;
        Matrix4x4(const XMFLOAT4X3& M) noexcept;

        operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

        // Comparison operators
        bool operator == (const Matrix4x4& M) const noexcept;
        bool operator != (const Matrix4x4& M) const noexcept;

        inline static Matrix4x4 Identity() noexcept;
        inline static Matrix4x4 Zero() noexcept;
        inline static Matrix4x4 ScalingFromVector(Vector3 scale) noexcept;
        inline static Matrix4x4 ScalingFromVector(Vector4 scale) noexcept;
        inline static Matrix4x4 RotationFromQuad(Vector4 rotation) noexcept;
        inline static Matrix4x4 CreatePerspectiveFieldOfView(
            float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;

        inline static Matrix4x4 CreatePerspectiveFieldOfView_ReverseY(
            float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
        inline static Matrix4x4 CreateLookAt(const Vector3& position,
            const Vector3& target, const Vector3& up) noexcept;
        inline static Matrix4x4 CreateScale(const Vector3& scales) noexcept;
        inline static Matrix4x4 CreateScale(float xs, float ys, float zs) noexcept;
        inline static Matrix4x4 CreateScale(float scale) noexcept;
        inline static Matrix4x4 CreateRotationX(float radians) noexcept;
        inline static Matrix4x4 CreateRotationY(float radians) noexcept;
        inline static Matrix4x4 CreateRotationZ(float radians) noexcept;
        inline static Matrix4x4 CreateWorld(const Vector3& position,
            const Vector3& forward, const Vector3& up) noexcept;
        inline Matrix4x4 CreateFromQuaternion(const Quaternion& rotation) noexcept;
	    static Matrix4x4 CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

        inline Vector3 GetRow3f(uint32_t row) const noexcept;
        inline Vector4 GetRow4f(uint32_t row) const noexcept;

        // Unary operators
        inline Matrix4x4 operator+ () const noexcept { return *this; }
        inline Matrix4x4 operator- () const noexcept;

        // Assignment operators
        Matrix4x4& operator= (const XMFLOAT3X3& M) noexcept;
        Matrix4x4& operator= (const XMFLOAT4X3& M) noexcept;
        inline Matrix4x4& operator+= (Matrix4x4 M) noexcept;
        inline Matrix4x4& operator-= (Matrix4x4 M) noexcept;
        inline Matrix4x4& operator*=(Matrix4x4 M) noexcept;
        inline Matrix4x4& operator*=(float S) noexcept;
        inline Matrix4x4& operator/=(float S) noexcept;
        Matrix4x4& operator/= (const Matrix4x4& M) noexcept;

        inline Matrix4x4 operator+(Matrix4x4 M) noexcept;
        inline Matrix4x4 operator-(Matrix4x4 M) noexcept;
        inline Matrix4x4 operator*(Matrix4x4 M) noexcept;
        inline Matrix4x4 operator*(float S) noexcept;
        inline Matrix4x4 operator/(float S) noexcept;

        inline Vector3 GetScale() noexcept;
        inline Vector4 GetRotation() noexcept;
        
        // Matrix operations
        bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept;
        inline void Translate(Vector3 value) noexcept;

        Matrix4x4 Transpose() const noexcept;
        inline void Transpose(Matrix4x4& result) const noexcept;

        Matrix4x4 Invert() const noexcept;
		void Invert(Matrix4x4& result) const noexcept;
		float Determinant() const noexcept;


        inline constexpr Vector3 Up() const noexcept
        {
            return Vector3(_21, _22, _23);
        }

        inline constexpr Vector3 Down() const noexcept
        {
            return Vector3(-_21, -_22, -_23);
        }

        inline constexpr Vector3 Translation() const noexcept
        {
            return Vector3(-_41, _42, _43);
        }

        inline constexpr Vector3 Backward() const noexcept
        {
            return Vector3(-_31, -_32, -_33); 
        }
        
        inline constexpr Vector3 Forward() const noexcept
        {
            return Vector3(_31, _32, _33); 
        }

        inline constexpr Vector3 Right() const noexcept
        {
            return Vector3(_11, _12, _13); 
        }
        
        inline constexpr Vector3 Left() const noexcept
        {
            return Vector3(-_11, -_12, -_13); 
        }
    };
	// Binary operators
    Matrix4x4 operator+ (const Matrix4x4& M1, const Matrix4x4& M2) noexcept;
    Matrix4x4 operator- (const Matrix4x4& M1, const Matrix4x4& M2) noexcept;
    Matrix4x4 operator* (const Matrix4x4& M1, const Matrix4x4& M2) noexcept;
    Matrix4x4 operator* (const Matrix4x4& M, float S) noexcept;
    Matrix4x4 operator/ (const Matrix4x4& M, float S) noexcept;
    Matrix4x4 operator/ (const Matrix4x4& M1, const Matrix4x4& M2) noexcept;
	// Element-wise divide
    Matrix4x4 operator* (float S, const Matrix4x4& M) noexcept;
    
}