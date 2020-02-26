/*
 * @Descripttion: CopyRight SaeruHikari
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-01 18:20:07
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-26 13:32:11
 */
#include <iostream>
#include "gtest/gtest.h"
#include "Core/Core.h"
#include "Core/EngineUtils/ConsoleDesk.h"

struct A
{
    A(float val)
        :b(val)
    {
        
    }
    A(size_t x, float y)
        :i(x), b(y)
    {
        
    }
    void Print()
    {
        std::cout << i << "  " << b << std::endl;
    }
    size_t i = 0;
    float b;
    auto index()
    {
        return i;
    }
};

struct B
{
   size_t x = 0;
   float y = 15; 
};

TEST(UnitTestCore, TCoreMinimal)
{
    Sakura::sstring OutString = "UnitTestCore CoreMinimal! DEBUG MSG: ";
    size_t i = 120;
    Sakura::atom u = i;
    auto msg = OutString + std::to_string(force_cast<int>(u));
    //Print(msg.c_str());
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

TEST(UnitTestCore, TContainerTest)
{
    std::map<std::string, int, std::less<>> testM;
    std::string key = "5";
    testM[key] = 12;
    auto val = Sakura::at(testM, key);
    std::cout << std::endl << val << std::endl;    

    std::variant<float> var{5.f};   
    A a = Sakura::convert<A>(var);
    std::variant<A, B, std::tuple<size_t, float>> varr = a;
    a.Print();

    Sakura::VariantIndexMap<std::variant<A, B, std::tuple<size_t, float>>,
     std::string> magicMap;
    B b = {0, 5.f};
    Sakura::sstring res = "Variant index magic";
    magicMap[b] = res;
    std::tuple<size_t, float> tup{0, 5.f};
    std::cout << std::endl << magicMap[tup];

    std::map<std::string, std::string, std::less<>> testMap;
    std::pmr::string p_str = "TEMPLATE";
    testMap["TEMPLATE"] = " MAGIC";
    std::string_view view = std::string_view(p_str);
    std::cout << Sakura::at(testMap, p_str) << std::endl;

    float valf = 15.f;
    Sakura::SPmrVector<float> pmrVec;
    pmrVec.push_back(valf);
}

