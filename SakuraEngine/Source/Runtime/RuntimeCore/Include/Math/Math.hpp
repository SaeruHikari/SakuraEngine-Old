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

FORCEINLINE void vector3_cross(const float a[4], const float b[4], float r[4])
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

namespace sakura::math
{
	FORCEINLINE Quaternion quaternion_from_rotator
	(
		const Rotator rot
	)
	{
		Quaternion res;
		vector::store_aligned(res.data_view(),
			quaternion::quaternion_from_euler(rot.pitch(), rot.yaw(), rot.roll()));
		return res;
	}

	FORCEINLINE Quaternion quaternion_from_euler
	(
		const float pitch, const float yaw, const float roll
	)
	{
		Quaternion res;
		vector::store_aligned(res.data_view(), quaternion::quaternion_from_euler(pitch, yaw, roll));
		return res;
	}

	FORCEINLINE Quaternion quaternion_from_rotation
	(
		const float4x4 rotation
	)
	{
		Quaternion res;
		vector::store_aligned(res.data_view(), quaternion::quaternion_from_rotation(rotation));
		return res;
	}

	FORCEINLINE float4x4 look_at_matrix
	(
		const Vector3f Eye,
		const Vector3f At
	)
	{
		float4x4 res;
		__transform::store_aligned(res.data_view(), __transform::look_at(Eye, At));
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

	FORCEINLINE float4x4 make_transform
	(
		const Vector3f translation,
		const Vector3f scale = Vector3f::vector_one(),
		const Quaternion quaternion = Quaternion::identity()
	)
	{
		float4x4 res;
		__transform::store_aligned(res.data_view(), __transform::make_transform(translation, scale, quaternion));
		return res;
	}

	FORCEINLINE float4x4 multiply
	(
		const float4x4 a,
		const float4x4 b
	)
	{
		float4x4 res;
		__transform::store_aligned(
			res.data_view(),
			__transform::multiply(__transform::load_aligned(a.data_view()), __transform::load_aligned(b.data_view()))
		);
		return res;
	}

	FORCEINLINE float4x4 inverse
	(
		const float4x4 a
	)
	{
		float4x4 res;
		__transform::store_aligned(res.data_view(),
			__transform::inverse(__transform::load_aligned(a.data_view()))
		);
		return res;
	}
}