#pragma once
#include "../entry.h"
#include <filesystem>

namespace sakura::vfs::dev_local
{
	class entry_dev_local final : public sakura::vfs::entry
	{
	public:
		entry_dev_local(const std::filesystem::path& std_path) noexcept;
		~entry_dev_local() override;
		
		[[nodiscard]] virtual sakura::vfs::file* open_as_file(const char* mode) noexcept override;
		[[nodiscard]] virtual sakura::vfs::dir* open_as_dir() noexcept override;
		
		virtual bool exists() const noexcept override;
		
		virtual bool is_directory() const noexcept override;
		virtual bool is_regular_file() const noexcept override;
		virtual void replace_filename(const sakura::vfs::path& p) noexcept override;

		virtual std::time_t last_write_time() const noexcept override;
		virtual size_t file_size() const noexcept override;
		
	protected:
		entry_dev_local() = default;
	protected:
		std::filesystem::directory_entry std_entry;
	};
}