/*
 * @Descripttion: The minmum core of the Engine.
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 09:36:54
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 11:29:43
 */
#pragma once
#include "SDefination.h"
#include "SInterface.h"

#ifndef SAKURA_TARGET_PLATFORM
#define SAKURA_TARGET_PLATFORM_WIN32 1
#define SAKURA_TARGET_PLATFORM
#endif

#ifdef SAKURA_TARGET_PLATFORM_WIN32
#include "SWinDefination.h"
#elif defined(SAKURA_TARGET_PLATFFORM_MAC)
#include "SMacDefination.h"
#elif defined(SAKURA_TARGET_PLATFORM_LINUX)
#include "SLinuxDefination.h"
#elif defined(SAKURA_TARGET_PLATFORM_UNKNOWN)
static_assert(0, "NO TARGET PLATFORM!");
#endif
