#include <filesystem>
#include <iosfwd>
#include <System/vfs/adapter.h>
#include <System/vfs/path.h>

sakura::u8string sakura::vfs::path::default_adpt = u8"Project:";

sakura::vfs::path::path(const sakura::u8string& p) noexcept
	:pth(p), adpt(default_adpt)
{

}

sakura::vfs::path& sakura::vfs::path::operator=(const string_type& p)
{
	sakura::vfs::path res(p);
	*this = res;
	return *this;
}

sakura::vfs::path::path(const sakura::u8string& p, const sakura::vfs::adapter& adapter) noexcept
	: pth(p), adpt(adapter.get_symbol())
{
	
}

const sakura::vfs::path sakura::vfs::path::invalid_path = sakura::vfs::path(u8"/INVALID PATH", u8"NONE: ");

sakura::vfs::path::path() noexcept
{
	*this = sakura::vfs::path::invalid_path;
}

sakura::vfs::path::path(const sakura::u8string& p, const sakura::u8string& adapter_name) noexcept
	: pth(p), adpt(adapter_name)
{

}

void sakura::vfs::path::set_default_adapter(sakura::vfs::adapter& adapter) noexcept
{
	default_adpt = adapter.get_symbol();
}

const sakura::u8string sakura::vfs::path::extension() const noexcept
{
	auto pos = pth.find_last_of(u8'.');
	if(pos != pth.npos)
		return pth.substr(pos);
	return sakura::u8string();
}

bool sakura::vfs::path::has_extension() const noexcept
{
	return !extension().empty();
}

sakura::u8string sakura::vfs::path::u8string() const noexcept
{
	return pth;
}

sakura::u8string sakura::vfs::path::adapter_name() const noexcept
{
	return adpt;
}

sakura::vfs::path sakura::vfs::path::parent_path() const noexcept
{
	sakura::vfs::path result = *this;
	auto iPos = pth.find_last_of(u8'/');
	if (iPos != 0)
		result.pth = pth.substr(0, iPos);
	else//Root
		result.pth = u8"/";
	return result;
}

const sakura::u8string sakura::vfs::path::filename() const noexcept
{
	// We do not support windows-style '\\'
	auto iPos = pth.find_last_of(u8'/') + 1;
	return pth.substr(iPos, pth.length() - iPos);
}

const sakura::u8string sakura::vfs::path::filename_noext() const noexcept
{
	auto _filename = filename();
	sakura::u8string res = _filename.substr(0, _filename.find_last_of(u8'.'));
	return res;
}

sakura::vfs::path sakura::vfs::path::operator/ (const sakura::u8string& rhs) const noexcept
{
	sakura::vfs::path result_path = *this;
	sakura::u8string result(pth.cbegin(), pth.cend());
	result_path.pth =
		result.append(u8"/")
		.append(sakura::u8string(rhs.cbegin(), rhs.cend()));
	return result_path;
}



sakura::vfs::path sakura::vfs::path::operator/ (const sakura::vfs::path& rhs) const noexcept
{
	sakura::vfs::path result_path = *this;
	sakura::u8string result(pth.cbegin(), pth.cend());
	result_path.pth = 
		result.append(u8"/")
			.append(sakura::u8string(rhs.pth.cbegin(), rhs.pth.cend()));
	return result_path;
}

sakura::vfs::path& sakura::vfs::path::replace_filename(
	const sakura::u8string& replacement) noexcept
{
	sakura::string_replace(pth, filename(), replacement);
	return *this;
}

bool sakura::vfs::path::operator==(const sakura::vfs::path& rhs) const noexcept
{
	return adpt == rhs.adpt && pth == rhs.pth;
}

namespace sakura
{
	template<typename T0, typename T1> struct two_param_hash {};
	template<> struct two_param_hash<sakura::u8string, sakura::u8string>
	{
		size_t operator()(const sakura::u8string& x, const sakura::u8string& y) const
		{
			u8string::const_iterator p = x.cbegin();
			u8string::const_iterator end = x.cend();
			uint32_t result = 2166136261U; // We implement an FNV-like string hash.
			while (p != end)
				result = (result * 16777619) ^ (uint8_t)*p++;
			u8string::const_iterator p2 = y.cbegin();
			u8string::const_iterator end2 = y.cend();
			while (p2 != end2)
				result = (result * 16777619) ^ (uint8_t)*p2++;
			return (size_t)result;
		}
	};
}

size_t sakura::vfs::path::hash_value() const noexcept
{
	sakura::two_param_hash<sakura::u8string, sakura::u8string> haser;
	return haser(adpt, pth);
}
