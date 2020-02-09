/*
 * @Descripttion: The minmum core of the Engine.
 * @Version: Do not edit
 * @Author: SaeruHikari
 * @Date: 2020-02-02 09:36:54
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-08 22:50:41
 */
#pragma once
// Concepts
#include "SConcepts.h"
// Definations
#include "SDefination.h"
// UUID
#include "SUUID.h"
// Interfaces
#include "SInterface.h"

#define DEBUG_GAME 1
#ifdef DEBUG_GAME
#include "SCommand.h"
#endif

#if defined(DEBUG) || defined(_DEBUG)
#include "SDebug.h"
#endif

#ifndef SAKURA_TARGET_PLATFORM
//#define SAKURA_TARGET_PLATFORM_WIN32 1
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
