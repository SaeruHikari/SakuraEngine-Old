#pragma once
#include "../adapter.h"

namespace sakura::vfs::dev_local
{
	class RuntimeCoreAPI adapter_dev_local final : public sakura::vfs::adapter
	{
	public:
		static void set_project_dir(const sakura::u8string& project_directory);
		static adapter_dev_local* get();
		[[nodiscard]] virtual sakura::vfs::entry* entry(const sakura::vfs::path& pth) final;
		virtual sakura::u8string get_symbol() const final;

		[[nodiscard]] file* fopen(const sakura::vfs::path& to_open, const char* mode, bool create_missed_dirs = false) override;

		static sakura::u8string project_dir;
	protected:
		adapter_dev_local();
	};
}