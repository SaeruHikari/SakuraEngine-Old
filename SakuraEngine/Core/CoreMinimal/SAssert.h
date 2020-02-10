/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:34:32
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 21:26:29
 */
#pragma once
#include <assert.h>

#ifdef DEBUG_ASSERT
#undef DEBUG_ASSERT
#endif

#if defined(SAKURA_DEBUG_EDITOR) 
#define DEBUG_ASSERT_EDITOR(x) assert(x)
#else
#define DEBUG_ASSERT_EDITOR(x)
#endif

#ifdef DEBUG_ASSERT_GAME
#undef DEBUG_ASSERT_GAME
#endif
#if defined(SAKURA_DEBUG_GAME)
#define DEBUG_ASSERT_GAME(x) assert(x)
#else
#define DEBUG_ASSERT_EDITOR(x)
#endif

#ifdef ASSERT_RUNTIME
#undef ASSERT_RUNTIME
#endif
#define ASSERT_RUNTIME(x) assert(x)

