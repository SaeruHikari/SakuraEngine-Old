#pragma once
#include "ScalarMath.h"

namespace sakura
{
    struct Rotator
    {
        /** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
        float pitch;
        /** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
        float yaw;
        /** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
        float roll;

		FORCEINLINE static float clamp_axis(float Angle)
		{
			// returns Angle in the range (-360,360)
			Angle = sakura::math::mod(Angle, 360.f);

			if (Angle < 0.f)
			{
				// shift to [0,360) range
				Angle += 360.f;
			}

			return Angle;
		}

		FORCEINLINE static float normalize_axis(float Angle)
		{
			// returns Angle in the range [0,360)
			Angle = clamp_axis(Angle);

			if (Angle > 180.f)
			{
				// shift to (-180,180]
				Angle -= 360.f;
			}

			return Angle;
		}
    };
}
