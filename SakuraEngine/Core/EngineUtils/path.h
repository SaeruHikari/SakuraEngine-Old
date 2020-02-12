/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 20:03:51
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 13:50:09
 */
#pragma once
#include "Core/Containers/SString.h"
#include "Core/CoreMinimal/SConcepts.h"

using namespace Sakura;

namespace Sakura::path
{
    template<typename T, typename _T = typename std::remove_reference<T>::type,
        typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    constexpr _T clean(T&& s)
    {
        if(s.empty()) 
            return _T(1, '.');
        _T r(s.data(), s.size());
        size_t n = s.size();
        bool rooted = s[0] == '/';
        size_t p = rooted ? 1 : 0;
        size_t dotdot = rooted ? 1 : 0;

         for (size_t i = p; i < n;) {
            if (s[i] == '/' || (s[i] == '.' && (i+1 == n || s[i+1] == '/'))) {
                // empty or . element
                ++i;
            } else if (s[i] == '.' && s[i+1] == '.' && (i+2 == n || s[i+2] == '/')) {
            // .. element: remove to last /
                i += 2;

            if (p > dotdot) {
                // backtrack
                for (--p; p > dotdot && r[p] != '/'; --p);

            } else if (!rooted) {
                // cannot backtrack, but not rooted, so append .. element
                if (p > 0) r[p++] = '/';
                r[p++] = '.';
                r[p++] = '.';
                dotdot = p;
            }

            } else {
                // real path element, add slash if needed
                if ((rooted && p != 1) || (!rooted && p != 0)) {
                    r[p++] = '/';
                }
                // copy element until the next /
                for (; i < n && s[i] != '/'; i++) {
                    r[p++] = s[i];
                }
            }
        }
        if (p == 0) 
            return _T(1, '.');
        return r.substr(0, p);
    }    

    namespace ____
    {
        template<typename T,
        typename std::enable_if<
            std::is_constructible<std::string_view, T>::value 
        >::type * = nullptr>
        sinline spmr_string __join(const T& str)
        {
            return (spmr_string)str;
        }

        template<typename T, typename... Ts,
        typename std::enable_if<
            std::is_constructible<std::string_view, T>::value 
        >::type * = nullptr>
        sinline spmr_string __join(const T& str, const Ts&... s)
        {
            return !((spmr_string)str).empty() ? ((spmr_string)str + "/" + __join(s...))  
                : __join(s...);
        }
    }

    template<typename T, typename... Ts,
    typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    sinline spmr_string join(const T& str, const Ts&... str2)
    {
        return ____::__join(str, str2...);
    }    
    
    template<typename T, typename _T = typename std::remove_reference<T>::type,
    typename std::enable_if<
      std::is_member_function_pointer<decltype(&_T::substr)>::value
    >::type* = nullptr>
    sinline constexpr std::pair<_T,_T> split(T&& s)
    {
        size_t p = s.rfind('/');
        if(p == s.npos)
            return std::make_pair(s, _T());
        return std::make_pair(s.substr(0, p+1), s.substr(p+1));
    }

    /**
     * @description: Return the dir part of the path 
     * @param {type} 
     * @return: 
     * @author: SaeruHikari
     */
    template<typename T, typename _T = typename std::remove_reference<T>::type,
            typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    sinline constexpr _T dir(T&& s)
    {
        _T res = clean(split(s).first);
        return std::move(res);
    } 

    template<typename T, typename _T = typename std::remove_reference<T>::type,
        typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    constexpr _T base(T&& s)
    {
        if (s.empty()) 
            return _T(1, '.');

        size_t p = s.size();
        for (; p > 0; p--) {
            if (s[p - 1] != '/') break;
        }
        if (p == 0) 
            return _T(1, '/');

        sstring_view x = (p == s.size() ? s : s.substr(0, p));
        size_t c = x.rfind('/');
        if constexpr(std::is_constructible<std::string_view, T>::value)
            return c != x.npos ? (_T)x.substr(c + 1) : (_T)x;
        else 
            return c != x.npos ? x.substr(c + 1) : x;
    }

    template<typename T, typename _T = typename std::remove_reference<T>::type,
       typename std::enable_if<
        std::is_constructible<std::string_view, T>::value 
    >::type * = nullptr>
    constexpr _T ext(T&& s)
    {
        for (size_t i = s.size() - 1; i != (size_t)-1 && s[i] != '/'; --i)
        {
            if (s[i] == '.') 
                return s.substr(i);
        }
        return _T();
    }
}