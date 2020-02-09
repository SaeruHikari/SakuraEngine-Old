/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Strings of Engine.
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 17:21:30
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 16:45:38
 */
// Prototype from Star Engine :
// https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
// Thanks for the great idea and work !
#pragma once
#include "../CoreMinimal/SKeyWords.h"
#include <string>
#include <memory_resource>

namespace Sakura
{
    using sstring = std::string;
    using spmr_string = std::pmr::string;
    using swstring = std::wstring;
    using spmr_wstring = std::pmr::wstring;
    using sstring_view = std::string_view;
    
    sinline sstring str(std::string_view view)
    {
        return sstring(view);
    }
    sinline sstring_view view(const std::string& str) noexcept
    {
        return std::string_view(str);
    }
    sinline sstring_view view(const std::pmr::string& str) noexcept
    {
        std::pmr::string wtf;
        return std::string_view(str);
    }
}
