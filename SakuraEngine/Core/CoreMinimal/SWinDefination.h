/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Windows typedefs.
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 00:41:22
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 10:12:44
 */

#pragma once
#ifndef SAKURA_TARGET_PLATFORM_LINUX
static_assert(0, "Target platform is not Linux, check for compile flags!");
#endif

#include <windows.h>
#include <minwindef.h>


// TO DO: Win RT
// ...