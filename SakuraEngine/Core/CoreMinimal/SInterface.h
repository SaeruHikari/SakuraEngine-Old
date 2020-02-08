/*
 * @Descripttion: CopyRight SaeruHikari: Interface alias of Sakura
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 09:37:28
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-02 17:46:21
 */
#pragma once

#ifndef SInterface
#define SInterface struct
#endif

#ifndef SImplements
#define SImplements public
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
