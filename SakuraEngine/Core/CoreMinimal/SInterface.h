/*
 * @Descripttion: CopyRight SaeruHikari: Interface alias of Sakura
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 09:37:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-18 18:17:31
 */
#pragma once

#ifndef sinterface
#define sinterface struct
#endif
#ifndef simplements
#define simplements public
#endif

namespace Sakura
{
    // Has V-Table Overhead!
    // Use Macro-defination and SFINAE instead.
    //struct NonCopyble
    //{
    //   NonCopyble() = default;
    //    NonCopyble(const NonCopyble& rhs) = delete;
    //   NonCopyble& operator=(const NonCopyble& rhs) = delete;
    //};
}
