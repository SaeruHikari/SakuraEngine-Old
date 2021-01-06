/*
 * @Author: SaeruHikari
 * @Date: 2020-07-30 14:11:41
 * @LastEditTime: 2020-08-11 17:08:49
 * @LastEditors: Please set LastEditors
 * @Description: malloc of SakuraEngine RuntimeCore.
 * @FilePath: \SakuraEngineV3\SakuraEngine\Runtime\Core\public\malloc.h
 */ 
#pragma once
#include "Base/Definations.h"
#include <stdlib.h>

namespace sakura
{
    FORCEINLINE void* sakura_malloc(size_t size) noexcept;
    FORCEINLINE void sakura_free(void* target) noexcept;
    FORCEINLINE void* sakura_realloc(void* p, size_t newsize) noexcept;
    FORCEINLINE void* sakura_memcpy(void* dst, void const* src, const size_t size) noexcept;
}

#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "Platform/Windows/Memory.inl"
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/PlayStation/Memory.inl"
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/Web/Memory.inl"
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
static_assert(0, "Implement This!");
#endif

