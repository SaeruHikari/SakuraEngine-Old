#pragma once
#include "Base/Definations.h"
#include <string>
#include <string_view>

#ifdef WHEN_PMR_IS_USABLE
#include <memory_resource>
namespace sakura
{
    using std::pmr::string;
    using std::pmr::wstring;
}
#else
namespace sakura
{
    using string = std::string;
    using wstring = std::wstring;
}
#endif
namespace sakura
{
    using std::string_view;
    using std::to_string;
    using std::to_wstring;

    using u8string = std::string;
    using u8string_view = std::string_view;

    template<typename string_type>
    void string_replace(
        string_type& src,
        const string_type& to_replace, const string_type& replacement)
    {
        typename string_type::size_type pos = 0;
        typename string_type::size_type srclen = to_replace.size();
        typename string_type::size_type dstlen = replacement.size();

        while ((pos = src.find(to_replace, pos)) != string_type::npos)
        {
            src.replace(pos, srclen, replacement);
            pos += dstlen;
        }
    }
}