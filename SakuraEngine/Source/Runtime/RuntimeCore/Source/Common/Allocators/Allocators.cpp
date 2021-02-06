#include "Allocators/CAllocator.h"
#include "Allocators/FreeListAllocator.h"
#include "Allocators/Utils.h"  /* CalculatePaddingWithHeader */

#include <stdlib.h>     /* malloc, free */
#include <limits>  /* limits_max */
#include <algorithm>    // std::max
#include <cassert> //assert

using namespace sakura;

sakura::allocator::allocator(const std::size_t totalSize){
    m_totalSize = totalSize;
    m_used = 0;
}

sakura::allocator::~allocator(){
    m_totalSize = 0;
}

// ---- C_ALLOCATOR ----

c_allocator::c_allocator()
    : allocator(0) {

}

void c_allocator::init() {

}

c_allocator::~c_allocator(){
    
}

void* c_allocator::allocate(const std::size_t size, const std::size_t alignment) {
	return malloc(size);
}

void c_allocator::free(void* ptr) {
	free(ptr);
}

// ---- C_ALLOCATOR ----


// ---- FREE_LIST_ALLOCATOR ----
#include "Allocators/FreeListAllocator.h"
#ifdef _DEBUG_ALLOCATORS
#include <iostream>
#endif

using namespace sakura;

free_list_allocator::free_list_allocator(const std::size_t totalSize, const PlacementPolicy pPolicy)
: allocator(totalSize) {
    m_pPolicy = pPolicy;
}

void free_list_allocator::init() {
    if (m_start_ptr != nullptr) {
        free(m_start_ptr);
        m_start_ptr = nullptr;
    }
    m_start_ptr = malloc(m_totalSize);

    this->reset();
}

free_list_allocator::~free_list_allocator() {
    free(m_start_ptr);
    m_start_ptr = nullptr;
}

void* free_list_allocator::allocate(const std::size_t size, const std::size_t alignment) {
    const std::size_t allocation_headerSize = sizeof(free_list_allocator::allocation_header);
    const std::size_t freeHeaderSize = sizeof(free_list_allocator::freeHeader);
    assert("Allocation size must be bigger" && size >= sizeof(Node));
    assert("Alignment must be 8 at least" && alignment >= 8);

    // Search through the free list for a free block that has enough space to allocate our data
    std::size_t padding;
    Node * affectedNode, 
         * previousNode;
    this->Find(size, alignment, padding, previousNode, affectedNode);
    assert (affectedNode != nullptr && "Not enough memory");


    const std::size_t alignmentPadding =  padding - allocation_headerSize;
    const std::size_t requiredSize = size + padding;    

    const std::size_t rest = affectedNode->data.blockSize - requiredSize;

    if (rest > 0) {
        // We have to split the block into the data block and a free block of size 'rest'
        Node * newfreeNode = (Node *)((std::size_t) affectedNode + requiredSize);
        newfreeNode->data.blockSize = rest;
        m_freeList.insert(affectedNode, newfreeNode);
    }
    m_freeList.remove(previousNode, affectedNode);

    // Setup data block
    const std::size_t headerAddress = (std::size_t) affectedNode + alignmentPadding;
    const std::size_t dataAddress = headerAddress + allocation_headerSize;
    ((free_list_allocator::allocation_header *) headerAddress)->blockSize = requiredSize;
    ((free_list_allocator::allocation_header *) headerAddress)->padding = static_cast<char>(alignmentPadding);

    m_used += requiredSize;
    m_peak = std::max(m_peak, m_used);

#ifdef _DEBUG_ALLOCATORS
    std::cout << "A" << "\t@H " << (void*) headerAddress << "\tD@ " <<(void*) dataAddress << "\tS " << ((free_list_allocator::allocation_header *) headerAddress)->blockSize <<  "\tAP " << alignmentPadding << "\tP " << padding << "\tM " << m_used << "\tR " << rest << std::endl;
#endif

    return (void*) dataAddress;
}

void free_list_allocator::Find(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    switch (m_pPolicy) {
        case FIND_FIRST:
            FindFirst(size, alignment, padding, previousNode, foundNode);
            break;
        case FIND_BEST:
            FindBest(size, alignment, padding, previousNode, foundNode);
            break;
    }
}

