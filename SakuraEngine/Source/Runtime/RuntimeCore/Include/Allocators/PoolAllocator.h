#include "AllocatorBase.h"
#include "Allocators/StackLinkedList.h"

namespace sakura
{
    class pool_allocator : public allocator {
    private:
        struct  freeHeader {
        };
        using Node = stack_linked_list<freeHeader>::Node;
        stack_linked_list<freeHeader> m_freeList;

        void* m_start_ptr = nullptr;
        std::size_t m_chunkSize;
    public:
        pool_allocator(const std::size_t totalSize, const std::size_t chunkSize);

        virtual ~pool_allocator() override;

        virtual void* allocate(const std::size_t size, const std::size_t alignment = 0) override;

        virtual void free(void* ptr) override;

        virtual void init() override;

        virtual void reset();
    private:
        pool_allocator(pool_allocator& pool_allocator);

    };
}