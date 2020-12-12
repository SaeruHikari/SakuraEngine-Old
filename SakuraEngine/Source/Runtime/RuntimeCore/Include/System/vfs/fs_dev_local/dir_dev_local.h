#pragma once
#include "System/vfs/dir.h"
#include <sys/types.h>

namespace sakura::vfs::dev_local
{
	class RuntimeCoreAPI dir_dev_local final : public sakura::vfs::dir
	{
	public:
		dir_dev_local(const std::filesystem::path& stdPath);
		
		[[nodiscard]] sakura::vector<sakura::vfs::path> children() const override;
		
		bool create_subdir(const sakura::u8string& dirName) override;
		
	protected:
		std::filesystem::directory_iterator std_dir_;
	};


}
