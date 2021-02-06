/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 *
 *
 * @Description:
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-09-22 16:22:48
 * @LastEditTime: 2020-09-22 16:56:37
 */
#include <System/VirtualFileSystem.h>
#include <System/vfs/fs_dev_local/entry_dev_local.h>
#include <System/vfs/fs_dev_local/adapter_dev_local.h>
#include <System/vfs/fs_dev_local/file_dev_local.h>
#include <System/vfs/fs_dev_local/dir_dev_local.h>

using namespace sakura::vfs::dev_local;

// adapter constructor
adapter_dev_local::adapter_dev_local()
{

}

adapter_dev_local* adapter_dev_local::get()
{
	static adapter_dev_local* singleton = nullptr;
	if (!singleton)
		singleton = new adapter_dev_local();
	if (singleton)
		return singleton;
	else
		assert(0 && "adapter_dev_local error! failed to initialize.");
	return nullptr;
}

std::filesystem::path get_dev_local_path(const sakura::vfs::path& p) noexcept
{
	assert(p.adapter_name() == adapter_dev_local::get()->get_symbol() && "path is not a dev_local adapter path!");
	sakura::u8string result = adapter_dev_local::get()->project_dir;
	auto ret = (const char*)result.append(p.u8string().c_str()).c_str();
	return ret;
}

sakura::vfs::path get_vfs_path(const std::filesystem::path& p) noexcept
{
	auto str = p.u8string();
	std::replace(str.begin(), str.end(), '\\', '/');
	sakura::u8string localStr = reinterpret_cast<const sakura::char8_t*>(str.c_str());
	auto rootSize = adapter_dev_local::get()->project_dir.size();
	return sakura::vfs::path(
		localStr.substr(rootSize, localStr.size() - rootSize),
		adapter_dev_local::get()->get_symbol()
	);
}



entry_dev_local::entry_dev_local(const std::filesystem::path& _std_path) noexcept
{
	std_entry = std::filesystem::directory_entry(_std_path);
}

entry_dev_local::~entry_dev_local()
{

}

sakura::vfs::file* entry_dev_local::open_as_file(const char* mode) noexcept
{
	return new sakura::vfs::dev_local::file_dev_local(std_entry.path(), mode);
}

sakura::vfs::dir* entry_dev_local::open_as_dir() noexcept
{
	return new sakura::vfs::dev_local::dir_dev_local(std_entry.path());
}

bool entry_dev_local::exists() const noexcept
{
	return std_entry.exists();
}

bool entry_dev_local::is_directory() const noexcept
{
	return std_entry.is_directory();
}

bool entry_dev_local::is_regular_file() const noexcept
{
	return std_entry.is_regular_file();
}

void entry_dev_local::replace_filename(const sakura::vfs::path& p) noexcept
{
	return std_entry.replace_filename(get_dev_local_path(p));
}

template <typename TP>
std::time_t to_time_t(TP tp) noexcept
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

std::time_t entry_dev_local::last_write_time() const noexcept
{
	auto t = std_entry.last_write_time();
	return to_time_t(t);
}

size_t entry_dev_local::file_size() const noexcept
{
	return std_entry.file_size();
}



// file implementations.
file_dev_local::file_dev_local(
	const std::filesystem::path& pth, const char* mode)
{
	loc_file = fopen(pth.string().c_str(), mode);
}

file_dev_local::~file_dev_local()
{
	fclose(loc_file);
}

bool file_dev_local::valid() const
{
	return loc_file != nullptr;
}

size_t file_dev_local::write(const void* buffer, size_t size, size_t count)
{
	return ::fwrite(buffer, size, count, loc_file);
}

size_t file_dev_local::read(void* buffer, size_t size, size_t count) const
{
	return ::fread(buffer, size, count, loc_file);
}

int file_dev_local::seek(size_t offset, VFS_SEEK_POS pos)
{
	return ::fseek(loc_file, static_cast<long>(offset), pos);
}

int file_dev_local::puts(const char* buf)
{
	return ::fputs(buf, loc_file);
}

char* file_dev_local::gets(char* str, size_t length) const
{
	return ::fgets(str, static_cast<int>(length), loc_file);
}

void file_dev_local::rewind()
{
	return ::rewind(loc_file);
}

size_t file_dev_local::tell() const
{
	return ::ftell(loc_file);
}

// directory implementation.
dir_dev_local::dir_dev_local(const std::filesystem::path& stdPath)
	:std_dir_(stdPath)
{
	
}

sakura::vector<sakura::vfs::path> dir_dev_local::children() const
{
	sakura::vector<sakura::vfs::path> result;
	for (auto& p : std_dir_)
	{
		result.emplace_back(get_vfs_path(*std_dir_));
	}
	return result;
}

bool dir_dev_local::create_subdir(const sakura::u8string& dirName)
{
	return std::filesystem::create_directory(
		std_dir_->path() / reinterpret_cast<const char*>(dirName.c_str())
	);
}

sakura::u8string adapter_dev_local::project_dir = u8"NONE";
void adapter_dev_local::set_project_dir(const sakura::u8string& project_directory)
{
	project_dir = project_directory;
	//mounted_adapters
}

sakura::vfs::entry* adapter_dev_local::entry(const sakura::vfs::path& pth)
{
	auto _std_entry = get_dev_local_path(pth);
	if (std::filesystem::exists(_std_entry))
	{
		entry_dev_local* local_entry = new entry_dev_local(_std_entry);
		return local_entry;
	}
	return nullptr;
}

sakura::u8string adapter_dev_local::get_symbol() const
{
	return u8"Project:";
}

sakura::vfs::file* adapter_dev_local::fopen(const sakura::vfs::path& to_open, const char* mode, bool create_missed_dirs)
{
	auto local_path = get_dev_local_path(to_open);
	if(create_missed_dirs && !std::filesystem::exists(local_path.parent_path()))
	{
		std::filesystem::create_directory(local_path.parent_path());
	}
	return new file_dev_local(local_path, mode);
}

