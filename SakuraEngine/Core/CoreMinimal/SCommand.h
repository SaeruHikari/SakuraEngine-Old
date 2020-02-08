/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:58:54
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 17:00:26
 */
#pragma once
#include <stdio.h>

namespace Utility
{
    inline void Print( const char* msg ) { printf("%s", msg); }
    inline void Print( const wchar_t* msg ) { wprintf(L"%ws", msg); }
}