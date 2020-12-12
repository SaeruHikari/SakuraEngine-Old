#pragma once
#include <crossguid/guid.hpp>

namespace sakura
{
    using Guid = xg::Guid;
    inline sakura::Guid new_guid() noexcept
    {
        return xg::newGuid();
    }
}

namespace fmt
{
	template<> struct formatter<sakura::Guid> : fmt::formatter<std::string>
	{
		template<typename FormatContext>
		auto format(sakura::Guid _guid, FormatContext& ctx)
		{
            std::string result = _guid.str().c_str();
            return formatter<std::string>::format(result, ctx);
		}
	};
}
