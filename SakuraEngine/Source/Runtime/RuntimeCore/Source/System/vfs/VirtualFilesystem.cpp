#include <System/VirtualFileSystem.h>

sakura::unordered_map<sakura::u8string, sakura::vfs::adapter*>
	sakura::vfs::adapter::mounted_adapters;

struct sakura::vfs::entry* sakura::vfs::try_entry(const path& pth)
{
	for (auto& iter : adapter::mounted_adapters)
	{
		if(iter.second->get_symbol() == pth.adapter_name())
		{
			return iter.second->entry(pth);
		}
	}
	return nullptr;
}

sakura::vfs::file* sakura::vfs::try_open_file(const path& pth, const char* mode, bool create_missed_dirs)
{
	for (auto& iter : adapter::mounted_adapters)
	{
		if (iter.second->get_symbol() == pth.adapter_name())
		{
			return iter.second->fopen(pth, mode, create_missed_dirs);
		}
	}
	return nullptr;
}

sakura::vfs::dir* sakura::vfs::try_open_dir(const path& pth)
{
	if (auto entry = sakura::unique_ptr<sakura::vfs::entry>(try_entry(pth)); entry)
		return entry->open_as_dir();
	return nullptr;
}
