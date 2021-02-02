#pragma once
#include "Base/Definitions.h"
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
#include "Platform/MacOS/Memory.inl"
#endif

