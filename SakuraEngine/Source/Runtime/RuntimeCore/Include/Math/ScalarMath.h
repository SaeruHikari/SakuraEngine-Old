#pragma once
#include "SakuraSTL.hpp"

namespace sakura::math
{
	FORCEINLINE float squire(const float v) noexcept
	{
		return v * v;
	}

	FORCEINLINE double squire(const double v) noexcept
	{
		return v * v;
	}
	
	FORCEINLINE float sqrt(const float v) noexcept
	{
		return std::sqrt(v);
	}

	FORCEINLINE double sqrt(const double v) noexcept
	{
		return std::sqrt(v);
	}

	FORCEINLINE double rsqrt(const double v) noexcept
	{
		return 1.0 / sqrt(v);
	}

	FORCEINLINE float rsqrt(const float v) noexcept
	{
		return 1.f / sqrt(v);
	}


	
	FORCEINLINE float abs(const float v) noexcept
	{
		return std::abs(v);
	}

	FORCEINLINE double abs(const double v) noexcept
	{
		return std::abs(v);
	}

	FORCEINLINE int32 abs(const int32 v) noexcept
	{
		return std::abs(v);
	}

	FORCEINLINE int64 abs(const int64 v) noexcept
	{
		return std::abs(v);
	}

	FORCEINLINE double tan(const double v) noexcept
	{
		return std::tan(v);
	}

	FORCEINLINE float tan(const float v) noexcept
	{
		return std::tan(v);
	}

	FORCEINLINE float atan(const float v) noexcept
	{
		return std::atan(v);
	}
	
	FORCEINLINE double atan(const double v) noexcept
	{
		return std::atan(v);
	}
	
	FORCEINLINE double atan(const double v0, const double v1) noexcept
	{
		return std::atan2(v0, v1);
	}

	FORCEINLINE float atan2(const float v0, const float v1) noexcept
	{
		return std::atan2(v0, v1);
	}



	FORCEINLINE float sin(const float v) noexcept
	{
		return std::sin(v);
	}

	FORCEINLINE double sin(const double v) noexcept
	{
		return std::sin(v);
	}

	FORCEINLINE float asin(const float v) noexcept
	{
		return std::asin(v);
	}

	FORCEINLINE double asin(const double v) noexcept
	{
		return std::asin(v);
	}

	FORCEINLINE float cos(const float v) noexcept
	{
		return std::cos(v);
	}

	FORCEINLINE double cos(const double v) noexcept
	{
		return std::cos(v);
	}

	FORCEINLINE float acos(const float v) noexcept
	{
		return std::acos(v);
	}

	FORCEINLINE double acos(const double v) noexcept
	{
		return std::cos(v);
	}



	// Note:  We use FASTASIN_HALF_PI instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
// It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
// For comparison:
//		HALF_PI				== 1.57079632679f == 0x3fC90FDB
//		FASTASIN_HALF_PI	== 1.5707963050f  == 0x3fC90FDA
#define FASTASIN_HALF_PI (1.5707963050f)
	/**
	* Computes the ASin of a scalar value.
	*
	* @param Value  input angle
	* @return ASin of Value
	*/
	FORCEINLINE float fast_asin(float Value)
	{
		// Clamp input to [-1,1].
		bool nonnegative = (Value >= 0.0f);
		float x = abs(Value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = sqrt(omx);
		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root;  // acos(|x|)
		// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI

	constexpr FORCEINLINE int32 trunc_to_int(float F)
	{
		return (int32)F;
	}

	/**
	 * Converts a float to an integer value with truncation towards zero.
	 * @param F		Floating point value to convert
	 * @return		Truncated integer value.
	 */
	constexpr FORCEINLINE float trunc_to_float(float F)
	{
		return (float)trunc_to_int(F);
	}
	
	FORCEINLINE float mod(float X, float Y)
	{
		if (fabsf(Y) <= 1.e-8f)
		{
			return 0.f;
		}
		const float Div = (X / Y);
		// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
		const float Quotient = fabsf(Div) < 8388608.f ? trunc_to_float(Div) : Div;
		float IntPortion = Y * Quotient;

		// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
		// For example Fmod(55.8, 9.3) would result in a very small negative value!
		if (fabsf(IntPortion) > fabsf(X))
		{
			IntPortion = X;
		}

		const float Result = X - IntPortion;
		return Result;
	}
}