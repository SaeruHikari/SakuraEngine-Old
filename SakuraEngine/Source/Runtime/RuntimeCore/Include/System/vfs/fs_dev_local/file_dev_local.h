#pragma once
#include "../file.h"
#include <SakuraSTL.hpp>

namespace std::filesystem
{
	class path;
}

namespace sakura::vfs::dev_local
{
	class RuntimeCoreAPI file_dev_local final : public sakura::vfs::file 
	{
	public:
		file_dev_local(const std::filesystem::path&, const char* mode);
		~file_dev_local() override;
		
		virtual bool valid() const final;

		virtual size_t read(void* buffer, size_t size, size_t count) const final;
		virtual size_t write(const void* buffer, size_t size, size_t count) final;
		
		virtual int seek(size_t offset, VFS_SEEK_POS pos) final;
		virtual int puts(const char* buf) final;
		[[nodiscard]] virtual char* gets(char* str, size_t length) const final;
		virtual void rewind() final;

		virtual size_t tell() const final;
	
	protected:
		FILE* loc_file = nullptr;
	};
}