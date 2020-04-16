/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 13:58:16
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-04-17 03:39:17
 */
// Excellent Prototype from Star Engine :
// https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
// Thanks for the great idea and work !
#ifdef SAKURA_TARGET_PLATFORM_OSX
#include <experimental/map>
#include <experimental/unordered_map>
#endif
#include <map>
#include <unordered_map>
#include "Core/EngineUtils/SHash.h"
#include "Core/CoreMinimal/SKeyWords.h"

namespace Sakura
{
    struct StringHasher
    {
        inline std::size_t operator()(const std::string &key) const
        {
            using std::size_t;
            using std::hash;
            return Sakura::hash::hash(key, Sakura::hash::defaultseed);
        }
        
        inline std::size_t operator()(const pmr::string &key) const
        {
            using std::size_t;
            using std::hash;
            return Sakura::hash::hash(key, Sakura::hash::defaultseed);
        }
    };
    
    template<class Key, class Val>
    using SMap = std::map<Key, Val, std::less<>>;

    template<class Key, class Val,
        typename HashFunc = std::hash<Key>, typename EqualKey = std::equal_to<Key>>
    using SUnorderedMap = std::unordered_map<Key, Val, HashFunc, EqualKey>;

    template<class Key, class Val>
    using SPmrMap = pmr::map<Key, Val, std::less<>>;

    template<class Key, class Val,
        typename HashFunc = std::hash<Key>, typename EqualKey = std::equal_to<Key>>
    using SPmrUnorderedMap = pmr::unordered_map<Key, Val, HashFunc, EqualKey>;

    template<class Key, class Val>
    using SStringMap = SPmrUnorderedMap<Key, Val, StringHasher>;

    // Prototype from Star Engine :
    // https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
    struct VariantIndexLess;
    template<class K, class T>
    using VariantIndexMap = std::map<K, T, VariantIndexLess>;
    template<class K, class T>
    using PmrVariantIndexMap = pmr::map<K, T, VariantIndexLess>;

    template<class Key, class Value, class Allocator, class... Keys,
    typename std::enable_if<
        std::is_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    auto at(std::map<Key, Value, std::less<>, Allocator>& m, Keys... keys)
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
        !std::is_constructible<std::string_view, Keys...>::value
    >::type * = nullptr>
    sinline auto at(std::map<Key, Value, std::less<>, Allocator>& m,
     Keys... key)
    {
        auto iter = m.find(key...);
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::map) out of range");
        }
        return iter->second;
    }

    template<class Key, class Value, class Allocator, class KeyLike,
    typename std::enable_if<
        !std::is_constructible<std::string_view, KeyLike>::value
    >::type * = nullptr>
    sinline auto at(std::map<Key, Value, std::less<>, Allocator>& m,
     const KeyLike& key)
    {
        auto iter = m.find(key);
        if (iter == m.end())
        {
            throw std::out_of_range("at(std::map) out of range");
        }
        return iter->second;
    }
}