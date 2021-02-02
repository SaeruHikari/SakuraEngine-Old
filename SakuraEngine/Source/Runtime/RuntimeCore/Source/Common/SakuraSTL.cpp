#include <SakuraSTL.hpp>

std::string_view std::to_string_view(const sakura::string& s) noexcept
{
    return { (const char*)s.data(), s.length() };
}

std::wstring_view std::to_string_view(const sakura::wstring& s) noexcept
{
    return { s.data(), s.length() };
}

namespace allocators
{
    inline void* alignedMalloc(size_t size, size_t alignment)
    {
        const int pointerSize = sizeof(void*);
        const int requestedSize = size + alignment - 1 + pointerSize;
        void* raw = std::malloc(requestedSize);

        if (!raw)
        {
            return 0;
        }

        void* start = (uint8_t*)raw + pointerSize;
        void* aligned = (void*)(((uintptr_t)((uint8_t*)start + alignment - 1)) & ~(alignment - 1));
        *(void**)((uint8_t*)aligned - pointerSize) = raw;
        return aligned;
    }

    /**
     * \brief Free memory allocated through alignedMalloc()
     * \param aligned The memory to free
     */
    inline void alignedFree(void* aligned)
    {
        if (!aligned)
        {
            return;
        }

        void* raw = *(void**)((uint8_t*)aligned - sizeof(void*));
        std::free(raw);
    }
}
