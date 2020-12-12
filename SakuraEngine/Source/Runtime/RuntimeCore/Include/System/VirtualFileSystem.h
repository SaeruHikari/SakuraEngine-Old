#pragma once
#include <System/vfs/entry.h>
#include <System/vfs/adapter.h>
#include <System/vfs/file.h>
#include <System/vfs/path.h>
#include <System/vfs/dir.h>

namespace sakura::vfs
{
	[[nodiscard]] RuntimeCoreAPI sakura::vfs::entry* try_entry(const path& pth);
	[[nodiscard]] RuntimeCoreAPI sakura::vfs::file* try_open_file(const path& pth, const char* mode, bool create_missed_dirs = false);
	[[nodiscard]] RuntimeCoreAPI sakura::vfs::dir* try_open_dir(const path& pth);
}
