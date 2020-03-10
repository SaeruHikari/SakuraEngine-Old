/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Strings of Engine.
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 17:21:30
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-07 11:10:32
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
    // __c_plus_plus 17+
    using spmr_string = std::pmr::string;
    using spmr_wstring = std::pmr::wstring;

    using sstring = std::string;
    using swstring = std::wstring;
    using sstring_view = std::string_view;
    
    namespace String
    {
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
            return std::string_view(str);
        }
        /**
         * @description: Discard old ref and transfer. 
         * @param {type} 
         * @return: 
         * @author: SaeruHikari
         */
        template<typename To, typename From,
            typename std::enable_if<
                std::is_constructible<std::string_view, To>::value &&
                std::is_constructible<std::string_view, From>::value
            >::type * = nullptr>
        To DiscardCastStringTo(From& from)
        {
            std::variant<From, To> var = std::move(from);
            return std::move(Sakura::convert<To>(var));
        }

        /**
         * @description: Copy and transfer.  
         * @param {type} 
         * @return: 
         * @author: SaeruHikari
         */
        template<typename To, typename From,
            typename std::enable_if<
                std::is_constructible<std::string_view, To>::value &&
                std::is_constructible<std::string_view, From>::value
            >::type * = nullptr>
        To CastStringTo(From& from)
        {
            std::variant<From, To> var = from;
            return std::move(Sakura::convert<To>(var));
        }
    }
}
