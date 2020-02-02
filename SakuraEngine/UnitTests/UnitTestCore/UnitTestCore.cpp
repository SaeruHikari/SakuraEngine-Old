/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 18:20:07
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 17:52:42
 */
#include <iostream>
#include "gtest/gtest.h"
#include "Core/Core.h"

TEST(UnitTestCore, TCoreMinimal)
{
    Sakura::sstring OutString = "UnitTestCore CoreMinimal! DEBUG MSG: ";
    size_t i = 120;
    Sakura::atom u = i;
    auto msg = OutString + std::to_string(force_cast<int>(u));
    Utility::Print(msg.c_str());
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