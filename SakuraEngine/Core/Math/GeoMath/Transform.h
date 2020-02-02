/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:06:25
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 19:47:25
 */
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Sakura::Math
{
    using Rotation2d = Eigen::Rotation2D<double>;
    using Rotation2f = Eigen::Rotation2D<float>;
    using Rotation2h = Eigen::Rotation2D<half>;

    using AngleAxisd = Eigen::AngleAxis<double>;
    using AngleAxisf = Eigen::AngleAxis<float>;
    using AngleAxish = Eigen::AngleAxis<half>;

    using Translation2d = Eigen::Translation<double, 2>;
    using Translation2f = Eigen::Translation<float, 2>;
    using Translation2h = Eigen::Translation<half, 2>;

    using Translation3d = Eigen::Translation<double, 3>;
    using Translation3f = Eigen::Translation<float, 3>;
    using Translation3h = Eigen::Translation<half, 3>;

    using AlignedBox2d = Eigen::AlignedBox<double, 2>;
    using AlignedBox2f = Eigen::AlignedBox<float, 2>;
    using AlignedBox2h = Eigen::AlignedBox<half, 2>;
} // namespace Sakura::Math
