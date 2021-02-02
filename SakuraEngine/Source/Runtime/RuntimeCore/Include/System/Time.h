#pragma once
#include <ctime>
#include <Containers/string.hpp>

namespace sakura
{
    using time_t = std::time_t;
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
