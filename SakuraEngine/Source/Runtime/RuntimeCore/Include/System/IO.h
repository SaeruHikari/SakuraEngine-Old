#pragma once
#include "Base/CDefinitions.h"
#include <stdio.h>

extern "C"
{
    int64_t fsize(const char* path);
}

#ifdef __cplusplus

#endif

#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "Platform/Windows/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/PlayStation/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/Web/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
#include "Platform/MacOS/IO-inl.h"
#endif
