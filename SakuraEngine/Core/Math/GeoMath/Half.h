/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 02:52:23
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-08 22:48:56
 */
#pragma once
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/src/Core/arch/CUDA/Half.h>

namespace Sakura::Math
{
    /**
     * @description: half is a 16bit float, could have better performance on GPU.
     */
    using Eigen::half;
}
