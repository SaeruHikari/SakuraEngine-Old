#pragma once
#include "Containers/span.hpp"

namespace sakura
{
    template<typename T, size_t R, size_t L>
    struct Matrix 
    {
        sakura::span<T, R*L> data_view()
        {
            return M2;
        }
    	const sakura::span<const T, R*L> data_view() const
        {
            return M2;
        }
    protected:
    	union
    	{
            float M[R][L];
            float M2[R * L];
    	};
    };

    template<> 
    struct alignas(16) Matrix<float, 4, 4>
    {
        FORCEINLINE Matrix() = default;
        FORCEINLINE Matrix(const float* data);

        sakura::span<float, 16> data_view()
        {
            return M16;
        }
        const sakura::span<const float, 16> data_view() const
        {
            return M16;
        }

    	union
        {
            alignas(16) float M[4][4] = {
		      { 1.f, 0.f, 0.f, 0.f },
		      { 0.f, 1.f, 0.f, 0.f },
		      { 0.f, 0.f, 1.f, 0.f },
		      { 0.f, 0.f, 0.f, 1.f }
		    };
            alignas(16) float M16[16];
        };

    };
    using Matrix4x4 = Matrix<float, 4, 4>;
    using float4x4 = Matrix4x4;
    static_assert(16 == alignof(float4x4), "matrix: alignas error.");
    static_assert(sizeof(float4x4) ==  sizeof(float) * 16, "matrix: size error.");

    FORCEINLINE Matrix<float, 4, 4>::Matrix(const float* data)
    {
        M[0][0] = data[0 * 4 + 0];M[1][0] = data[1 * 4 + 0];M[2][0] = data[2 * 4 + 0];M[3][0] = data[3 * 4 + 0];
        M[0][1] = data[0 * 4 + 1];M[1][1] = data[1 * 4 + 1];M[2][1] = data[2 * 4 + 1];M[3][1] = data[3 * 4 + 1];
        M[0][2] = data[0 * 4 + 2];M[1][2] = data[1 * 4 + 2];M[2][2] = data[2 * 4 + 2];M[3][2] = data[3 * 4 + 2];
        M[0][3] = data[0 * 4 + 3];M[1][3] = data[1 * 4 + 3];M[2][3] = data[2 * 4 + 3];M[3][3] = data[3 * 4 + 3];
    }

}
