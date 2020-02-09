/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Strings of Engine.
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 17:21:30
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 23:56:29
 */
// Prototype from Star Engine :
// https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
// Thanks for the great idea and work !
#pragma once
#include "../CoreMinimal/SKeyWords.h"
#include <string>
#include "SVariant.h"
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

    /**
     * @description: SAutoString -> string, pmr::string, string_view
     * @author: SaeruHikari
     */
    struct SAutoString 
    {
    public:
        sinline SAutoString(sstring str)
        {
            val = std::forward<sstring>(str);
        }    
        sinline SAutoString(spmr_string pmrStr)
        {
            val = std::forward<spmr_string>(pmrStr);
        }
        sinline SAutoString(const char* cstr)
        {
            val = std::forward<spmr_string>(spmr_string(cstr));
        }
    public:
        template<typename T, 
        typename std::enable_if<
            std::is_constructible<std::string_view, T>::value
        >::type * = nullptr>
        sinline constexpr T as() const
        {
            return Sakura::convert<T>(val);
        }
    public:
        sinline SAutoString& operator=(sstring str)
        {
            this->val = std::forward<sstring>(str);
            return *this;
        }
        sinline SAutoString& operator=(spmr_string pmrStr)
        {
            this->val = std::forward<spmr_string>(pmrStr);
            return *this;
        }
        sinline SAutoString& operator=(const char* cstr)
        {
            this->val = std::forward<spmr_string>(spmr_string(cstr));
            return *this;
        }
    private:
        std::variant<sstring, spmr_string, sstring_view> val; 
    };
}
