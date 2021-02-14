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
#include "Platform/windows/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/prospero/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/web/IO-inl.h"
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
#include "Platform/mac/IO-inl.h"
#endif
