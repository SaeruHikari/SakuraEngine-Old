/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 02:12:39
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 03:08:33
 */
#pragma once
#include <Eigen/Core>
#include "Half.h"
#include "Core/CoreMinimal/SDefination.h"

namespace Math
{
    using Vector3h = Eigen::Matrix<half, 3, 1>;

    using Vector3f =  Eigen::Matrix<float, 3, 1>;
    using Vector4f = Eigen::Matrix<float, 4, 1>;

    using Vector3d = Eigen::Matrix<double, 3, 1>;
    using Vector4d = Eigen::Matrix<double, 4, 1>;

    using Vector4u = Eigen::Matrix<uint32, 4, 1>;

    using Vector4i = Eigen::Matrix<int32, 4, 1>;

    using Vector4dr = Eigen::Matrix<double, 4, 1, Eigen::RowMajor>;

    // Alias
    using double4 = Vector4d;

    inline static const float PI = 3.141592654f;
    inline static const float TWO_PI = 6.283185307f;
    inline static const float ONE_DIV_PI = 0.318309886f; 
    inline static const float ONE_DIV_TWO_PI = 0.159154943f;
    inline static const float PI_DIV_TWO = 1.570796327f;
    inline static const float PI_DIV_FOUR = 0.785398163f;
}
