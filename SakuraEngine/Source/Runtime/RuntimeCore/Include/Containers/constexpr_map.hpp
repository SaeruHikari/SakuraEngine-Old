#pragma once
#include <SakuraSTL.hpp>

namespace sakura 
{
namespace detail 
{
template <typename T>
struct constexpr_hash{};

template <typename T>
constexpr void swap(T& a, T& b) noexcept
{
    T tmp{ a };
    a = b;
    b = tmp;
}

template <typename Key>
class compare_key 
{
public:
    const Key key;

    constexpr compare_key(const Key& key_) noexcept : key(key_) 
    {
    }

    template <typename Element>
    constexpr bool operator<(const Element& rhs) const noexcept 
    {
        return key < rhs->first;
    }
};

template <typename Key, typename Value>
class element 
{
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = sakura::pair<key_type, mapped_type>;
    using compare_key_type = compare_key<key_type>;

    constexpr element(const key_type& key, const mapped_type& value) noexcept 
        : pair(key, value) {}

    constexpr bool operator<(const element& rhs) const noexcept 
    {
        return pair.first < rhs.pair.first;
    }

    constexpr bool operator<(const compare_key_type& rhs) const noexcept 
    {
        return pair.first < rhs.key;
    }

    constexpr const auto& operator*() const noexcept 
    {
        return pair;
    }

    constexpr const auto* operator->() const noexcept 
    {
        return &pair;
    }

    constexpr void swap(element& rhs) noexcept 
    {
        detail::swap(pair.first, rhs.pair.first);
        detail::swap(pair.second, rhs.pair.second);
    }

private:
    value_type pair;
};

template <typename Key, typename Hasher = sakura::detail::constexpr_hash<Key>>
class compare_key_hash : public compare_key<Key> 
{
    using base_type = compare_key<Key>;

public:
    const sakura::size_t hash;

    constexpr compare_key_hash(const Key& key_) noexcept 
        : base_type(key_), hash(Hasher()(key_)) {}

    template <typename Element>
    constexpr bool operator<(const Element& rhs) const noexcept 
    {
        return hash < rhs.hash || (!(rhs.hash < hash) && base_type::operator<(rhs));
    }
};

template <typename Key, typename Value, typename Hasher = sakura::detail::constexpr_hash<Key>>
class element_hash : public element<Key, Value> 
{
    using base_type = element<Key, Value>;

public:
    using key_type = Key;
    using mapped_type = Value;
    using compare_key_type = compare_key_hash<key_type, Hasher>;

    friend compare_key_type;

    constexpr element_hash(const key_type& key, const mapped_type& value) noexcept
        : base_type(key, value), hash(Hasher()(key)) {}

    template <typename T>
    constexpr bool operator<(const T& rhs) const noexcept 
    {
        return hash < rhs.hash || (!(rhs.hash < hash) && base_type::operator<(rhs));
    }

    constexpr void swap(element_hash& rhs) noexcept 
    {
        detail::swap(hash, rhs.hash);
        base_type::swap(rhs);
    }

private:
    sakura::size_t hash;
};

} // namespace detail

template <typename Element>
class iterator {
public:
    constexpr iterator(const Element* pos_) noexcept : pos(pos_) 
    {
    }

    constexpr bool operator==(const iterator& rhs) const noexcept 
    {
        return pos == rhs.pos;
    }

    constexpr bool operator!=(const iterator& rhs) const noexcept
    {
        return pos != rhs.pos;
    }

    constexpr iterator& operator++() noexcept 
    {
        ++pos;
        return *this;
    }

    constexpr iterator& operator+=(sakura::size_t i) noexcept 
    {
        pos += i;
        return *this;
    }

    constexpr iterator operator+(sakura::size_t i) const noexcept 
    {
        return pos + i;
    }

    constexpr iterator& operator--() noexcept 
    {
        --pos;
        return *this;
    }

    constexpr iterator& operator-=(sakura::size_t i) noexcept 
    {
        pos -= i;
        return *this;
    }

    constexpr sakura::size_t operator-(const iterator& rhs) const noexcept
    {
        return pos - rhs.pos;
    }

    constexpr const auto& operator*() const noexcept 
    {
        return **pos;
    }

