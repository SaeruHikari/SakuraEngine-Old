#include <string.h>

namespace sakura
{
    FORCEINLINE void* sakura_malloc(size_t size) noexcept
    {
        return ::malloc(size);
    }

    FORCEINLINE void sakura_free(void* target) noexcept
    {
        return ::free(target);
    }

    FORCEINLINE void* sakura_realloc(void* p, size_t newsize) noexcept
    {
        return ::realloc(p, newsize);
    }

    FORCEINLINE void* sakura_memcpy(void* dst, void const* src, const size_t size) noexcept
    {
        return ::memcpy(dst, src, size);
    }
}
