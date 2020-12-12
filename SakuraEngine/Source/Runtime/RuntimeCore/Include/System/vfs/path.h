#pragma once
#include <SakuraSTL.hpp>

namespace sakura::vfs
{
	struct adapter;
}

namespace sakura::vfs
{
	// "Engine:/../"
	// "Packed:/../"
	struct RuntimeCoreAPI path
	{
		using string_type = sakura::u8string;
		using string_view_type = sakura::u8string_view;

		path() noexcept;
		path(const sakura::u8string& p) noexcept;
		sakura::vfs::path& operator=(const string_type& p);
		path(const sakura::u8string& p, const sakura::vfs::adapter& adapter) noexcept;
		path(const sakura::u8string& p, const sakura::u8string& adapter_name) noexcept;

		static const sakura::vfs::path invalid_path;

		void set_default_adapter(sakura::vfs::adapter& adapter) noexcept;
		
		const sakura::u8string extension() const noexcept;
		bool has_extension() const noexcept;

		const sakura::u8string filename() const noexcept;
		const sakura::u8string filename_noext() const noexcept;

		sakura::u8string u8string() const noexcept;
		FORCEINLINE auto c_str() const noexcept
		{
			return pth.c_str();
		}
		sakura::u8string adapter_name() const noexcept;

		sakura::vfs::path parent_path() const noexcept;

		size_t hash_value() const noexcept;

		// setters
		sakura::vfs::path& replace_filename(const sakura::u8string& replacement) noexcept;

		bool operator==(const sakura::vfs::path& rhs) const noexcept;
		sakura::vfs::path operator/ (const sakura::vfs::path& rhs) const noexcept;
		sakura::vfs::path operator/ (const sakura::u8string& rhs) const noexcept;

		static sakura::u8string default_adpt;
	protected:
		sakura::u8string pth = u8"";
		sakura::u8string adpt = u8"NONE:";
	};
}	

inline std::ostream& operator<<(std::ostream& os, const sakura::vfs::path& path) noexcept
{
	os << path.adapter_name() << path.u8string();
	return os;
}

namespace fmt
{
	template<> struct formatter<sakura::vfs::path> : fmt::formatter<sakura::u8string>
	{
		template<typename FormatContext>
		auto format(sakura::vfs::path pth, FormatContext& ctx)
		{
			sakura::u8string result = pth.adapter_name().append(pth.u8string());
			return formatter<sakura::u8string>::format(result, ctx);
		}
	};
}

