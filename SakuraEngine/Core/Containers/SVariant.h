/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:21:56
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-20 00:40:12
 */
#pragma once
#include "Core/CoreMinimal/SKeyWords.h"
#include <variant>

namespace Sakura
{
    struct VariantIndexLess {
        template<class... Ts>
        bool operator()(const std::variant<Ts...>& lhs, const std::variant<Ts...>& rhs) const noexcept {
            return lhs.index() < rhs.index();
        }
    };

    template<class T, class... Args>
    sinline void convert(const std::variant<Args...>& var, T& res) snoexcept
    {
        res = visit(([](const auto& v) {
            return T(v);
        }), var);
    }

    template<class T, class...Args>
    sinline auto convert(const std::variant<Args...>& var) snoexcept
    {
        return visit(([](const auto& v) {
            return static_cast<T>(v);
        }), var);
    }

    template<class T, class... Ts>
    constexpr bool is(const std::variant<Ts...>& var) snoexcept
    {
        return std::holds_alternative<T>(var);   
    }

    template<typename VariantType, typename T, size_t index = 0>
    constexpr size_t variant_index()
    {
        if constexpr (index == std::variant_size_v<VariantType>) 
        {
            return index;
        } 
        else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>)
        {
            return index;
        } 
        else {
            return variant_index<VariantType, T, index + 1>();
        }
    }
}