/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:01:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-29 16:46:15
 */
#pragma once
#define SAKURA_MATH_DIRECTXMATH 1
#ifdef SAKURA_MATH_EIGEN
#include "GeoMath/eigen/GeoMath.h"
#elif defined(SAKURA_MATH_DIRECTXMATH)
#include "GeoMath/dxmath/GeoMath.h"
#endif
#include "Color.h"
#include "SAlgorithm.hpp"

inline static const float PI = 3.141592654f;
inline static const float TWO_PI = 6.283185307f;
inline static const float ONE_DIV_PI = 0.318309886f; 
inline static const float ONE_DIV_TWO_PI = 0.159154943f;
inline static const float PI_DIV_TWO = 1.570796327f;
inline static const float PI_DIV_FOUR = 0.785398163f;