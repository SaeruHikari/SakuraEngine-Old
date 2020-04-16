// Prototype from Star Engine :
// https://github.com/star-e/StarEngine/blob/master/Star/SMap.h
// Thanks for the great idea and work !
// Copyright (C) 2019-2020 star.engine at outlook dot com
//
// StarEngine is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// StarEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with StarEngine.  If not, see <https://www.gnu.org/licenses/>.
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
 * @Date: 2020-03-15 22:36:05
 * @LastEditTime: 2020-03-30 12:18:46
 */
#pragma once
#include "../CoreMinimal/SKeyWords.h"
#include <string>
#include "SVariant.h"
#ifdef SAKURA_TARGET_PLATFORM_OSX
#include <experimental/memory_resource>
#include <experimental/string>
using namespace std::experimental;
#else
#include <memory_resource>
#endif

using namespace std; 

namespace Sakura
{
    // __c_plus_plus 17+
    using sstring = pmr::string;
    using swstring = pmr::wstring;
    /*
    using sstring = std::string;
    using swstring = std::wstring;*/
    using sstring_view = std::string_view;
    
    namespace String
    {
        sinline sstring str(std::string_view view)
        {
            return sstring(view);
        }
        sinline sstring_view view(const std::string& str) noexcept
        {
            return std::string_view(str);
        }
        sinline sstring_view view(const pmr::string& str) noexcept
        {
            return std::string_view(str);
        }
        /**
         * @description: Discard old ref and transfer. 
         * @param {type} 
         * @return: 
         * @author: SaeruHikari
         */
        template<typename To, typename From,
            typename std::enable_if<
                std::is_constructible<std::string_view, To>::value &&
                std::is_constructible<std::string_view, From>::value
            >::type * = nullptr>
        To DiscardCastStringTo(From& from)
        {
            std::variant<From, To> var = std::move(from);
            return std::move(Sakura::convert<To>(var));
        }

        /**
         * @description: Copy and transfer.  
         * @param {type} 
         * @return: 
         * @author: SaeruHikari
         */
        template<typename To, typename From,
            typename std::enable_if<
                std::is_constructible<std::string_view, To>::value &&
                std::is_constructible<std::string_view, From>::value
            >::type * = nullptr>
        To CastStringTo(From& from)
        {
            std::variant<From, To> var = from;
            return std::move(Sakura::convert<To>(var));
        }
    }
}
