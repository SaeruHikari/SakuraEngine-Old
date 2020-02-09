/*
 * @Author: your name
 * @Date: 2020-02-09 16:02:27
 * @LastEditTime : 2020-02-09 16:39:11
 * @LastEditors  : SaeruHikari
 * @Description: In User Settings Edit
 * @FilePath: /SakuraEngine/Core/EngineUtils/Win32/time.cpp
 */
#ifndef SAKURA_TARGET_PLATFORM_WIN32
static_assert(0, "Target platform is not win32 but win32 implementation 
    is linked, Check the makefile for details");
#endif
#include "../time.h"