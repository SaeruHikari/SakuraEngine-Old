#pragma once
#include <Base/Definations.h>
#include <atomic>

namespace sakura::lockfree
{

    /**
     * \brief A lock-free (*not* wait-free) statically-sized free-list implemented with CAS
     *
     * FreeList is implemented as a forward-linked list using indices instead of pointers.
     * The array of blocks is allocated separately from the array of next indices, and both
     * are always allocated aligned to a cache line.
     *
     * Indices are stored as 32-bits with a 64-bit head index whose upper 32-bits are tagged
     * to avoid ABA problems
     */
    class FreeList
    {
    public:
        /**
         * \brief Default constructor.  You must call initialize() if you use this constructor.
         */
        FreeList();
        /**
         * \brief Constructor with initialization
         * \param block_size The size of each block allocate() will return
         * \param block_count The number of blocks to allocate
         */
        FreeList(uint32_t block_size, uint32_t block_count);
        ~FreeList();

        /**
         * \brief Initialize this FreeList.  Only use if you used to default constructor
         * \param block_size The size of each block allocate() will return
         * \param block_count The number of blocks to allocate
         */
        void initialize(uint32_t block_size, uint32_t block_count);

        /**
         * \brief Allocate a single block from this FreeList
         * \return 0 if all blocks are allocated, a pointer to a memory block of size block_size_ otherwise
         */
        void* allocate();
        /**
         * \brief Free a block of memory allocated from this FreeList
         * \param mem The block to be freed
         */
        void free(void const* mem);
        /**
         * \brief Returns whether or not this FreeList owns a block of memory
         * \param mem the block to check
         * \return true if this FreeList owns the block, false otherwise
         */
        bool owns(void const* mem);

        /**
         * \brief Returns whether or not this FreeList currently has any outstanding allocations
         */
        bool has_outstanding_allocations();

        /**
         * \brief Construct all the blocks with a specific template.  If you call this you must call
         * destruct_all() prior to destroying this FreeList.  This is mainly for use by the ObjectPool
         * class, or for writing a similar class, and generally that class should be used instead of
         * FreeList directly
         * \param tmpl The object template to use
         *
         * \note sizeof(T) must be less than or equal to block_size_
         */
        template<typename T>
        void construct_all(const T& tmpl)
        {
            SAKURA_ASSERT(sizeof(T) <= block_size_);
            for (uint32_t i = 0; i < block_count_; ++i)
            {
                new (blocks_ + (i * block_size_)) T(tmpl);
            }
        }

        /**
         * \brief Construct all the blocks with a default constructor.  If you call this you must call
         * destruct_all() prior to destroying this FreeList. This is mainly for use by the ObjectPool
         * class, or for writing a similar class, and generally that class should be used instead of
         * FreeList directly
         * \note sizeof(T) must be less than or equal to block_size_
         */
        template<typename T>
        void construct_all()
        {
            SAKURA_ASSERT(sizeof(T) <= block_size_);
            for (uint32_t i = 0; i < block_count_; ++i)
            {
                new (blocks_ + (i * block_size_)) T();
            }
        }

        /**
         * \brief Destruct all the objects in this FreeList.  You must have called construct_all() first.
         * \note sizeof(T) must equal block_size_
         */
        template<typename T>
        void destruct_all()
        {
            SAKURA_ASSERT(sizeof(T) <= block_size_);
            for (uint32_t i = 0; i < block_count_; ++i)
            {
                reinterpret_cast<T*>(blocks_ + (i * block_size_))->~T();
            }
        }

    private:

        inline uint32_t get_tag(uint64_t val)
        {
            return (uint32_t)(val >> 32);
        }

        inline uint32_t get_val(uint64_t val)
        {
            return (uint32_t)val & 0xffffffff;
        }

        inline void set_tag(uint64_t& val, uint32_t tag)
        {
            val = get_val(val) | ((uint64_t)tag << 32);
        }

        inline void set_val(uint64_t& val, uint32_t v)
        {
            val = ((uint64_t)get_tag(val) << 32) | v;
        }

        uint8_t* blocks_;
        std::atomic_uint32_t* next_;
        std::atomic_uint64_t head_;
        std::atomic_uint32_t alloc_count_;

        uint32_t block_size_;
        uint32_t block_count_;
    };

} // namespace sakura::lockfree

