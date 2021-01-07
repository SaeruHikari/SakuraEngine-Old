#pragma once
#include "SakuraSTL.hpp"
#include "Math/Quaternion.h"
#include "SakuraDXMathVector.h"

#pragma warning( push )
#pragma warning ( disable : 4668 )
#include <DirectXMath.h>
#pragma warning( pop )
#include <DirectXPackedVector.h>

namespace sakura::math::__quaternion
{
    using namespace sakura::math;
    using MatrixRegister = DirectX::XMMATRIX;

	FORCEINLINE __vector::VectorRegister quaternion_from_euler
	(
		const float pitch, const float yaw, const float roll
	)
	{
		DirectX::XMVECTOR Angles = DirectX::XMVectorSet(pitch, yaw, roll, 0.0f);
		DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYawFromVector(Angles);
		return Q;
	}

    FORCEINLINE __vector::VectorRegister quaternion_from_rotation
    (
        const sakura::float4x4 rotation
    )
    {
        DirectX::XMMATRIX rot = DirectX::XMLoadFloat4x4A(
			reinterpret_cast<const DirectX::XMFLOAT4X4A*>(rotation.data_view().data()));
        return DirectX::XMQuaternionRotationMatrix(rot);
    }

	FORCEINLINE __vector::VectorRegister quaternion_from_axis
	(
		__vector::VectorRegister axis, const float angle
	)
	{
		DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationAxis(axis, angle);
		return Q;
	}

}