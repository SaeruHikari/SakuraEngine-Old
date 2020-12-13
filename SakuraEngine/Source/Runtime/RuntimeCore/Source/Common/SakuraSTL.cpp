#include <SakuraSTL.hpp>


std::string_view std::to_string_view(const sakura::string& s) noexcept
{
    return { (const char*)s.data(), s.length() };
}

std::wstring_view std::to_string_view(const sakura::wstring& s) noexcept
{
    return { s.data(), s.length() };
}

/*
std::string_view std::to_string_view(const sakura::u8string& s) noexcept
{
    return { (const char*)s.data(), s.length() };
}

sakura::string_view sakura::to_string_view(const sakura::u8string& s) noexcept
{
    return { (const char*)s.data(), s.length() };
}

sakura::string_view sakura::to_string_view(const sakura::string& s) noexcept
{
	return { (const char*)s.data(), s.length() };
}

sakura::wstring_view sakura::to_string_view(const sakura::wstring& s) noexcept
{
    return { s.data(), s.length() };
}
*/
