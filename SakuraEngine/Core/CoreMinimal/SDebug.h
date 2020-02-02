/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: CopyRight SaeruHikari
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 16:34:32
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 17:10:23
 */
#pragma once
#include <assert.h>

#ifdef DEBUG_ASSERT
#undef DEBUG_ASSERT
#endif

#define DEBUG_ASSERT assert
