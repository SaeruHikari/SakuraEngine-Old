#pragma once
#include "path.h"
#include "entry.h"
#include "file.h"
#include "Containers/unordered_map.hpp"

namespace sakura::vfs
{
	// interface of virtual filesystem adapter.
	struct RuntimeCoreAPI adapter
	{
		virtual ~adapter() noexcept {}
		[[nodiscard]] virtual sakura::vfs::entry* entry(const path& pth) = 0;
		
		virtual sakura::u8string get_symbol() const = 0;
		[[nodiscard]] virtual file* fopen(const sakura::vfs::path& to_open, const char* mode, bool create_missed_dirs = false) = 0;
		
		static sakura::unordered_map<sakura::u8string, adapter*> mounted_adapters;
	};
}