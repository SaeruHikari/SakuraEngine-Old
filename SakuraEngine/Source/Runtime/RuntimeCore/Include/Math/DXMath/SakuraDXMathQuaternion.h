#pragma once
#include "SakuraSTL.hpp"
#include "Math/Quaternion.h"
#include "SakuraDXMathVector.h"

#pragma warning( push )
#pragma warning ( disable : 4668 )
#include <DirectXMath.h>
#pragma warning( pop )
#include <DirectXPackedVector.h>

namespace sakura::math::quaternion
{
    using namespace sakura::math;
    FORCEINLINE vector::VectorRegister QuaternionFromRotator
    (
        const Rotator rot
    )
    {
	    DirectX::XMVECTOR Angles = DirectX::XMVectorSet(rot.pitch(), rot.yaw(), rot.roll(), 0.0f);
	    DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYawFromVector(Angles);
        return Q;
    }
}