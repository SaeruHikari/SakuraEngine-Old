/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 02:12:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-29 21:37:10
 */
#pragma once
#include <Eigen/Core> 
#include <Eigen/Geometry>

namespace Sakura::Math
{
    using Vector1h = Eigen::Matrix<half, 1, 1>;
    using Vector2h = Eigen::Matrix<half, 2, 1>;
    using Vector3h = Eigen::Matrix<half, 3, 1>;
    using Vector4h = Eigen::Matrix<half, 4, 1>;

    using Vector1 = Eigen::Matrix<float, 1, 1>;
    using Vector2 = Eigen::Matrix<float, 2, 1>;
    using Vector3 = Eigen::Matrix<float, 3, 1>;
    using Vector4 = Eigen::Matrix<float, 4, 1>;

    using Vector1d = Eigen::Matrix<double, 1, 1>;
    using Vector2d = Eigen::Matrix<double, 2, 1>;
    using Vector3d = Eigen::Matrix<double, 3, 1>;
    using Vector4d = Eigen::Matrix<double, 4, 1>;

    using Vector1u = Eigen::Matrix<std::uint32_t, 1, 1>;
    using Vector2u = Eigen::Matrix<std::uint32_t, 2, 1>;
    using Vector3u = Eigen::Matrix<std::uint32_t, 3, 1>;
    using Vector4u = Eigen::Matrix<std::uint32_t, 4, 1>;

    using Vector1i = Eigen::Matrix<std::int32_t, 1, 1>;
    using Vector2i = Eigen::Matrix<std::int32_t, 2, 1>;
    using Vector3i = Eigen::Matrix<std::int32_t, 3, 1>;
    using Vector4i = Eigen::Matrix<std::int32_t, 4, 1>;

    using Vector1dr = Eigen::Matrix<double, 1, 1, Eigen::RowMajor>;
    using Vector2dr = Eigen::Matrix<double, 2, 1, Eigen::RowMajor>;
    using Vector3dr = Eigen::Matrix<double, 3, 1, Eigen::RowMajor>;
    using Vector4dr = Eigen::Matrix<double, 4, 1, Eigen::RowMajor>;

    // HLSL Alias
    using double4 = Vector4d;
    using double3 = Vector3d;
    using double2 = Vector2d;
    using double1 = Vector1d;

    using float4 = Vector4;
    using float3 = Vector3;
    using float2 = Vector2;
    using float1 = Vector1;

    using half4 = Vector4h;
    using half3 = Vector3h;
    using half2 = Vector2h;
    using half1 = Vector1h;

    using int4 = Vector4i;
    using int3 = Vector3i;
    using int2 = Vector2i;
    using int1 = Vector1i;

    using uint4 = Vector4u;
    using uint3 = Vector3u;
    using uint2 = Vector2u;
    using uint1 = Vector1u;


    //using AlignedVector3d = Eigen::AlignedVector3<double>;
    //using AlignedVector3 = Eigen::AlignedVector3<float>;
}

