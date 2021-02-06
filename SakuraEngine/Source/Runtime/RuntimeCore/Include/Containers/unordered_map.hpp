#pragma once
#include <unordered_map>

#ifdef WHEN_PMR_IS_USABLE
#include <memory_resource>
namespace sakura
{
	using std::pmr::unordered_map;
}
#else
namespace sakura
{
	using std::unordered_map;
}
#endif