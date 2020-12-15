#pragma once
#include "Rotator.h"
#include "SakuraSTL.hpp"
#include "ScalarMath.h"
#include "Vector.h"

namespace sakura
{
	struct alignas(16) Quaternion
	{
    public:
        constexpr Quaternion() = default;
        constexpr Quaternion(float x, float y, float z, float w)
            :M({ x, y, z, w })
        {

        }
		span<float, 4> data_view();
        [[nodiscard]] span<const float, 4> data_view() const;
		static constexpr Quaternion identity() noexcept;

	public:
		Rotator rotator() const;
		Quaternion conjugate(void) const;
		bool is_normalized(void) const;
		float size() const;
		float size_squared() const;
	protected:
		union
		{
			struct alignas(16)
			{
				float X;
				float Y;
				float Z;
				float W;
			};
			alignas(16) std::array<float, 4> M = { 0.f, 0.f, 0.f, 0.f };
		};
	};

	FORCEINLINE span<float, 4> Quaternion::data_view()
	{
		return M;
	}
	FORCEINLINE sakura::span<const float, 4> Quaternion::data_view() const
	{
		return M;
	}
	
	FORCEINLINE constexpr Quaternion Quaternion::identity() noexcept
	{
		return Quaternion(0, 0, 0, 1);
	}

	FORCEINLINE Rotator Quaternion::rotator() const
	{
		const float SingularityTest = - Z * X + W * Y;
		const float YawY = 2.f * (W * Z + X * Y);
		const float YawX = (1.f - 2.f * (sakura::math::squire(Y) + sakura::math::squire(Z)));

		// reference 
		// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

		// this value was found from experience, the above websites recommend different values
		// but that isn't the case for us, so I went through different testing, and finally found the case 
		// where both of world lives happily. 
		const float SINGULARITY_THRESHOLD = 0.4999995f;
		const float RAD_TO_DEG = (180.f) / 3.1415926f;
		Rotator RotatorFromQuat;

		if (SingularityTest < -SINGULARITY_THRESHOLD)
		{
			RotatorFromQuat.yaw(-90.f);
			RotatorFromQuat.roll(sakura::math::atan2(YawY, YawX) * RAD_TO_DEG);
			RotatorFromQuat.pitch(Rotator::normalize_axis(RotatorFromQuat.yaw() + (2.f * sakura::math::atan2(X, W) * RAD_TO_DEG)));
		}
		else if (SingularityTest > SINGULARITY_THRESHOLD)
		{
			RotatorFromQuat.yaw(90.f);
			RotatorFromQuat.roll(sakura::math::atan2(YawY, YawX) * RAD_TO_DEG);
			RotatorFromQuat.pitch(Rotator::normalize_axis(- RotatorFromQuat.yaw() + (2.f * sakura::math::atan2(X, W) * RAD_TO_DEG)));
		}
		else
		{
			RotatorFromQuat.yaw(sakura::math::fast_asin(2.f * (SingularityTest)) * RAD_TO_DEG);
			RotatorFromQuat.roll(sakura::math::atan2(YawY, YawX) * RAD_TO_DEG);
			RotatorFromQuat.pitch(- sakura::math::atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (sakura::math::squire(X) + sakura::math::squire(Y)))) * RAD_TO_DEG);
		}
		return RotatorFromQuat;
	}

	FORCEINLINE Quaternion Quaternion::conjugate() const
	{
		return Quaternion(-X, -Y, -Z, -W);
	}

	FORCEINLINE bool Quaternion::is_normalized() const
	{
		return sakura::math::abs(1.f - size_squared()) < 0.01f;
	}

	FORCEINLINE float Quaternion::size() const
	{
		return sakura::math::sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	FORCEINLINE float Quaternion::size_squared() const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}
}