    constexpr const auto* operator->() const noexcept 
    {
        return &**pos;
    }

private:
    const Element* pos;
};

namespace detail {

template <typename Compare, typename Iterator, typename Key>
constexpr auto bound(Iterator left, Iterator right, const Key& key) noexcept 
{
    sakura::size_t count = right - left;
    while (count > 0) 
    {
        const sakura::size_t step = count / 2;
        right = left + step;
        if (Compare()(*right, key)) 
        {
            left = ++right;
            count -= step + 1;
        } 
        else
            count = step;
    }
    return left;
}

struct less {
    template <typename A, typename B>
    constexpr bool operator()(const A& a, const B& b) const noexcept 
    {
        return a < b;
    }
};

struct greater_equal {
    template <typename A, typename B>
    constexpr bool operator()(const A& a, const B& b) const noexcept 
    {
        return !(b < a);
    }
};

template <typename Element, sakura::size_t N>
class map_c 
{
private:
    //static_assert(N > 0, "map is empty");

    template <typename T, sakura::size_t... I>
    constexpr map_c(const T (&data)[N], std::index_sequence<I...>) noexcept
        : data_{ { data[I].first, data[I].second }... } 
    {
        static_assert(sizeof...(I) == N, "index_sequence has identical length");
        // that is already sorted (like static maps), it has a small code size, and it's stable.
        for (auto left = data_, right = data_ + N - 1; data_ < right; right = left, left = data_) 
        {
            for (auto it = data_; it < right; ++it) 
            {
                if (it[1] < it[0]) 
                {
                    it[0].swap(it[1]);
                    left = it;
                }
            }
        }
    }

    using compare_key_type = typename Element::compare_key_type;
public:
    Element data_[N];
    template <typename T>
    constexpr map_c(const T (&data)[N]) noexcept 
        : map_c(data, std::make_index_sequence<N>()) {}

    using key_type = typename Element::key_type;
    using mapped_type = typename Element::mapped_type;
    using value_type = typename Element::value_type;
    using size_type = sakura::size_t;
    using difference_type = std::ptrdiff_t;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using const_iterator = iterator<Element>;

    constexpr bool unique() const noexcept 
    {
        for (auto right = data_ + N - 1, it = data_; it < right; ++it) {
            if (!(it[0] < it[1])) {
                return false;
            }
        }
        return true;
    }

    constexpr const mapped_type& at(const key_type& key) const noexcept 
    {
        return find(key)->second;
    }

    static constexpr int size() noexcept 
    {
        return N;
    }

    constexpr const_iterator begin() const noexcept 
    {
        return data_;
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }

    constexpr const_iterator end() const noexcept 
    {
        return data_ + N;
    }

    constexpr const_iterator cend() const noexcept 
    {
        return end();
    }

    constexpr const_iterator lower_bound(const key_type& key) const noexcept 
    {
        return bound<less>(data_, data_ + N, compare_key_type{ key });
    }

    constexpr const_iterator upper_bound(const key_type& key) const noexcept 
    {
        return bound<greater_equal>(data_, data_ + N, compare_key_type{ key });
    }

    constexpr sakura::pair<const_iterator, const_iterator> equal_range(const key_type& key) const noexcept 
    {
        const compare_key_type compare_key{ key };
        auto first = bound<less>(data_, data_ + N, compare_key);
        return { first, bound<greater_equal>(first, data_ + N, compare_key) };
    }

    constexpr sakura::size_t count(const key_type& key) const noexcept 
    {
        const auto range = equal_range(key);
        return range.second - range.first;
    }

    constexpr const_iterator find(const key_type& key) const noexcept 
    {
        const compare_key_type compare_key{ key };
        auto it = bound<less>(data_, data_ + N, compare_key);
        if (it != data_ + N && greater_equal()(*it, compare_key)) {
            return it;
        } else {
            return end();
        }
    }

    constexpr bool contains(const key_type& key) const noexcept 
    {
        return find(key) != end();
    }
};

template <typename Element>
class map_c<Element, 0> 
{
public:
    constexpr map_c(std::nullptr_t npt) noexcept
    {

    }
    using compare_key_type = typename Element::compare_key_type;
    using key_type = typename Element::key_type;
    using mapped_type = typename Element::mapped_type;
    using value_type = typename Element::value_type;
    using size_type = sakura::size_t;
    using difference_type = std::ptrdiff_t;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using const_iterator = iterator<Element>;

