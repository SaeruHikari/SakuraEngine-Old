/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 10:21:32
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 02:34:55
 */
#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Sakura::Math
{
    using Matrix4x4d = Eigen::Matrix<double, 4, 4>;
    using Matrix3x3d = Eigen::Matrix<double, 3, 3>;
    using Matrix2x2d = Eigen::Matrix<double, 2, 2>;
    using Matrix3x4d = Eigen::Matrix<double, 3, 4>;

    using Matrix4x4f = Eigen::Matrix<float, 4, 4>;
    using Matrix3x3f = Eigen::Matrix<float, 3, 3>;
    using Matrix2x2f = Eigen::Matrix<float, 2, 2>;
    using Matrix3x4f = Eigen::Matrix<float, 3, 4>;

    using Matrix4x4i = Eigen::Matrix<std::int32_t, 4, 4>;
    using Matrix3x3i = Eigen::Matrix<std::int32_t, 3, 3>;
    using Matrix2x2i = Eigen::Matrix<std::int32_t, 2, 2>;
    using Matrix3x4i = Eigen::Matrix<std::int32_t, 3, 4>;

    using Matrix4x4u = Eigen::Matrix<std::uint32_t, 4, 4>;
    using Matrix3x3u = Eigen::Matrix<std::uint32_t, 3, 3>;
    using Matrix2x2u = Eigen::Matrix<std::uint32_t, 2, 2>;
    using Matrix3x4u = Eigen::Matrix<std::uint32_t, 3, 4>;

    using Matrix4x4h = Eigen::Matrix<half, 4, 4>;
    using Matrix3x3h = Eigen::Matrix<half, 3, 3>;
    using Matrix2x2h = Eigen::Matrix<half, 2, 2>;
    using Matrix3x4h = Eigen::Matrix<half, 3, 4>;
    
}