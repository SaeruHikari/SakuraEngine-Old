#include "Base/GenerationalId.h"
#include <SakuraSTL.hpp>

using namespace sakura;

RuntimeCoreAPI const GenerationalId GenerationalId::UNINITIALIZED = GenerationalId();
GenerationalId::GenerationalId(uhalf_t generation, uhalf_t index)
{
	u.v.generation = generation;
	u.v.index = index;
}

GenerationalId::GenerationalId()
{
	u.storage = std::numeric_limits<sakura::size_t>::max();
}

GenerationalId::operator size_t() const
{
	return u.storage;
}

GenerationalId::operator bool() const
{
	return u.storage != std::numeric_limits<sakura::size_t>::max();
}

size_t GenerationalId::storage() const
{
	return u.storage;
}

GenerationalId::uhalf_t GenerationalId::generation() const
{
	return u.v.generation;
}

GenerationalId::uhalf_t GenerationalId::index() const
{
	return u.v.index;
}

GenerationalId::GenerationalId(uint64 storage)
{
	u.storage = storage;
}