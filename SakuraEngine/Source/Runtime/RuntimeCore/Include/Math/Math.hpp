#pragma once
#include "Fixed.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Transform.h"

#ifdef SAKURA_USE_DXMATH
#include "Math/DXMath/SakuraDXMathVector.h"
#include "Math/DXMath/SakuraDXMathQuaternion.h"
#include "Math/DXMath/SakuraDXMathTransform.h"
#endif

namespace sakura::math
{
	// Implementations

	FORCEINLINE float4x4 make_transform
	(
		const Vector3f translation,
		const Vector3f scale = Vector3f::vector_one(),
		const Quaternion quaternion = Quaternion::identity()
	)
	{
		float4x4 res;
		__matrix::store_aligned(res.data_view(), __matrix::make_transform(translation, scale, quaternion));
		return res;
	}

	FORCEINLINE float4x4 perspective_fov
	(
		float FovAngleY,
		float AspectRatio,
		float NearZ,
		float FarZ
	)
	{
		float4x4 res;
		__matrix::store_aligned(res.data_view(),
			__matrix::inverse(__matrix::perspective_fov(FovAngleY, AspectRatio, NearZ, FarZ))
		);
		return res;
	}

	FORCEINLINE float4x4 look_at_matrix
	(
		const Vector3f Eye,
		const Vector3f At
	)
	{
		float4x4 res;
		__matrix::store_aligned(res.data_view(), __matrix::look_at(Eye, At));
		return res;
	}

	FORCEINLINE float4x4 multiply
	(
		const float4x4 a,
		const float4x4 b
	)
	{
		float4x4 res;
		__matrix::store_aligned(
			res.data_view(),
			__matrix::multiply(__matrix::load_aligned(a.data_view()), __matrix::load_aligned(b.data_view()))
		);
		return res;
	}

	FORCEINLINE float4x4 inverse
	(
		const float4x4 a
	)
	{
		float4x4 res;
		__matrix::store_aligned(res.data_view(),
			__matrix::inverse(__matrix::load_aligned(a.data_view()))
		);
		return res;
	}

	FORCEINLINE float4x4 transpose
	(
		const float4x4 a
	)
	{
		float4x4 res;
		__matrix::store_aligned(res.data_view(),
			__matrix::transpose(__matrix::load_aligned(a.data_view()))
		);
		return res;
	}



	// Wrappers

	FORCEINLINE Quaternion quaternion_from_rotator
	(
		const Rotator rot
	)
	{
		Quaternion res;
		__vector::store_aligned(res.data_view(),
			__quaternion::quaternion_from_euler(rot.pitch(), rot.yaw(), rot.roll()));
		return res;
	}

	FORCEINLINE Quaternion quaternion_from_euler
	(
		const float pitch, const float yaw, const float roll
	)
	{
		Quaternion res;
		__vector::store_aligned(res.data_view(), __quaternion::quaternion_from_euler(pitch, yaw, roll));
		return res;
	}

	FORCEINLINE Quaternion quaternion_from_rotation
	(
		const float4x4 rotation
	)
	{
		Quaternion res;
		__vector::store_aligned(res.data_view(), __quaternion::quaternion_from_rotation(rotation));
		return res;
	}

	FORCEINLINE Quaternion look_at_quaternion
	(
		const Vector3f Eye,
		const Vector3f At
	)
	{
		return sakura::math::quaternion_from_rotation(sakura::math::look_at_matrix(Eye, At));
	}

	FORCEINLINE Quaternion look_at_quaternion
	(
		const Vector3f direction
	)
	{
		return look_at_quaternion(Vector3f(0.f, 0.f, 0.f), direction);
	}
}