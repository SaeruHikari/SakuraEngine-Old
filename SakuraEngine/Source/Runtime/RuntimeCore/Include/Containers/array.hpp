#pragma once
#include <array>

namespace sakura
{
    using std::array;
    namespace detail
    {
        template <typename T, std::size_t ... Is>
        constexpr std::array<T, sizeof...(Is)>
        create_array(T value, std::index_sequence<Is...>)
        {
            // cast Is to void to remove the warning: unused value
            return {{(static_cast<void>(Is), value)...}};
        }
    }

    template <typename T, std::size_t N>
    constexpr std::array<T, N> create_array(T&& value)
    {
        return detail::create_array(std::forward<T>(value), std::make_index_sequence<N>());
    }
}