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
 * @LastEditTime: 2020-03-29 23:00:36
 */
#pragma once
#include <stdint.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Sakura::Math
{
    struct Matrix4x4;
    struct Quaternion;
    struct Vector2;
    struct Vector3;
    struct Vector4;
}

namespace Sakura::Math
{
    struct Vector2 : XMFLOAT2
    {
        Vector2() noexcept : XMFLOAT2(0.f, 0.f) {}
        constexpr explicit Vector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
        constexpr Vector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
        explicit Vector2(_In_reads_(2) const float *pArray) noexcept : XMFLOAT2(pArray) {}
        Vector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
        Vector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
        explicit Vector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

        Vector2(const Vector2&) = default;
        Vector2& operator=(const Vector2&) = default;

        Vector2(Vector2&&) = default;
        Vector2& operator=(Vector2&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

        // Comparison operators
        bool operator == (const Vector2& V) const noexcept;
        bool operator != (const Vector2& V) const noexcept;

        // Assignment operators
        Vector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
        Vector2& operator+= (const Vector2& V) noexcept;
        Vector2& operator-= (const Vector2& V) noexcept;
        Vector2& operator*= (const Vector2& V) noexcept;
        Vector2& operator*= (float S) noexcept;
        Vector2& operator/= (float S) noexcept;

        // Unary operators
        Vector2 operator+ () const noexcept { return *this; }
        Vector2 operator- () const noexcept { return Vector2(-x, -y); }

        // Vector operations
        bool InBounds(const Vector2& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector2& V) const noexcept;
        void Cross(const Vector2& V, Vector2& result) const noexcept;
        Vector2 Cross(const Vector2& V) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector2& result) const noexcept;

        void Clamp(const Vector2& vmin, const Vector2& vmax) noexcept;
        void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept;

        // Static functions
        static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
        static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;

        static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

        static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

        static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept;
        static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept;
        static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;

        static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept;
        static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;

        static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept;
        static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;

