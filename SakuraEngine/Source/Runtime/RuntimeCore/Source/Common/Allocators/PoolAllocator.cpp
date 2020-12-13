/*
 * @Author: your name
 * @Date: 2020-08-11 17:19:49
 * @LastEditTime: 2020-08-11 17:30:03
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \allocators\PoolAllocator.cpp
 */
#include "Allocators/PoolAllocator.h"
#include <algorithm>    //max
#ifdef _DEBUG_ALLOCATORS
#include <iostream>
#endif

using namespace sakura;

pool_allocator::pool_allocator(const std::size_t totalSize, const std::size_t chunkSize)
: allocator(totalSize), m_freeList()
{
    assert(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
    assert(totalSize % chunkSize == 0 && "Total Size must be a multiple of Chunk Size");
    this->m_chunkSize = chunkSize;
}

void pool_allocator::init() {
    m_start_ptr = malloc(m_totalSize);
    this->reset();
}

pool_allocator::~pool_allocator() {
    free(m_start_ptr);
}

void *pool_allocator::allocate(const std::size_t allocationSize, const std::size_t alignment) {
    assert(allocationSize == this->m_chunkSize && "Allocation size must be equal to chunk size");

    Node * freePosition = m_freeList.pop();

    assert(freePosition != nullptr && "The pool allocator is full");

    m_used += m_chunkSize;
    m_peak = std::max(m_peak, m_used);
#ifdef _DEBUG_ALLOCATORS
    std::cout << "A" << "\t@S " << m_start_ptr << "\t@R " << (void*) freePosition << "\tM " << m_used << std::endl;
#endif

    return (void*) freePosition;
}

void pool_allocator::free(void * ptr) {
    m_used -= m_chunkSize;

    m_freeList.push((Node *) ptr);

#ifdef _DEBUG_ALLOCATORS
    std::cout << "F" << "\t@S " << m_start_ptr << "\t@F " << ptr << "\tM " << m_used << std::endl;
#endif
}

void pool_allocator::reset() {
    m_used = 0;
    m_peak = 0;
    // Create a linked-list with all free positions
    const size_t nChunks = m_totalSize / m_chunkSize; 
    for (int i = 0; i < nChunks; ++i) {
        std::size_t address = (std::size_t) m_start_ptr + i * m_chunkSize;
        m_freeList.push((Node *) address);
    }
}