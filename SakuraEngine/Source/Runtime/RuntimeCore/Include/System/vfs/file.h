#pragma once
#include <cstdio>

namespace sakura::vfs
{
	enum VFS_SEEK_POS
	{
		VFS_SEEK_SET = SEEK_SET,
		VFS_SEEK_CUR = SEEK_CUR,
		VFS_SEEK_END = SEEK_END
	};

	// file interface of vfs.
	// open & close automatically with RAII.
	struct RuntimeCoreAPI file
	{
		virtual ~file() noexcept {}
		
		virtual bool valid() const = 0;

		virtual size_t read(void* buffer, size_t size, size_t count) const = 0;
		
		virtual size_t write(const void* buffer, size_t size, size_t count) = 0;

		virtual int seek(size_t offset, VFS_SEEK_POS pos) = 0;

		virtual int puts(const char* buf) = 0;

		[[nodiscard]] virtual char* gets(char* str, size_t length) const = 0;

		virtual void rewind() = 0;

		virtual size_t tell() const = 0;
		
		static constexpr auto eof = EOF;
	};
}
