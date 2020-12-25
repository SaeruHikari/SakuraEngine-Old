#include <SakuraSTL.hpp>
#include <Containers/LockFree/FreeList.hpp>


std::string_view std::to_string_view(const sakura::string& s) noexcept
{
    return { (const char*)s.data(), s.length() };
}

std::wstring_view std::to_string_view(const sakura::wstring& s) noexcept
{
    return { s.data(), s.length() };
}

namespace allocators
{
    inline void* alignedMalloc(size_t size, size_t alignment)
    {
        const int pointerSize = sizeof(void*);
        const int requestedSize = size + alignment - 1 + pointerSize;
        void* raw = std::malloc(requestedSize);

        if (!raw)
        {
            return 0;
        }

        void* start = (uint8_t*)raw + pointerSize;
        void* aligned = (void*)(((uintptr_t)((uint8_t*)start + alignment - 1)) & ~(alignment - 1));
        *(void**)((uint8_t*)aligned - pointerSize) = raw;
        return aligned;
    }

    /**
     * \brief Free memory allocated through alignedMalloc()
     * \param aligned The memory to free
     */
    inline void alignedFree(void* aligned)
    {
        if (!aligned)
        {
            return;
        }

        void* raw = *(void**)((uint8_t*)aligned - sizeof(void*));
        std::free(raw);
    }
}

namespace sakura::lockfree
{

    FreeList::FreeList()
        : blocks_(0)
        , next_(0)
        , block_size_(0)
        , block_count_(0)
    {
    }

    FreeList::FreeList(uint32_t block_size, uint32_t block_count)
        : blocks_(0)
        , next_(0)
        , block_size_(0)
        , block_count_(0)
    {
        initialize(block_size, block_count);
    }

    FreeList::~FreeList()
    {
        using atomic_uint32_t = std::atomic_uint32_t;
        for (uint32_t i = 0; i < block_count_; ++i)
        {
            next_[i].~atomic_uint32_t();
        }
        allocators::alignedFree(blocks_);
        allocators::alignedFree(next_);
    }

    void FreeList::initialize(uint32_t block_size, uint32_t block_count)
    {
        SAKURA_ASSERT(!blocks_);
        SAKURA_ASSERT(!next_);

        block_size_ = block_size;
        block_count_ = block_count;

        head_.store(0);
        alloc_count_.store(0);

        blocks_ = (uint8_t*)allocators::alignedMalloc(block_size * block_count, SAKURA_CACHELINE_SIZE);
        next_ = (std::atomic_uint32_t*)allocators::alignedMalloc(sizeof(std::atomic_uint32_t) * block_count, SAKURA_CACHELINE_SIZE);

        memset(blocks_, 0xCD, block_size * block_count);

        for (uint32_t i = 0; i < block_count_; ++i)
        {
            new (next_ + i) std::atomic_uint32_t();

            if (i == block_count_ - 1)
            {
                next_[i].store(0xffffffffUL);
            }
            else
            {
                next_[i].store(i + 1);
            }
        }
    }

    bool FreeList::has_outstanding_allocations()
    {
        return alloc_count_.load() == 0;
    }

    void* FreeList::allocate()
    {
        SAKURA_ASSERT(blocks_);

        while (true)
        {
            uint64_t head = head_.load(std::memory_order_consume);

            if (get_val(head) == 0xffffffffULL)
            {
                return 0;  // Allocation failed
            }

            // Load the next index
            uint64_t new_head = next_[get_val(head)].load();

            // Increment the tag to avoid ABA
            set_tag(new_head, get_tag(head) + 1);

            // If setting head to next is successful, return the item at next
            if (head_.compare_exchange_strong(head, new_head))
            {
                alloc_count_.fetch_add(1);
                return static_cast<void*>(blocks_ + (block_size_ * get_val(head)));
            }
        }
    }

    void FreeList::free(void const* mem)
    {
        if (!mem)
        {
            return;
        }

#if FREE_LIST_DEBUG
        initDebug();
#endif

        uint32_t index = (static_cast<uint8_t const*>(mem) - blocks_) / block_size_;

        SAKURA_ASSERT(((static_cast<uint8_t const*>(mem) - blocks_) % block_size_) == 0);
        SAKURA_ASSERT(owns(mem));

        while (true)
        {
            // Load head
            uint64_t head = head_.load(std::memory_order_consume);

            uint64_t new_head = head;
            // set new head to the index of the block we're currently freeing
            set_val(new_head, index);
            // Increment the tag to avoid ABA
            set_tag(new_head, get_tag(new_head) + 1);


            // Store head as next index for this item
            next_[index].store(get_val(head));

            // If setting the head to next is successful, return
            if (head_.compare_exchange_strong(head, new_head))
            {
                alloc_count_.fetch_sub(1);
                return;
            }
        }
    }

    bool FreeList::owns(void const* mem)
    {
        uint32_t sub = (static_cast<uint8_t const*>(mem) - blocks_);
        return sub < block_count_* block_size_;
    }

} // namespace sakura::lockfree
