#pragma once
#include "SakuraSTL.hpp"

namespace sakura::vfs
{
	struct file;
	struct dir;
	struct path;
}

namespace sakura::vfs
{
	struct RuntimeCoreAPI entry
	{
		virtual ~entry() noexcept {}
		
		[[nodiscard]] virtual sakura::vfs::file* open_as_file(const char* mode) noexcept = 0;
		[[nodiscard]] virtual sakura::vfs::dir* open_as_dir() noexcept = 0;
		
		[[nodiscard]] virtual bool exists() const noexcept = 0;

		[[nodiscard]] virtual bool is_directory() const noexcept = 0;
		[[nodiscard]] virtual bool is_regular_file() const noexcept = 0;
		virtual void replace_filename(const sakura::vfs::path& p) noexcept = 0;

		[[nodiscard]] virtual size_t file_size() const noexcept = 0;
		[[nodiscard]] virtual std::time_t last_write_time() const noexcept = 0;
	};

}