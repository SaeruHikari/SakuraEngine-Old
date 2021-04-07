#pragma once
#include "Base/CDefinitions.h"
#include <stdlib.h>

extern "C"
{
    FORCEINLINE void* sakura_malloc(size_t size) noexcept;
    FORCEINLINE void sakura_free(void* target) noexcept;
    FORCEINLINE void* sakura_realloc(void* p, size_t newsize) noexcept;
    FORCEINLINE void* sakura_memcpy(void* dst, void const* src, const size_t size) noexcept;
}

#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "Platform/Windows/Memory-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/prospero/Memory-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/web/Memory-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
#include "Platform/mac/Memory-inl.h"
#endif

