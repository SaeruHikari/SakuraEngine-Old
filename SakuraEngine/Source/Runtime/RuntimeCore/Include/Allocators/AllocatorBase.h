/*
 * @Author: your name
 * @Date: 2020-08-11 17:19:36
 * @LastEditTime: 2020-08-11 17:25:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \allocators\Allocator.h
 */
#pragma once
#include <Base/Definations.h>

namespace sakura
{
    class RuntimeCoreAPI allocator {
    protected:
        allocator() = default;
        std::size_t m_totalSize;
        std::size_t m_used;
        std::size_t m_peak;
    public:
        allocator(const std::size_t totalSize);

        virtual ~allocator();

        virtual void* allocate(const std::size_t size, const std::size_t alignment = 0) = 0;

        virtual void free(void* ptr) = 0;

        virtual void init() = 0;

        friend class Benchmark;
    };
}

