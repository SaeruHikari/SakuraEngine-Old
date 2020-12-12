#pragma once
#include <SakuraSTL.hpp>

namespace sakura
{
    struct RuntimeCoreAPI DateTime
    {
        int tm_sec;   // seconds after the minute - [0, 60] including leap second
        int tm_min;   // minutes after the hour - [0, 59]
        int tm_hour;  // hours since midnight - [0, 23]
        int tm_mday;  // day of the month - [1, 31]
        int tm_mon;   // months since January - [0, 11]
        int tm_year;  // years since 1900

        [[nodiscard]] static sakura::DateTime now() noexcept;
        [[nodiscard]] bool operator<(const sakura::DateTime& rhs) const noexcept;
        [[nodiscard]] bool operator==(const sakura::DateTime& rhs) const noexcept;
    	[[nodiscard]] bool operator!=(const sakura::DateTime& rhs) const noexcept;
        [[nodiscard]] sakura::string to_string(void) const noexcept;
    };

    [[nodiscard]] sakura::time_t RuntimeCoreAPI now(void) noexcept;
    [[nodiscard]] sakura::DateTime RuntimeCoreAPI to_date_time(const sakura::time_t* time_t) noexcept;
    [[nodiscard]] sakura::string RuntimeCoreAPI date_time_string(const DateTime& dt) noexcept;
}

namespace fmt
{
	template<> struct formatter<sakura::DateTime> : fmt::formatter<std::string>
	{
		template<typename FormatContext>
		auto format(sakura::DateTime dt, FormatContext& ctx)
		{
            std::string result = sakura::date_time_string(dt).c_str();
            return formatter<std::string>::format(result, ctx);
		}
	};
}
