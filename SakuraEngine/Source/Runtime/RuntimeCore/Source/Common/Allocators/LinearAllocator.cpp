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
 * @Date: 2020-08-11 17:19:49
 * @LastEditTime: 2020-08-11 17:20:50
 */
#include <cassert>   /*assert		*/
#include <algorithm>    // max
#ifdef _DEBUG
#include <iostream>
#endif
#define API_EXPORTS 1
#include "Allocators/Utils.h"  /* CalculatePadding */
#include "Allocators/LinearAllocator.h"

using namespace sakura;

RuntimeCoreAPI linear_allocator::linear_allocator(const std::size_t totalSize)
: allocator(totalSize), m_offset(0)
{
}

linear_allocator::linear_allocator(const linear_allocator& linear_allocator)
{
    if (m_start_ptr != nullptr) {
        ::free(m_start_ptr);
    }
    m_start_ptr = malloc(linear_allocator.m_totalSize);
    m_offset = linear_allocator.m_offset;
    m_used = linear_allocator.m_used;
    m_totalSize = linear_allocator.m_totalSize;
    m_peak = linear_allocator.m_peak;
    ::memcpy(m_start_ptr, linear_allocator.m_start_ptr, m_totalSize);
}

RuntimeCoreAPI void linear_allocator::init() {
    if (m_start_ptr != nullptr) {
        ::free(m_start_ptr);
    }
    m_start_ptr = malloc(m_totalSize);
    m_offset = 0;
}

RuntimeCoreAPI linear_allocator::~linear_allocator() {
    ::free(m_start_ptr);
    m_start_ptr = nullptr;
}
#ifdef _DEBUG
#include <iostream>
#endif
RuntimeCoreAPI void* linear_allocator::allocate(const std::size_t size, const std::size_t alignment) {
    std::size_t padding = 0;
    std::size_t paddedAddress = 0;
    const std::size_t currentAddress = (std::size_t)m_start_ptr + m_offset;

    if (alignment != 0 && m_offset % alignment != 0) {
        // Alignment is required. Find the next aligned memory address and update offset
        padding = Utils::CalculatePadding(currentAddress, alignment);
    }

    if (m_offset + padding + size > m_totalSize) {
        return nullptr;
    }

    m_offset += padding;
    const std::size_t nextAddress = currentAddress + padding;
    m_offset += size;
#ifdef _DEBUG_ALLOCATORS
    std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
#endif
    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return (void*) nextAddress;
}

RuntimeCoreAPI void linear_allocator::free(void* ptr) {
    assert(false && "Use reset() method");
}

RuntimeCoreAPI void linear_allocator::reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}
