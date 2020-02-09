/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Window Interface of SakuraEngine
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:43:36
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 11:01:26
 */
#pragma once
#include "Core/CoreMinimal/CoreMinimal.h"
#include <string>

namespace Sakura
{
    struct WindowDesc
    {
        float width;
        float height;
        std::string title = "SakuraEngine Window";
    };

    Interface SWindow
    {
    public:
        template<typename T>
        static T constexpr GetWindow(SWindow wind)
        {
            return force_cast<T>(wind.ptr);
        }

        template<typename T>
        T constexpr GetWindow()
        {
            return force_cast<T>(ptr);
        }
    private:
        void* ptr = nullptr;
    };
}
