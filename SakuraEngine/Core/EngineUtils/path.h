/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 20:03:51
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 00:04:32
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
}