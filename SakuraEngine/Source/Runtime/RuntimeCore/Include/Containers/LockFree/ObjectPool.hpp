#pragma once
#include "FreeList.hpp"
#include <memory>

namespace sakura::lockfree
{

    template<typename T>
    class ObjectPool;

    namespace detail
    {
        struct SPStorage
        {
            uint8_t data[72];
        };

        template<class T> class SPAllocator;

        // specialize for void:
        template<>
        class SPAllocator<void>
        {
        public:
            typedef void* pointer;
            typedef const void* const_pointer;
            // reference to void members are impossible.
            typedef void value_type;

            template<class U>
            struct rebind
            {
                typedef SPAllocator<U> other;
            };

            SPAllocator(FreeList* pool, SPStorage* block) throw ()
                : block_(block)
                , used_(0)
                , pool_(pool)
            {
            }

            template<class U>
            SPAllocator(const SPAllocator<U>& u) throw ()
            {
                block_ = u.get_block();
                used_ = u.get_used();
                pool_ = u.get_pool();
            }

            SPStorage* get_block() const { return block_; }
            uint32_t get_used() const { return used_; }
            FreeList* get_pool() const { return pool_; }

        private:
            SPStorage* block_;
            uint32_t used_;
            FreeList* pool_;
        };

        template<class T>
        class SPAllocator
        {
        public:
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef T value_type;

            template<class U>
            struct rebind
            {
                typedef SPAllocator<U> other;
            };

            SPAllocator(FreeList* pool, SPStorage* block) throw ()
                : block_(block)
                , used_(0)
                , pool_(pool)
            {
            }

            template<class U>
            SPAllocator(const SPAllocator<U>& u) throw ()
            {
                block_ = u.get_block();
                used_ = u.get_used();
                pool_ = u.get_pool();
            }

            ~SPAllocator() throw ()
            {
            }

            pointer address(reference r) const
            {
                return &r;
            }
            const_pointer address(const_reference r) const
            {
                return &r;
            }
            size_type max_size() const throw ()
            {
                return ~size_type(0);
            }
            pointer allocate(size_type n, SPAllocator<void>::const_pointer hint = 0)
            {
                uint32_t to_alloc = n * sizeof(T);
                SAKURA_ASSERT_MSG(to_alloc <= (sizeof(SPStorage) - used_), "to_alloc=%d, size=%u, used=%d", to_alloc, (uint32_t)sizeof(SPStorage), used_);

                pointer p = reinterpret_cast<pointer>(block_->data + used_);
                used_ += to_alloc;
                return p;
            }
            void deallocate(pointer p, size_type n)
            {
                uint32_t to_free = n * sizeof(T);
                used_ -= to_free;
                SAKURA_ASSERT_MSG(used_ >= -(int32_t)sizeof(SPStorage), "to_free=%d, size=%u, used=%d", to_free, (uint32_t)sizeof(SPStorage), used_);

                if (used_ == 0 || used_ < 0)
                {
                    pool_->free(block_);
                }
            }

            void construct(pointer p, const_reference val)
            {
                new (p) T(val);
            }
            void destroy(pointer p)
            {
                p->~T();
            }

            SPStorage* get_block() const { return block_; }
            int32_t get_used() const { return used_; }
            FreeList* get_pool() const { return pool_; }

        private:
            SPStorage* block_;
            int32_t used_;
            FreeList* pool_;
        };

    } // namespace detail

    /**
     * \brief A fixed-count lock-free pool of the same type of object.  Supports both bare- and shared-pointer
     * allocation
     * \param T the object type
     */
    template<typename T>
    class ObjectPool
    {
        struct Deleter
        {
            Deleter(ObjectPool* pool, detail::SPStorage* storage)
                : pool_(pool)
                , sp_(storage)
                , free_(true)
            {
            }

            void operator()(T const* t)
            {
                if (free_)
                {
                    pool_->free(t);
                }
            }

            ObjectPool* pool_;
            detail::SPStorage* sp_;
            bool free_;
        };

    public:
        /**
         * \brief Default constructor.  Must call initialize() before calling allocate()
         */
        ObjectPool()
            : initialized_(false)
        {
        }

        /**
         * \brief Constructor with initialization.
         * \param count The number of objects in the pool
         * \param tmpl The object template to use to construct the objects
         */
        ObjectPool(uint32_t count, const T& tmpl)
            : initialized_(false)
        {
            initialize(count, tmpl);
        }

