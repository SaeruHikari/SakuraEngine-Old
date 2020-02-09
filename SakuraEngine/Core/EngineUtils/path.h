/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 20:03:51
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 01:08:50
 */
#pragma once
#include "Core/Containers/SString.h"

using namespace Sakura;

namespace Sakura::path
{
    /**
     * @description: Return the shortest path name quivalent to path
     * @param {sstring_view} s
     * @return: 
     * @author: SaeruHikari
     */    
    spmr_string clean(sstring_view s);

    /**
     * @description: Return the shortest path name quivalent to path
     * @param {const spmr_string&} s
     * @return: 
     * @author: SaeruHikari
     */    
    sinline spmr_string clean(const spmr_string& s)
    {
        return clean(sstring_view(s));
    };   
    /**
     * @description: Return the shortest path name quivalent to path
     * @param {const sstring&} s
     * @return: 
     * @author: SaeruHikari
     */    
    sinline spmr_string clean(const sstring& s)
    {
        return clean(sstring_view(s));
    }

    template<typename T,
    typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    sinline spmr_string __join(const T& str)
    {
        return (spmr_string)str;
    }

    template<typename T, typename... Ts,
    typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    sinline spmr_string __join(const T& str, const Ts&... s)
    {
        return !((spmr_string)str).empty() ? ((spmr_string)str + "/" + __join(s...))  
            : __join(s...);
    }

    template<typename T, typename... Ts,
    typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    sinline spmr_string join(const T& str, const Ts&... str2)
    {
        return __join(str, str2...);
    }    
}