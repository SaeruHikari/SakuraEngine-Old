/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 18:20:07
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 02:54:57
 */
#include <iostream>
#include "gtest/gtest.h"
#include "Core/CoreMinimal/SDefination.h"
#include "Core/Math/GeoMath/Vector.h"

TEST(UnitTestCore, FirstPrint)
{
    sstring OutString = "UnitTestCore FirstPrint!";
    Math::Vector3f val(1, 2, 3);
    val.dot(val);
    std::cout << OutString;
}