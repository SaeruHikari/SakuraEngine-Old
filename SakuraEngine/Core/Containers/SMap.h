/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 13:58:16
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-08 19:10:58
 */
// Prototype from Star Engine :
// https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
// Thanks for the great idea and work !
#include "Core/CoreMinimal/SKeyWords.h"
#include <map>
#include <memory_resource>

namespace Sakura
{
    template<class Key, class Val>
    using SMap = std::map<Key, Val, std::less<>>;

    template<class Key, class Val>
    using SPmrMap = std::pmr::map<Key, Val, std::less<>>;

    // Prototype from Star Engine :
    // https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
    struct VariantIndexLess;
    template<class K, class T>
    using VariantIndexMap = std::map<K, T, VariantIndexLess>;
    template<class K, class T>
    using PmrVariantIndexMap = std::pmr::map<K, T, VariantIndexLess>;

    template<class Key, class Value, class Allocator, class... Keys,
    typename std::enable_if<
        std::is_trivially_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    sinline typename std::map<Key, Value, std::less<>, Allocator>::mapped_type&
    at(std::map<Key, Value, std::less<>, Allocator>& m, Keys... keys)
    {
        auto iter = m.find(std::string_view(keys...));
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::map) out of range");
        }
        return iter->second;
    }

    template<class Key, class Value, class Allocator, class... Keys,
    typename std::enable_if<
        std::is_trivially_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    sinline typename std::map<Key, Value, std::less<>, Allocator>::mapped_type const&
    at(std::map<Key, Value, std::less<>, Allocator>& m, Keys... keys)
    {
        auto iter = m.find(std::string_view(keys...));
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::map) out of range");
        }
        return iter->second;
    }

    template<class Key, class Value, class Allocator, class KeyLike>
    sinline auto at(std::map<Key, Value, std::less<>, Allocator>& m, const KeyLike& key)
    {
        auto iter = m.find(key);
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::map) out of range");
        }
        return iter->second;
    }

}