        ~ObjectPool()
        {
            freelist_.template destruct_all<T>();
            sp_storage_freelist_.template destruct_all<detail::SPStorage>();
        }

        /**
         * \brief Returns whether or not this ObjectPool has any outstanding allocations
         */
        bool hasOutstandingAllocations()
        {
            return freelist_.has_outstanding_allocations() || sp_storage_freelist_.has_outstanding_allocations();
        }

        /**
         * \brief initialize the pool.  Only use with the default constructor
         * \param count The number of objects in the pool
         * \param tmpl The object template to use to construct the objects
         */
        void initialize(uint32_t count, const T& tmpl)
        {
            SAKURA_ASSERT(!initialized_);
            freelist_.initialize(sizeof(T), count);
            freelist_.template construct_all<T>(tmpl);
            sp_storage_freelist_.initialize(sizeof(detail::SPStorage), count);
            sp_storage_freelist_.template construct_all<detail::SPStorage>();
            initialized_ = true;
        }

        /**
         * \brief Allocate a single object from the pool, returning a shared pointer
         * \return An empty shared pointer if there are no objects left in the pool.  Otherwise
         * a shared pointer to an object of type T
         */
        std::shared_ptr<T> allocateShared()
        {
            SAKURA_ASSERT(initialized_);

            T* item = static_cast<T*>(freelist_.allocate());
            if (!item)
            {
                return std::shared_ptr<T>();
            }

            std::shared_ptr<T> ptr = makeShared(item);
            if (!ptr)
            {
                freelist_.free(item);
                return std::shared_ptr<T>();
            }

            return ptr;
        }

        /**
         * \brief Make a shared_ptr out of a bare pointer allocated by this pool
         */
        std::shared_ptr<T> makeShared(T* t)
        {
            return makeSharedImpl(t);
        }

        /**
         * \brief Make a shared_ptr out of a bare pointer allocated by this pool
         */
        std::shared_ptr<T const> makeShared(T const* t)
        {
            return makeSharedImpl<T const>(t);
        }

        /**
         * \brief "Remove" a shared pointer allocated by this pool, turning it into a bare pointer.
         * shared_ptrs held to this object will no longer delete the object when the go out of scope.
         * Generally dangerous and not to be used lightly, this will probably go away in the future.
         */
        T* removeShared(const std::shared_ptr<T>& t)
        {
            SAKURA_ASSERT(freelist_.owns(t.get()));

            Deleter* d = std::get_deleter<Deleter>(t);
            d->free_ = false;
            return t.get();
        }

        /**
         * \brief "Remove" a shared pointer allocated by this pool, turning it into a bare pointer.
         * shared_ptrs held to this object will no longer delete the object when the go out of scope.
         * Generally dangerous and not to be used lightly, this will probably go away in the future.
         */
        T const* removeShared(const std::shared_ptr<T const>& t)
        {
            SAKURA_ASSERT(freelist_.owns(t.get()));

            Deleter* d = std::get_deleter<Deleter>(t);
            d->free_ = false;
            return t.get();
        }

        /**
         * \brief Allocate a single object from the pool, returning a bare pointer
         * \return 0 if there are no objects left in the pool.  Otherwise a pointer to an object
         * of type T
         */
        T* allocate()
        {
            return static_cast<T*>(freelist_.allocate());
        }

        /**
         * \brief Return an object allocated through allocateBare() to the pool
         * \param t An object that was allocated with allocateBare()
         */
        void free(T const* t)
        {
            freelist_.free(t);
        }

        /**
         * \brief Returns whether or not this pool owns the provided object
         */
        bool owns(T const* t)
        {
            return freelist_.owns(t);
        }

        /**
         * \brief Returns whether or not this pool owns the provided object
         */
        bool owns(const std::shared_ptr<T const>& t)
        {
            return owns(t.get());
        }

    private:

        template<typename T2>
        std::shared_ptr<T2> makeSharedImpl(T2* t)
        {
            ROS_ASSERT(freelist_.owns(t));

            detail::SPStorage* sp_storage = static_cast<detail::SPStorage*>(sp_storage_freelist_.allocate());

            if (!sp_storage)
            {
                return std::shared_ptr<T2>();
            }

            std::shared_ptr<T2> ptr(t, Deleter(this, sp_storage), detail::SPAllocator<void>(&sp_storage_freelist_, sp_storage));
            return ptr;
        }

        bool initialized_;

        FreeList freelist_;
        FreeList sp_storage_freelist_;
    };

} // namespace lockfree

