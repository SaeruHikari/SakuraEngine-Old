#pragma once
#include <Base/Definations.h>

namespace sakura
{
	template<typename Storage, uint FractionBits>
    struct fixed
    {
	private:
        Storage storage;
    };
    using fixed64 = fixed<sakura::int64, 32>;
    using fixed32 = fixed<sakura::int32, 16>;
}