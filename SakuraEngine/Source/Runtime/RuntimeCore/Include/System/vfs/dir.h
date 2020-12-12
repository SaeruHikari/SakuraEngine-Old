#pragma once
#include <SakuraSTL.hpp>

namespace sakura::vfs
{
	struct RuntimeCoreAPI dir
	{
		virtual ~dir() {}
		
		virtual bool create_subdir(const sakura::u8string& dirName) = 0;
		
		[[nodiscard]] virtual sakura::vector<sakura::vfs::path> children() const = 0;
	};
}