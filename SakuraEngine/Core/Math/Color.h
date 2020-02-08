/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:01:03
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-08 21:49:55
 */
#pragma once
#include <eigen3/Eigen/Core>

namespace Sakura::Math
{
    using Unorm = std::uint8_t;
    using Snorm = std::int8_t;
    
    using Unorm4 = Eigen::Matrix<Unorm, 4, 1>;
    using Unorm3 = Eigen::Matrix<Unorm, 3, 1>;
    using Unorm2 = Eigen::Matrix<Unorm, 2, 1>;
    using Unorm1 = Eigen::Matrix<Unorm, 1, 1>;

    using Snorm4 = Eigen::Matrix<Snorm, 4, 1>;
    using Snorm3 = Eigen::Matrix<Snorm, 3, 1>;
    using Snorm2 = Eigen::Matrix<Snorm, 2, 1>;
    using Snorm1 = Eigen::Matrix<Snorm, 1, 1>;

}
