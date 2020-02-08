/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 19:09:50
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-08 19:11:07
 */
#pragma once
#include "Core/CoreMinimal/SKeyWords.h"
#include <unordered_map>
#include <memory_resource>

namespace Sakura
{
    template<class Key, class Val>
    using SUnorderedMap = std::unordered_map<Key, Val, std::less<>>;

    template<class Key, class Val>
    using SPmrUnorderedMap = std::pmr::unordered_map<Key, Val, std::less<>>;

    template<class Key, class Value, class Allocator, class... Keys,
    typename std::enable_if<
        std::is_trivially_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    sinline typename std::unordered_map<Key, Value, std::less<>, Allocator>::mapped_type&
    at(std::unordered_map<Key, Value, std::less<>, Allocator>& m, Keys... keys)
    {
        auto iter = m.find(std::string_view(keys...));
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::unordered_map) out of range");
        }
        return iter->second;
    }

    template<class Key, class Value, class Allocator, class... Keys,
    typename std::enable_if<
        std::is_trivially_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    sinline typename std::unordered_map<Key, Value, std::less<>, Allocator>::mapped_type const&
    at(std::unordered_map<Key, Value, std::less<>, Allocator>& m, Keys... keys)
    {
        auto iter = m.find(std::string_view(keys...));
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::unordered_map) out of range");
        }
        return iter->second;
    }

    template<class Key, class Value, class Allocator, class KeyLike>
    sinline auto at(std::unordered_map<Key, Value, std::less<>, Allocator>& m, const KeyLike& key)
    {
        auto iter = m.find(key);
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::unordered_map) out of range");
        }
        return iter->second;
    }

}
