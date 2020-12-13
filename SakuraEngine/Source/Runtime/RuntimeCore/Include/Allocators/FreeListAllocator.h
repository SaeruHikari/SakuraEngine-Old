#pragma once
#include "AllocatorBase.h"
#include <Allocators/SinglyLinkedList.h>

namespace sakura
{
    class free_list_allocator : public allocator {
    public:
        enum PlacementPolicy {
            FIND_FIRST,
            FIND_BEST
        };

    private:
        struct freeHeader {
            std::size_t blockSize;
        };
        struct allocation_header {
            std::size_t blockSize;
            char padding;
        };

        typedef singly_linked_list<freeHeader>::Node Node;


        void* m_start_ptr = nullptr;
        PlacementPolicy m_pPolicy;
        singly_linked_list<freeHeader> m_freeList;

    public:
        free_list_allocator(const std::size_t totalSize, const PlacementPolicy pPolicy);

        virtual ~free_list_allocator() override;

        virtual void* allocate(const std::size_t size, const std::size_t alignment = 0) override;

        virtual void free(void* ptr) override;

        virtual void init() override;

        virtual void reset();
    private:
        free_list_allocator(free_list_allocator& free_list_allocator);

        void Coalescence(Node* prevBlock, Node* freeBlock);

        void Find(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode);
        void FindBest(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode);
        void FindFirst(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode);
    };

}
