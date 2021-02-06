#pragma once
#include "Containers/cvector.h"

#ifdef __cplusplus
#include <vector>
namespace sakura
{
#ifdef WHEN_PMR_IS_USABLE
	using std::pmr::vector;
#else
    using std::vector;
#endif
}
#endif