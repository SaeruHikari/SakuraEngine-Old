#pragma once
#include "Rotator.h"
#include "SakuraSTL.hpp"
#include "ScalarMath.h"
#include "Vector.h"

namespace sakura
{
	struct alignas(16) Quaternion : public Vector4f
	{
    public:
        constexpr Quaternion() = default;
        constexpr Quaternion(float x, float y, float z, float w)
            :Vector4f(x, y, z, w)
        {

        }
		static constexpr Quaternion identity() noexcept;

	public:
		Rotator rotator() const;
		Quaternion conjugate(void) const;
	};

	FORCEINLINE constexpr Quaternion Quaternion::identity() noexcept
	{
		return Quaternion(0, 0, 0, 1);
	}

	FORCEINLINE Rotator Quaternion::rotator() const
	{
		Rotator rot;
		return rot;
	}

	FORCEINLINE Quaternion Quaternion::conjugate() const
	{
		return Quaternion(-X, -Y, -Z, -W);
	}

}
