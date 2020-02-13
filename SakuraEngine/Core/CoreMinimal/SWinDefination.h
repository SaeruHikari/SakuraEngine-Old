/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Windows typedefs.
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 00:41:22
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 16:30:29
 */

#pragma once
#ifndef SAKURA_TARGET_PLATFORM_WIN32
static_assert(0, "Target platform is not Win32, check for compile flags!");
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <minwindef.h>



// TO DO: Win RT
// ...