#pragma once
#include "AllocatorBase.h"

namespace sakura
{
	class c_allocator : public allocator {
	public:
		c_allocator();

		virtual ~c_allocator() override;

		virtual void* allocate(const std::size_t size, const std::size_t alignment = 0) override;

		virtual void free(void* ptr) override;

		virtual void init() override;
	};

}

