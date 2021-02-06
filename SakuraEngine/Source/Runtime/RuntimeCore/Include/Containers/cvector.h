#pragma once
#include "Base/CDefinitions.h"

/***** DEFINITIONS *****/

#define cvector_MINIMUM_CAPACITY 2
#define cvector_GROWTH_FACTOR 2
#define cvector_SHRINK_THRESHOLD (1 / 4)

#define cvector_ERROR -1
#define cvector_SUCCESS 0

#define cvector_UNINITIALIZED NULL
#define cvector_INITIALIZER \
	{ 0, 0, 0, cvector_UNINITIALIZED }

extern "C"
{
    typedef struct cvector
    {
        size_t size;
        size_t capacity;
        size_t element_size;
        void* data;
    } cvector;

    typedef struct cvector_iterator 
    {
        void* pointer;
        size_t element_size;
    } cvector_iterator;

    /***** METHODS *****/
    /* Constructor */
    int cvector_setup(cvector* cvector, size_t capacity, size_t element_size);

    /* Copy Constructor */
    int cvector_copy(cvector* destination, cvector* source);

    /* Copy Assignment */
    int cvector_copy_assign(cvector* destination, cvector* source);

    /* Move Constructor */
    int cvector_move(cvector* destination, cvector* source);

    /* Move Assignment */
    int cvector_move_assign(cvector* destination, cvector* source);

    int cvector_swap(cvector* destination, cvector* source);

    /* Destructor */
    int cvector_destroy(cvector* cvector);

    /* Insertion */
    int cvector_push_back(cvector* cvector, void* element);
    int cvector_push_front(cvector* cvector, void* element);
    int cvector_insert(cvector* cvector, size_t index, void* element);
    int cvector_assign(cvector* cvector, size_t index, void* element);

    /* Deletion */
    int cvector_pop_back(cvector* cvector);
    int cvector_pop_front(cvector* cvector);
    int cvector_erase(cvector* cvector, size_t index);
    int cvector_clear(cvector* cvector);

    /* Lookup */
    void* cvector_get(cvector* cvector, size_t index);
    const void* cvector_const_get(const cvector* cvector, size_t index);
    void* cvector_front(cvector* cvector);
    void* cvector_back(cvector* cvector);
    #define cvector_GET_AS(type, cvector_pointer, index) \
        *((type*)cvector_get((cvector_pointer), (index)))

    /* Information */
    bool cvector_is_initialized(const cvector* cvector);
    size_t cvector_byte_size(const cvector* cvector);
    size_t cvector_free_space(const cvector* cvector);
    bool cvector_is_empty(const cvector* cvector);

    /* Memory management */
    int cvector_resize(cvector* cvector, size_t new_size);
    int cvector_reserve(cvector* cvector, size_t minimum_capacity);
    int cvector_shrink_to_fit(cvector* cvector);

    /* cvector_iterators */
    cvector_iterator cvector_begin(cvector* cvector);
    cvector_iterator cvector_end(cvector* cvector);
    cvector_iterator cvector_cvector_iterator(cvector* cvector, size_t index);

    void* cvector_iterator_get(cvector_iterator* cvector_iterator);
    #define cvector_ITERATOR_GET_AS(type, cvector_iterator) *((type*)cvector_iterator_get((cvector_iterator)))

    int cvector_iterator_erase(cvector* cvector, cvector_iterator* cvector_iterator);

    void cvector_iterator_increment(cvector_iterator* cvector_iterator);
    void cvector_iterator_decrement(cvector_iterator* cvector_iterator);

    void* cvector_iterator_next(cvector_iterator* cvector_iterator);
    void* cvector_iterator_previous(cvector_iterator* cvector_iterator);

    bool cvector_iterator_equals(cvector_iterator* first, cvector_iterator* second);
    bool cvector_iterator_is_before(cvector_iterator* first, cvector_iterator* second);
    bool cvector_iterator_is_after(cvector_iterator* first, cvector_iterator* second);

    size_t cvector_iterator_index(cvector* cvector, cvector_iterator* cvector_iterator);

    #define cvector_FOR_EACH(cvector_pointer, cvector_iterator_name)           \
        for (cvector_iterator(cvector_iterator_name) = cvector_begin((cvector_pointer)), \
                end = cvector_end((cvector_pointer));                        \
                !cvector_iterator_equals(&(cvector_iterator_name), &end);                 \
                cvector_iterator_increment(&(cvector_iterator_name)))

    /***** PRIVATE *****/
    bool _cvector_should_grow(cvector* cvector);
    bool _cvector_should_shrink(cvector* cvector);

    size_t _cvector_free_bytes(const cvector* cvector);
    void* _cvector_offset(cvector* cvector, size_t index);
    const void* _cvector_const_offset(const cvector* cvector, size_t index);

    void _cvector_assign(cvector* cvector, size_t index, void* element);

    int _cvector_move_right(cvector* cvector, size_t index);
    void _cvector_move_left(cvector* cvector, size_t index);

    int _cvector_adjust_capacity(cvector* cvector);
    int _cvector_reallocate(cvector* cvector, size_t new_capacity);

    void _cvector_swap(size_t* first, size_t* second);
}

#include "Detail/cvector-inl.h"