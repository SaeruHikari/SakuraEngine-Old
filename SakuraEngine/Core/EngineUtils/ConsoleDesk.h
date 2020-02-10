/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Console Desk of Sakura, only stl stream typedef now
 * @Version: 0.0.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 00:12:37
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 01:21:46
 */
#pragma once
#ifdef SAKURA_CONSOLE_INOUT
#include <iostream>

#ifndef sout 
#define sout std::cout
#endif

#ifndef sin
#define sin  std::cin
#endif

#ifndef wsout
#define wsout std::wcout
#endif

#ifndef wsin
#define wsin std::wcin
#endif

inline void Print( const char* msg ) { printf("%s", msg); }
inline void Print( const wchar_t* msg ) { wprintf(L"%ws", msg); }

#else
// Console inout closed.

#endif