void free_list_allocator::FindFirst(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    //Iterate list and return the first free block with a size >= than given size
    Node * it = m_freeList.head,
         * itPrev = nullptr;
    
    while (it != nullptr) {
        padding = Utils::CalculatePaddingWithHeader((std::size_t)it, alignment, sizeof (free_list_allocator::allocation_header));
        const std::size_t requiredSpace = size + padding;
        if (it->data.blockSize >= requiredSpace) {
            break;
        }
        itPrev = it;
        it = it->next;
    }
    previousNode = itPrev;
    foundNode = it;
}

void free_list_allocator::FindBest(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    // Iterate WHOLE list keeping a pointer to the best fit
    std::size_t smallestDiff = std::numeric_limits<std::size_t>::max();
    Node * bestBlock = nullptr;
    Node * it = m_freeList.head,
         * itPrev = nullptr;
    while (it != nullptr) {
        padding = Utils::CalculatePaddingWithHeader((std::size_t)it, alignment, sizeof (free_list_allocator::allocation_header));
        const std::size_t requiredSpace = size + padding;
        if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff)) {
            bestBlock = it;
        }
        itPrev = it;
        it = it->next;
    }
    previousNode = itPrev;
    foundNode = bestBlock;
}

void free_list_allocator::free(void* ptr) {
    // Insert it in a sorted position by the address number
    const std::size_t currentAddress = (std::size_t) ptr;
    const std::size_t headerAddress = currentAddress - sizeof (free_list_allocator::allocation_header);
    const free_list_allocator::allocation_header * allocation_header{ (free_list_allocator::allocation_header *) headerAddress};

    Node * freeNode = (Node *) (headerAddress);
    freeNode->data.blockSize = allocation_header->blockSize + allocation_header->padding;
    freeNode->next = nullptr;

    Node * it = m_freeList.head;
    Node * itPrev = nullptr;
    while (it != nullptr) {
        if (ptr < it) {
            m_freeList.insert(itPrev, freeNode);
            break;
        }
        itPrev = it;
        it = it->next;
    }
    
    m_used -= freeNode->data.blockSize;

    // Merge contiguous nodes
    Coalescence(itPrev, freeNode);  

#ifdef _DEBUG_ALLOCATORS
    std::cout << "F" << "\t@ptr " <<  ptr <<"\tH@ " << (void*) freeNode << "\tS " << freeNode->data.blockSize << "\tM " << m_used << std::endl;
#endif
}

void free_list_allocator::Coalescence(Node* previousNode, Node * freeNode) {   
    if (freeNode->next != nullptr && 
            (std::size_t) freeNode + freeNode->data.blockSize == (std::size_t) freeNode->next) {
        freeNode->data.blockSize += freeNode->next->data.blockSize;
        m_freeList.remove(freeNode, freeNode->next);
#ifdef _DEBUG_ALLOCATORS
    std::cout << "\tMerging(n) " << (void*) freeNode << " & " << (void*) freeNode->next << "\tS " << freeNode->data.blockSize << std::endl;
#endif
    }
    
    if (previousNode != nullptr &&
            (std::size_t) previousNode + previousNode->data.blockSize == (std::size_t) freeNode) {
        previousNode->data.blockSize += freeNode->data.blockSize;
        m_freeList.remove(previousNode, freeNode);
#ifdef _DEBUG_ALLOCATORS
    std::cout << "\tMerging(p) " << (void*) previousNode << " & " << (void*) freeNode << "\tS " << previousNode->data.blockSize << std::endl;
#endif
    }
}

void free_list_allocator::reset() {
    m_used = 0;
    m_peak = 0;
    Node * firstNode = (Node *) m_start_ptr;
    firstNode->data.blockSize = m_totalSize;
    firstNode->next = nullptr;
    m_freeList.head = nullptr;
    m_freeList.insert(nullptr, firstNode);
}

// ---- FREE_LIST_ALLOCATOR ----

// ---- LINEAR_ALLOCATOR ----
#include "Allocators/LinearAllocator.h"

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

// ---- LINEAR_ALLOCATOR ----

// ---- POOL_ALLOCATOR ----
#include "Allocators/PoolAllocator.h"

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

// ---- POOL_ALLOCATOR ----

// ---- STACK_ALLOCATOR ----
#include "Allocators/StackAllocator.h"


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
// ---- STACK_ALLOCATOR ----