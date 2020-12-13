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
 * @LastEditTime: 2020-08-11 17:39:57
 */
#include "Allocators/StackAllocator.h"
#include "Allocators/Utils.h"  /* CalculatePadding */
#include <stdlib.h>     /* malloc, free */
#include <algorithm>    /* max */
#ifdef _DEBUG_ALLOCATORS
#include <iostream>
#endif

using namespace sakura;

stack_allocator::stack_allocator(const std::size_t totalSize)
: allocator(totalSize) {

}

void stack_allocator::init() {
    if (m_start_ptr != nullptr) {
        free(m_start_ptr);
    }
    m_start_ptr = malloc(m_totalSize);
    m_offset = 0;
}

stack_allocator::~stack_allocator() {
    free(m_start_ptr);
    m_start_ptr = nullptr;
}

void* stack_allocator::allocate(const std::size_t size, const std::size_t alignment) {
    const std::size_t currentAddress = (std::size_t)m_start_ptr + m_offset;

    std::size_t padding = Utils::CalculatePaddingWithHeader(currentAddress, alignment, sizeof (allocation_header));

    if (m_offset + padding + size > m_totalSize) {
        return nullptr;
    }
    m_offset += padding;

    const std::size_t nextAddress = currentAddress + padding;
    const std::size_t headerAddress = nextAddress - sizeof (allocation_header);
    stack_allocator::allocation_header ah{(char)padding};
    stack_allocator::allocation_header* headerPtr = (stack_allocator::allocation_header*) headerAddress;
    headerPtr = &ah;
    
    m_offset += size;

#ifdef _DEBUG_ALLOCATORS
    std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
#endif
    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return (void*) nextAddress;
}

void stack_allocator::free(void *ptr) {
    // Move offset back to clear address
    const std::size_t currentAddress = (std::size_t) ptr;
    const std::size_t headerAddress = currentAddress - sizeof (allocation_header);
    const allocation_header * ah{ (allocation_header *) headerAddress};

    m_offset = currentAddress - ah->padding - (std::size_t) m_start_ptr;
    m_used = m_offset;

#ifdef _DEBUG_ALLOCATORS
    std::cout << "F" << "\t@C " << (void*) currentAddress << "\t@F " << (void*) ((char*) m_start_ptr + m_offset) << "\tO " << m_offset << std::endl;
#endif
}

void stack_allocator::reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}