    const Element* data_ = nullptr;
    
    constexpr bool unique() const noexcept 
    {
        return true;
    }

    constexpr const mapped_type& at(const key_type& key) const noexcept 
    {
        return find(key)->second;
    }

    static constexpr int size() noexcept 
    {
        return 0;
    }

    constexpr const_iterator begin() const noexcept 
    {
        return data_;
    }

    constexpr const_iterator cbegin() const noexcept 
    {
        return begin();
    }

    constexpr const_iterator end() const noexcept 
    {
        return data_;
    }
    constexpr const_iterator cend() const noexcept 
    {
        return end();
    }

    constexpr const_iterator lower_bound(const key_type& key) const noexcept 
    {
        return bound<less>(data_, data_, compare_key_type{ key });
    }

    constexpr const_iterator upper_bound(const key_type& key) const noexcept 
    {
        return bound<greater_equal>(data_, data_, compare_key_type{ key });
    }

    constexpr sakura::pair<const_iterator, const_iterator> equal_range(const key_type& key) const noexcept 
    {
        const compare_key_type compare_key{ key };
        auto first = bound<less>(data_, data_, compare_key);
        return { first, bound<greater_equal>(first, data_, compare_key) };
    }

    constexpr sakura::size_t count(const key_type& key) const noexcept 
    {
        const auto range = equal_range(key);
        return range.second - range.first;
    }

    constexpr const_iterator find(const key_type& key) const noexcept 
    {
        return end();
    }
    constexpr bool contains(const key_type& key) const noexcept 
    {
        return false;
    }
};

template<typename T>
struct ____map_size
{
	inline constexpr static const auto value = std::decay_t<T>::size();
};
} // namespace detail

template <typename Key, typename Value, sakura::size_t N>
static constexpr auto map_c(const sakura::pair<const Key, const Value> (&items)[N]) noexcept 
{
    return detail::map_c<detail::element<Key, Value>, N>(items);
}

template <typename Key, typename Value, sakura::size_t N>
static constexpr auto unordered_map_c(const sakura::pair<const Key, const Value> (&items)[N]) noexcept 
{
    return detail::map_c<detail::element_hash<Key, Value>, N>(items);
}

} // namespace sakura

// mapbox::sakura::string

namespace sakura {
namespace detail {

// Use different constants for 32 bit vs. 64 bit size_t
#ifdef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
constexpr std::size_t hash_offset = std::integral_constant<std::size_t, 0x811C9DC5>::value;
constexpr std::size_t hash_prime = std::integral_constant<std::size_t, 0x1000193>::value;
#else
constexpr std::size_t hash_offset =
    std::conditional_t<sizeof(std::size_t) < 8,
                       std::integral_constant<std::size_t, 0x811C9DC5>,
                       std::integral_constant<std::size_t, 0xCBF29CE484222325>>::value;
constexpr sakura::size_t hash_prime =
    std::conditional_t<sizeof(std::size_t) < 8,
                       std::integral_constant<std::size_t, 0x1000193>,
                       std::integral_constant<std::size_t, 0x100000001B3>>::value;
#endif
// FNV-1a hash
constexpr static sakura::size_t str_hash(const char* str,
    const sakura::size_t value = hash_offset) noexcept 
{
    return *str ? str_hash(str + 1, (value ^ static_cast<sakura::size_t>(*str)) * hash_prime) : value;
}

constexpr bool str_less(const char* lhs, const char* rhs) noexcept 
{
    return *lhs && *rhs && *lhs == *rhs ? str_less(lhs + 1, rhs + 1) : *lhs < *rhs;
}

constexpr bool str_equal(const char* lhs, const char* rhs) noexcept 
{
    return *lhs == *rhs && (*lhs == '\0' || str_equal(lhs + 1, rhs + 1));
}

} // namespace detail

} // namespace sakura


namespace sakura::detail 
{
    template <>
    struct constexpr_hash<std::string_view> 
    {
        constexpr sakura::size_t operator()(const std::string_view& str) const 
        {
            return ::sakura::detail::str_hash(str.data());
        }
    };
} 