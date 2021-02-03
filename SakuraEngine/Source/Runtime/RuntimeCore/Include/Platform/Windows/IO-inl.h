#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma warning (disable:4800)
#include <Windows.h>

extern "C"
{
    int64_t fsize(const char* path) 
    {
        WIN32_FILE_ATTRIBUTE_DATA info;
        BOOL r = GetFileAttributesExA(path, GetFileExInfoStandard, &info);
        if (!r) return -1;
        return ((int64_t) info.nFileSizeHigh << 32) | info.nFileSizeLow;
    }
}