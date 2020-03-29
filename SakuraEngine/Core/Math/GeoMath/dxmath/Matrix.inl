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
 * @Date: 2020-03-29 21:50:27
 * @LastEditTime: 2020-03-30 00:11:55
 */
#pragma once

namespace Sakura::Math
{
	inline Matrix4x4 Matrix4x4::Identity() noexcept
	{
		return Matrix4x4(
            1.f, 0, 0, 0,
			0, 1.f, 0, 0,
			0, 0, 1.f, 0,
			0, 0, 0, 1.f);
	}

	inline Matrix4x4 Matrix4x4::Zero() noexcept
	{
		return Matrix4x4(
            0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
	}

    inline Vector3 Matrix4x4::GetRow3f(uint32_t row) const noexcept
	{
		return Vector3(*((XMFLOAT3*)&m[row]));
	}

    inline Vector4 Matrix4x4::GetRow4f(uint32_t row) const noexcept
	{
		return Vector4(*((XMFLOAT4*)&m[row]));
	}

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	inline bool Matrix4x4::operator == (const Matrix4x4& M) const noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4Equal(x1, y1)
			&& XMVector4Equal(x2, y2)
			&& XMVector4Equal(x3, y3)
			&& XMVector4Equal(x4, y4)) != 0;
	}

	inline bool Matrix4x4::operator != (const Matrix4x4& M) const noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4NotEqual(x1, y1)
			|| XMVector4NotEqual(x2, y2)
			|| XMVector4NotEqual(x3, y3)
			|| XMVector4NotEqual(x4, y4)) != 0;
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	inline Matrix4x4::Matrix4x4(const XMFLOAT3X3& M) noexcept
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
	}

	inline Matrix4x4::Matrix4x4(const XMFLOAT4X3& M) noexcept
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
	}

	inline Matrix4x4& Matrix4x4::operator= (const XMFLOAT3X3& M) noexcept
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator= (const XMFLOAT4X3& M) noexcept
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
		return *this;
	}

    inline Matrix4x4 Matrix4x4::operator- () const noexcept
	{
		Matrix4x4 result;
		XMStoreFloat4x4(&result, -XMLoadFloat4x4(this));
		return result;
	}

	inline Matrix4x4& Matrix4x4::operator+= (Matrix4x4 M) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		R += RR;
		XMStoreFloat4x4(this, R);
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator-= (Matrix4x4 M) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		R -= RR;
		XMStoreFloat4x4(this, R);
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(Matrix4x4 M) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		R *= RR;
		XMStoreFloat4x4(this, R);
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator*=(float S) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		R *= S;
		XMStoreFloat4x4(this, R);
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator/=(float S) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		R /= S;
		XMStoreFloat4x4(this, R);
		return *this;
	}

	inline Matrix4x4& Matrix4x4::operator/= (const Matrix4x4& M) noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------
	inline Matrix4x4 Matrix4x4::operator+(Matrix4x4 M) noexcept
	{
		Matrix4x4 result;
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		R = R + RR;
		XMStoreFloat4x4(&result, R);
		return result;
	}

	inline Matrix4x4 Matrix4x4::operator-(Matrix4x4 M) noexcept
	{
		Matrix4x4 result;
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		R = R - RR;
		XMStoreFloat4x4(&result, R);
		return result;
	}

	inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 M) noexcept
	{
		XMMATRIX R = XMLoadFloat4x4(this);
		FXMMATRIX RR = XMLoadFloat4x4(&M);
		XMMATRIX X = XMMatrixMultiply(R, RR);
		Matrix4x4 result;
		XMStoreFloat4x4(&result, X);
		return result;
	}

	inline Matrix4x4 Matrix4x4::operator*(float S) noexcept
	{
		Matrix4x4 result;
		XMMATRIX R = XMLoadFloat4x4(this);
		R = R * S;
		XMStoreFloat4x4(&result, R);
		return result;
	}

	inline Matrix4x4 Matrix4x4::operator/(float S) noexcept
	{
		Matrix4x4 result;
		XMMATRIX R = XMLoadFloat4x4(this);
		R = R / S;
		XMStoreFloat4x4(&result, R);
		return result;
	}
	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------
	inline Matrix4x4 operator+ (const Matrix4x4& M1, const Matrix4x4& M2) noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorAdd(x1, y1);
		x2 = XMVectorAdd(x2, y2);
		x3 = XMVectorAdd(x3, y3);
		x4 = XMVectorAdd(x4, y4);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 operator- (const Matrix4x4& M1, const Matrix4x4& M2) noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorSubtract(x1, y1);
		x2 = XMVectorSubtract(x2, y2);
		x3 = XMVectorSubtract(x3, y3);
		x4 = XMVectorSubtract(x4, y4);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 operator* (const Matrix4x4& M1, const Matrix4x4& M2) noexcept
	{
		using namespace DirectX;
		XMMATRIX m1 = XMLoadFloat4x4(&M1);
		XMMATRIX m2 = XMLoadFloat4x4(&M2);
		XMMATRIX X = XMMatrixMultiply(m1, m2);

		Matrix4x4 R;
		XMStoreFloat4x4(&R, X);
		return R;
	}

	inline Matrix4x4 operator* (const Matrix4x4& M, float S) noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 operator/ (const Matrix4x4& M, float S) noexcept
	{
		using namespace DirectX;
		assert(S != 0.f);

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		float rs = 1.f / S;

		x1 = XMVectorScale(x1, rs);
		x2 = XMVectorScale(x2, rs);
		x3 = XMVectorScale(x3, rs);
		x4 = XMVectorScale(x4, rs);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 operator/ (const Matrix4x4& M1, const Matrix4x4& M2) noexcept
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 operator* (float S, const Matrix4x4& M) noexcept
	{
		using namespace DirectX;

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		Matrix4x4 R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	inline Matrix4x4 Matrix4x4::ScalingFromVector(Vector3 scale) noexcept
	{
		Matrix4x4 result;
		XMVECTOR s = XMLoadFloat3(&scale);
		XMStoreFloat4x4(&result, XMMatrixScalingFromVector(s));
		return result;
	}

	inline Matrix4x4 Matrix4x4::ScalingFromVector(Vector4 scale) noexcept
	{
		Matrix4x4 result;
		XMVECTOR s = XMLoadFloat4(&scale);
		XMStoreFloat4x4(&result, XMMatrixScalingFromVector(s));
		return result;
	}	

	inline Matrix4x4 Matrix4x4::RotationFromQuad(Vector4 rotation) noexcept
	{
		Matrix4x4 result;
		XMVECTOR r = XMLoadFloat4(&rotation);
		XMStoreFloat4x4(&result, XMMatrixRotationQuaternion(r));
		return result;
	}

	inline Vector3 Matrix4x4::GetScale() noexcept
	{
		XMVECTOR S, R, T;
		XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(this)); 
		Vector3 scale;
		XMStoreFloat3(&scale, S);
		return scale;
	}

	inline Vector4 Matrix4x4::GetRotation() noexcept 
	{
		XMVECTOR S, R, T;
		XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(this)); 
		Vector4 rotation;
		XMStoreFloat4(&rotation, R);
		return rotation;
	}
	
	inline void Matrix4x4::Translate(Vector3 value) noexcept
	{
		//value.x *= -1;
		XMVECTOR trans = XMLoadFloat3(&value);
		auto R = XMLoadFloat4x4(this);
		auto translate =  XMMatrixTranslationFromVector(trans);
		XMStoreFloat4x4(this, translate * R);
	}

	inline Matrix4x4 Matrix4x4::CreatePerspectiveFieldOfView(
            float fov, float aspectRatio, float nearPlane, float farPlane) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		auto f44 = XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
		XMStoreFloat4x4(&R, f44);
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreatePerspectiveFieldOfView_ReverseY(
            float fov, float aspectRatio, float nearPlane, float farPlane) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		auto f44 = XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
		XMStoreFloat4x4(&R, f44);
		R.m[1][1] *= -1;
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& eye,
		const Vector3& target, const Vector3& up) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMVECTOR eyev = XMLoadFloat3(&eye);
		XMVECTOR targetv = XMLoadFloat3(&target);
		XMVECTOR upv = XMLoadFloat3(&up);
		XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateWorld(const Vector3& position,
		const Vector3& forward, const Vector3& up) noexcept
	{
		using namespace DirectX;
		XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
		XMVECTOR yaxis = XMLoadFloat3(&up);
		XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
		yaxis = XMVector3Cross(zaxis, xaxis);

		Matrix4x4 R;
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
		R._14 = R._24 = R._34 = 0.f;
		R._41 = position.x; R._42 = position.y; R._43 = position.z;
		R._44 = 1.f;
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateFromQuaternion(const Quaternion& rotation) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMVECTOR quatv = XMLoadFloat4(&rotation);
		XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateScale(const Vector3& scales) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateScale(float xs, float ys, float zs) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateScale(float scale) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateRotationX(float radians) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateRotationY(float radians) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateRotationZ(float radians) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
		return R;
	}

	inline Matrix4x4 Matrix4x4::CreateFromYawPitchRoll(float yaw,
		float pitch, float roll) noexcept
	{
		using namespace DirectX;
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		return R;
	}

	inline bool Matrix4x4::Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept
	{
		using namespace DirectX;

		XMVECTOR s, r, t;

		if (!XMMatrixDecompose(&s, &r, &t, *this))
			return false;

		XMStoreFloat3(&scale, s);
		XMStoreFloat4(&rotation, r);
		XMStoreFloat3(&translation, t);

		return true;
	}

	inline Matrix4x4 Matrix4x4::Transpose() const noexcept
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4(this);
		Matrix4x4 R;
		XMStoreFloat4x4(&R, XMMatrixTranspose(M));
		return R;
	}

	inline void Matrix4x4::Transpose(Matrix4x4& result) const noexcept
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4(this);
		XMStoreFloat4x4(&result, XMMatrixTranspose(M));
	}

	inline Matrix4x4 Matrix4x4::Invert() const noexcept
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4(this);
		Matrix4x4 R;
		XMVECTOR det;
		XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
		return R;
	}

	inline void Matrix4x4::Invert(Matrix4x4& result) const noexcept
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4(this);
		XMVECTOR det;
		XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
	}

	inline float Matrix4x4::Determinant() const noexcept
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4(this);
		return XMVectorGetX(XMMatrixDeterminant(M));
	}
}
