#pragma once
#include "SakuraDXMathVector.h"
#include "SakuraDXMathQuaternion.h"

namespace sakura::math::__matrix
{
	using MatrixRegister = DirectX::XMMATRIX;

	FORCEINLINE void store_aligned(sakura::span<float, 16> target, const MatrixRegister matrix)
	{
		return DirectX::XMStoreFloat4x4A(reinterpret_cast<DirectX::XMFLOAT4X4A*>(target.data()), matrix);
	}

	FORCEINLINE MatrixRegister load_aligned(const sakura::span<const float, 16> target)
	{
		return DirectX::XMLoadFloat4x4A(reinterpret_cast<const DirectX::XMFLOAT4X4A*>(target.data()));
	}
	
	FORCEINLINE MatrixRegister inverse
	(
		const MatrixRegister a
	)
	{
		return DirectX::XMMatrixInverse(nullptr, a);
	}
	
	FORCEINLINE MatrixRegister multiply
	(
		const MatrixRegister a,
		const MatrixRegister b
	)
	{
		return DirectX::XMMatrixMultiply(a, b);
	}
	
	FORCEINLINE MatrixRegister make_transform
	(
		const Vector3f translation,
		const Vector3f scale,
		const Quaternion quaternion
	)
	{
		using namespace DirectX;
		return DirectX::XMMatrixTransformation(
			g_XMZero,
			g_XMIdentityR3,
			__vector::load_float3_w0(scale.data_view()),
			g_XMZero,
			__vector::load_aligned(quaternion.data_view()),
			__vector::load_float3_w0(translation.data_view())
		);
	}

	FORCEINLINE MatrixRegister look_at
	(
		const Vector3f Eye,
		const Vector3f At
	)
	{
		return DirectX::XMMatrixLookAtLH(
			__vector::load_float3_w0(Eye.data_view()),
			__vector::load_float3_w0(At.data_view()),
			DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f)
		);
	}
}
