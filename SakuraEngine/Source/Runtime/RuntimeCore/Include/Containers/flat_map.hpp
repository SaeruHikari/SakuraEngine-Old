#pragma once
#include <Base/Definations.h>
#include <SakuraSTL.hpp>

namespace sakura
{
/*!
 * \brief The flat_map class implements a simple map based on sakura::vector instead of a binary tree.
 *
 */
template<typename Key, typename Value, template<class> class Hash = std::hash, typename Compare = std::equal_to<Key>>
class flat_map
{
    template<typename Hash_Type = sakura::size_t>
    struct key_data
    {
        Key         m_key;
        Hash_Type   m_hash_value;

        struct order
        {
            inline bool operator () (const key_data& left, const key_data& right)  const
            {
                return (left.m_hash_value < right.m_hash_value);
            }
            inline bool operator () (const Hash_Type& left, const key_data& right) const
            {
                return (left < right.m_hash_value);
            }
            inline bool operator () (const key_data& left, const Hash_Type& right) const
            {
                return (left.m_hash_value < right);
            }
        };
    };

    public:
        using key_data_type = key_data<>;
        using value_type = sakura::pair<const Key, Value>;
        using iterator = typename sakura::vector<Value>::iterator;
        using const_iterator = typename sakura::vector<Value>::const_iterator;
        using const_iterator_key = typename sakura::vector<key_data_type>::const_iterator;
        using iterator_key = typename sakura::vector<key_data_type>::iterator;
        using hash_type = sakura::size_t;


        flat_map() {}

    private:
        using has_type = Hash<Key>;

    public:
        iterator end()
        {
            return m_value_list.end();
        }

        const_iterator end() const
        {
            return m_value_list.end();
        }

        const_iterator cend() const
        {
            return m_value_list.cend();
        }

        void insert(value_type&& value)
        {
            insert(std::move(value.first), std::move(value.second));
        }

        void insert(const Key&& key, Value&& value)
        {
            if (find(key) != end())
                return;

            m_key_list.push_back(key_data_type{std::move(key), has_type()(key)});
            std::stable_sort(m_key_list.begin(), m_key_list.end(), typename key_data_type::order());

            auto found_key = find_key_const(key);
            if (found_key != m_key_list.cend())
            {
                const auto index = std::distance(m_key_list.cbegin(), found_key);
                m_value_list.insert(m_value_list.begin() + index, std::move(value));
            }
        }

        template<typename T>
        const_iterator find(const T& key) const
        {
            const auto hash_value = Hash<T>()(key);
            auto itr = std::lower_bound(m_key_list.begin(), m_key_list.end(),
                                        hash_value,
                                        typename key_data_type::order());
            for (; itr != m_key_list.end(); ++itr)
            {
                auto& item = *itr;
                if (item.m_hash_value != hash_value)
                    break;

                if (item.m_key == key)
                    return (m_value_list.cbegin() + std::distance(m_key_list.cbegin(), itr));;
            }
            return m_value_list.cend();
        }

        iterator find(const Key& key)
        {
            const auto itr = find_key_const(key);
            if (itr != m_key_list.end())
                return (m_value_list.begin() + std::distance(m_key_list.cbegin(), itr));
            else
                return (m_value_list.end());
        }

        const_iterator find(const Key& key) const
        {
            const auto itr = find_key_const(key);
            if (itr != m_key_list.end())
                return (m_value_list.cbegin() + std::distance(m_key_list.cbegin(), itr));
            else
                return (m_value_list.cend());
        }
#ifdef _LIBCPP_VERSION
#   if _LIBCPP_VERSION <= 3700
#       define RTTR_NO_CXX11_CONST_EREASE_SUPPORT_IN_STL 1
#   endif
#elif (RTTR_COMPILER == RTTR_COMPILER_GNUC && RTTR_COMP_VER < 490)
#   define RTTR_NO_CXX11_CONST_EREASE_SUPPORT_IN_STL 1
#endif

// older versions of gcc stl, have no support for const_iterator in sakura::vector<T>::erase(const_iterator)
#if RTTR_NO_CXX11_CONST_EREASE_SUPPORT_IN_STL
        bool erase(const Key& key)
        {
            iterator_key itr = find_key(key);
            if (itr != m_key_list.end())
            {
                auto value_itr = m_value_list.begin() + std::distance(m_key_list.begin(), itr);
                if (value_itr != m_value_list.end())
                {
                    m_key_list.erase(itr);
                    m_value_list.erase(value_itr);
                    return true;
                }
            }

            return false;
        }
#else
        bool erase(const Key& key)
        {
            const_iterator_key itr = find_key_const(key);
            if (itr != m_key_list.end())
            {
                auto value_itr = m_value_list.cbegin() + std::distance(m_key_list.cbegin(), itr);
                if (value_itr != m_value_list.cend())
                {
                    m_key_list.erase(itr);
                    m_value_list.erase(value_itr);
                    return true;
                }
            }

            return false;
        }
#endif

        void clear()
        {
            m_key_list.clear();
            m_value_list.clear();
        }

        const sakura::vector<Value>& value_data() const
        {
            return m_value_list;
        }

    private:

        const_iterator_key find_key_const(const Key& key) const
        {
            const auto hash_value = has_type()(key);
            auto itr = std::lower_bound(m_key_list.begin(), m_key_list.end(),
                                        hash_value,
                                        typename key_data_type::order());
            for (; itr != m_key_list.end(); ++itr)
            {
                auto& item = *itr;
                if (item.m_hash_value != hash_value)
                    break;

                if (Compare()(item.m_key, key))
                    return itr;
            }
            return m_key_list.end();
        }

        iterator_key find_key(const Key& key)
        {
            const auto hash_value = has_type()(key);
            auto itr = std::lower_bound(m_key_list.begin(), m_key_list.end(),
                                        hash_value,
                                        typename key_data_type::order());
            for (; itr != m_key_list.end(); ++itr)
            {
                auto& item = *itr;
                if (item.m_hash_value != hash_value)
                    break;

                if (Compare()(item.m_key, key))
                    return itr;
            }
            return m_key_list.end();
        }

    private:
        sakura::vector<key_data_type>  m_key_list;
        sakura::vector<Value>          m_value_list;
};
}