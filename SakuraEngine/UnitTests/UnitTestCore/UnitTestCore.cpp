/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 18:20:07
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 11:06:16
 */
#include <iostream>
#include "gtest/gtest.h"
#include "Core/CoreMinimal/CoreMinimal.h"
#include "Core/Math/GeoMath/Vector.h"

TEST(UnitTestCore, TCoreMinimal)
{
    Sakura::sstring OutString = "UnitTestCore CoreMinimal!";
    size_t i = 0;
    Sakura::atom u = i;
    std::cout << OutString << u << std::endl;
}

TEST(UnitTestCore, TMathTest)
{
    Sakura::Math::Vector3f vec3A(1, 2, 3);
    Sakura::Math::Vector3f vec3B(1, 0, 0);
    Sakura::Math::Vector3f vec3C(0, 2, 0);
    auto var = vec3A.dot(vec3A);
    // 1 + 4 + 9 = 14
    std::cout << "Start VectorMath UnitTest: " << std::endl;
    std::cout.setf(std::ios::boolalpha);
    std::cout << "(1, 2, 3) ^ 2: " << var << 
        " Correct: " << (var == 14) << std::endl;
    auto cross = vec3B.cross(vec3C);
    std::cout << "(1, 0, 0) cross (0, 2, 0): x: " << cross[0] 
        << " y: " << cross[1]
        << " z: " << cross[2];
}