        static void Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept;
        static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector2& v, const Matrix4x4& m, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Matrix4x4& m) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix4x4& m, _Out_writes_(count) Vector2* resultArray) noexcept;

        static void Transform(const Vector2& v, const Matrix4x4& m, Vector4& result) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix4x4& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static void TransformNormal(const Vector2& v, const Matrix4x4& m, Vector2& result) noexcept;
        static Vector2 TransformNormal(const Vector2& v, const Matrix4x4& m) noexcept;
        static void TransformNormal(_In_reads_(count) const Vector2* varray,
            size_t count, const Matrix4x4& m, _Out_writes_(count) Vector2* resultArray) noexcept;
    };
    // Binary operators
    Vector2 operator+ (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator- (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator* (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator* (const Vector2& V, float S) noexcept;
    Vector2 operator/ (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator/ (const Vector2& V, float S) noexcept;
    Vector2 operator* (float S, const Vector2& V) noexcept;

    struct Vector3 : XMFLOAT3
    {
        Vector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
        constexpr explicit Vector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
        constexpr Vector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
        explicit Vector3(_In_reads_(3) const float *pArray) noexcept : XMFLOAT3(pArray) {}
        Vector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
        Vector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
        explicit Vector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

        Vector3(const Vector3&) = default;
        Vector3& operator=(const Vector3&) = default;

        Vector3(Vector3&&) = default;
        Vector3& operator=(Vector3&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

        // Comparison operators
        bool operator == (const Vector3& V) const noexcept;
        bool operator != (const Vector3& V) const noexcept;

        // Assignment operators
        inline Vector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
        inline Vector3& operator+= (const Vector3& V) noexcept;
        inline Vector3& operator-= (const Vector3& V) noexcept;
        inline Vector3& operator*= (const Vector3& V) noexcept;
        inline Vector3& operator*= (float S) noexcept;
        inline Vector3& operator/= (float S) noexcept;

        // Unary operators
        inline Vector3 operator+ () const noexcept { return *this; }
        inline Vector3 operator- () const noexcept;

        // Vector operations
        inline bool InBounds(const Vector3& Bounds) const noexcept;

        inline float Length() const noexcept;
        inline float LengthSquared() const noexcept;

        inline float Dot(const Vector3& V) const noexcept;
        inline void Cross(const Vector3& V, Vector3& result) const noexcept;
        inline Vector3 Cross(const Vector3& V) const noexcept;

        inline void Normalize() noexcept;
        inline void Normalize(Vector3& result) const noexcept;

        inline void Clamp(const Vector3& vmin, const Vector3& vmax) noexcept;
        inline void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept;

        // Static functions
        inline static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
        inline static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

        inline static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        inline static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

        inline static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        inline static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

        inline static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        inline static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;

        inline static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        inline static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

        inline static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result) noexcept;
        inline static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;

        inline static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result) noexcept;
        inline static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;

        inline static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result) noexcept;
        inline static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;

        inline static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept;
        inline static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;

        inline static void Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept;
        inline static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;

        inline static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept;
        inline static Vector3 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        inline static void Transform(const Vector3& v, const Matrix4x4& m, Vector3& result) noexcept;
        inline static Vector3 Transform(const Vector3& v, const Matrix4x4& m) noexcept;
        inline static void Transform(_In_reads_(count) const Vector3* varray,
             size_t count, const Matrix4x4& m, _Out_writes_(count) Vector3* resultArray) noexcept;

        inline static void Transform(const Vector3& v, const Matrix4x4& m, Vector4& result) noexcept;
        inline static void Transform(_In_reads_(count) const Vector3* varray, size_t count,
            const Matrix4x4& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        inline static void TransformNormal(const Vector3& v, const Matrix4x4& m, Vector3& result) noexcept;
        inline static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) noexcept;
        inline static void TransformNormal(_In_reads_(count) const Vector3* varray,
            size_t count, const Matrix4x4& m, _Out_writes_(count) Vector3* resultArray) noexcept;
    };
    // Binary operators
    Vector3 operator+ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator- (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V, float S) noexcept;
    Vector3 operator/ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator/ (const Vector3& V, float S) noexcept;
    Vector3 operator* (float S, const Vector3& V) noexcept;


    struct Vector4 : XMFLOAT4
    {
        Vector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}    
        constexpr explicit Vector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
        constexpr Vector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        explicit Vector4(_In_reads_(4) const float *pArray) noexcept : XMFLOAT4(pArray) {}
        Vector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Vector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
        explicit Vector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Vector4(const Vector4&) = default;
        Vector4& operator=(const Vector4&) = default;

        Vector4(Vector4&&) = default;
        Vector4& operator=(Vector4&&) = default;

        operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Vector4& V) const noexcept;
        bool operator != (const Vector4& V) const noexcept;

        // Assignment operators
        Vector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Vector4& operator+= (const Vector4& V) noexcept;
        Vector4& operator-= (const Vector4& V) noexcept;
        Vector4& operator*= (const Vector4& V) noexcept;
        Vector4& operator*= (float S) noexcept;
        Vector4& operator/= (float S) noexcept;

        // Unary operators
        Vector4 operator+ () const noexcept { return *this; }
        Vector4 operator- () const noexcept;

        // Vector operations
        bool InBounds(const Vector4& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector4& V) const noexcept;
        void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
        Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector4& result) const noexcept;

        void Clamp(const Vector4& vmin, const Vector4& vmax) noexcept;
        void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept;

        // Static functions
        static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
        static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

        static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

        static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

        static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result) noexcept;
        static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;
            
        static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result) noexcept;
        static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;

        static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result) noexcept;
        static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;

        static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept;
        static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;

        static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept;
        static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Matrix4x4& m, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Matrix4x4& m) noexcept;
        static void Transform(_In_reads_(count) const Vector4* varray,
            size_t count, const Matrix4x4& m, _Out_writes_(count) Vector4* resultArray) noexcept;
    };
    // Binary operators
    Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V, float S) noexcept;
    Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator/ (const Vector4& V, float S) noexcept;
    Vector4 operator* (float S, const Vector4& V) noexcept;

    struct Quaternion : public XMFLOAT4
    {
        Quaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        Quaternion(const Vector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
        explicit Quaternion(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
        explicit Quaternion(_In_reads_(4) const float *pArray) noexcept : XMFLOAT4(pArray) {}
        Quaternion(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Quaternion(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
        explicit Quaternion(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }
        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;
        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

		// Comparison operators
		inline bool operator == (const Quaternion& q) const noexcept;
		inline bool operator != (const Quaternion& q) const noexcept;
        // Assignment operators
        Quaternion& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Quaternion& operator+= (const Quaternion& q) noexcept;
        Quaternion& operator-= (const Quaternion& q) noexcept;
        Quaternion& operator*= (const Quaternion& q) noexcept;
        Quaternion& operator*= (float S) noexcept;
        Quaternion& operator/= (const Quaternion& q) noexcept;
        // Unary operators
        Quaternion operator+ () const  noexcept { return *this; }
        Quaternion operator- () const noexcept;
        // Quaternion operations
        float Length() const noexcept;
        float LengthSquared() const noexcept;

        void Normalize() noexcept;
        void Normalize(Quaternion& result) const noexcept;

        void Conjugate() noexcept;
        void Conjugate(Quaternion& result) const noexcept;

        void Inverse(Quaternion& result) const noexcept;

        float Dot(const Quaternion& Q) const noexcept;

        // Static functions
        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
        static Quaternion CreateFromRotationMatrix(const Matrix4x4& M) noexcept;

        static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
        static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;
    };
    // Binary operators
    Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q, float S) noexcept;
    Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (float S, const Quaternion& Q) noexcept;
}