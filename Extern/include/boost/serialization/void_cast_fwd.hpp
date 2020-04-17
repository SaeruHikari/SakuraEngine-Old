/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-04-17 01:20:32
 * @LastEditTime: 2020-04-17 04:28:39
 */
#ifndef  BOOST_SERIALIZATION_VOID_CAST_FWD_HPP
#define BOOST_SERIALIZATION_VOID_CAST_FWD_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// void_cast_fwd.hpp:   interface for run-time casting of void pointers.

// (C) Copyright 2005 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// gennadiy.rozental@tfn.com

//  See http://www.boost.org for updates, documentation, and revision history.

#include <cstddef> // NULL
#include <boost/serialization/force_include.hpp>

namespace boost {
namespace serialization {
namespace void_cast_detail{
class void_caster;
} // namespace void_cast_detail
template<class Derived, class Base>
BOOST_DLLEXPORT 
inline const void_cast_detail::void_caster & void_cast_register(
    const Derived * dnull = nullptr, 
    const Base * bnull = nullptr
) BOOST_USED;
} // namespace serialization
} // namespace boost

#endif // BOOST_SERIALIZATION_VOID_CAST_HPP
