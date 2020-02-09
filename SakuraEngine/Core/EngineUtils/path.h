/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 20:03:51
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 20:45:49
 */
#pragma once
#include "Core/Containers/SString.h"

using namespace Sakura;

namespace Sakura::path
{
    /**
     * @description: Return the shortest path name quivalent to path
     * @param {const spmr_string&} s
     * @return: 
     * @author: SaeruHikari
     */    
    spmr_string clean(const spmr_string& s);

    /**
     * @description: Return the shortest path name quivalent to path
     * @param {const sstring&} s
     * @return: 
     * @author: SaeruHikari
     */    
    spmr_string clean(const sstring& s);
    
    /**
     * @description: Return the shortest path name quivalent to path
     * @param {sstring_view} s
     * @return: 
     * @author: SaeruHikari
     */    
    spmr_string clean(sstring_view s);   

    template<typename T, typename... Ts, 
    typename std::enable_if<
        std::is_trivially_constructible<std::string_view, Ts...>::value
    >::type * = nullptr>
    T Sstr(const char* data)
    {
        return T(data);
    }
}