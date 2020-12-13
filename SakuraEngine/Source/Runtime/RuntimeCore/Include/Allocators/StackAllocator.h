/*
 * @Author: your name
 * @Date: 2020-08-11 17:19:36
 * @LastEditTime: 2020-08-11 17:33:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \allocators\StackAllocator.h
 */
#pragma once
#include "AllocatorBase.h"

namespace sakura
{
    class stack_allocator : public allocator {
    protected:
        void* m_start_ptr = nullptr;
        std::size_t m_offset;
    public:
        stack_allocator(const std::size_t totalSize);

        virtual ~stack_allocator() override;

        virtual void* allocate(const std::size_t size, const std::size_t alignment = 0) override;

        virtual void free(void* ptr) override;

        virtual void init() override;

        virtual void reset();
    private:
        stack_allocator(stack_allocator& stack_allocator);

        struct allocation_header {
            char padding;
        };

    };

}