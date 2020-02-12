/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 02:52:23
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 02:34:35
 */
#pragma once
#include <Eigen/Core>
#include <Eigen/src/Core/arch/CUDA/Half.h>

namespace Sakura::Math
{
    /**
     * @description: half is a 16bit float, could have better performance on GPU.
     */
    using Eigen::half;
}
