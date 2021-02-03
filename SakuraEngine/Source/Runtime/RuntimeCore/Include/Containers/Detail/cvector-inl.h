extern "C"
{
inline int cvector_setup(cvector* cvector, size_t capacity, size_t element_size) {
	assert(cvector != NULL);

	if (cvector == NULL) return cvector_ERROR;

	cvector->size = 0;
	cvector->capacity = SAKURA_MAX(cvector_MINIMUM_CAPACITY, capacity);
	cvector->element_size = element_size;
	cvector->data = malloc(cvector->capacity * element_size);

	return cvector->data == NULL ? cvector_ERROR : cvector_SUCCESS;
}

inline int cvector_copy(cvector* destination, cvector* source) {
	assert(destination != NULL);
	assert(source != NULL);
	assert(cvector_is_initialized(source));
	assert(!cvector_is_initialized(destination));

	if (destination == NULL) return cvector_ERROR;
	if (source == NULL) return cvector_ERROR;
	if (cvector_is_initialized(destination)) return cvector_ERROR;
	if (!cvector_is_initialized(source)) return cvector_ERROR;

	/* Copy ALL the data */
	destination->size = source->size;
	destination->capacity = source->size * 2;
	destination->element_size = source->element_size;

	/* Note that we are not necessarily allocating the same capacity */
	destination->data = malloc(destination->capacity * source->element_size);
	if (destination->data == NULL) return cvector_ERROR;

	memcpy(destination->data, source->data, cvector_byte_size(source));

	return cvector_SUCCESS;
}

inline int cvector_copy_assign(cvector* destination, cvector* source) {
	assert(destination != NULL);
	assert(source != NULL);
	assert(cvector_is_initialized(source));
	assert(cvector_is_initialized(destination));

	if (destination == NULL) return cvector_ERROR;
	if (source == NULL) return cvector_ERROR;
	if (!cvector_is_initialized(destination)) return cvector_ERROR;
	if (!cvector_is_initialized(source)) return cvector_ERROR;

	cvector_destroy(destination);

	return cvector_copy(destination, source);
}

inline int cvector_move(cvector* destination, cvector* source) {
	assert(destination != NULL);
	assert(source != NULL);

	if (destination == NULL) return cvector_ERROR;
	if (source == NULL) return cvector_ERROR;

	*destination = *source;
	source->data = NULL;

	return cvector_SUCCESS;
}

inline int cvector_move_assign(cvector* destination, cvector* source) {
	cvector_swap(destination, source);
	return cvector_destroy(source);
}

inline int cvector_swap(cvector* destination, cvector* source) {
	void* temp;

	assert(destination != NULL);
	assert(source != NULL);
	assert(cvector_is_initialized(source));
	assert(cvector_is_initialized(destination));

	if (destination == NULL) return cvector_ERROR;
	if (source == NULL) return cvector_ERROR;
	if (!cvector_is_initialized(destination)) return cvector_ERROR;
	if (!cvector_is_initialized(source)) return cvector_ERROR;

	_cvector_swap(&destination->size, &source->size);
	_cvector_swap(&destination->capacity, &source->capacity);
	_cvector_swap(&destination->element_size, &source->element_size);

	temp = destination->data;
	destination->data = source->data;
	source->data = temp;

	return cvector_SUCCESS;
}

inline int cvector_destroy(cvector* cvector) {
	assert(cvector != NULL);

	if (cvector == NULL) return cvector_ERROR;

	free(cvector->data);
	cvector->data = NULL;

	return cvector_SUCCESS;
}

/* Insertion */
inline int cvector_push_back(cvector* cvector, void* element) {
	assert(cvector != NULL);
	assert(element != NULL);

	if (_cvector_should_grow(cvector)) {
		if (_cvector_adjust_capacity(cvector) == cvector_ERROR) {
			return cvector_ERROR;
		}
	}

	_cvector_assign(cvector, cvector->size, element);

	++cvector->size;

	return cvector_SUCCESS;
}

inline int cvector_push_front(cvector* cvector, void* element) {
	return cvector_insert(cvector, 0, element);
}

inline int cvector_insert(cvector* cvector, size_t index, void* element) {
	void* offset;

	assert(cvector != NULL);
	assert(element != NULL);
	assert(index <= cvector->size);

	if (cvector == NULL) return cvector_ERROR;
	if (element == NULL) return cvector_ERROR;
	if (cvector->element_size == 0) return cvector_ERROR;
	if (index > cvector->size) return cvector_ERROR;

	if (_cvector_should_grow(cvector)) {
		if (_cvector_adjust_capacity(cvector) == cvector_ERROR) {
			return cvector_ERROR;
		}
	}

	/* Move other elements to the right */
	if (_cvector_move_right(cvector, index) == cvector_ERROR) {
		return cvector_ERROR;
	}

	/* Insert the element */
	offset = _cvector_offset(cvector, index);
	memcpy(offset, element, cvector->element_size);
	++cvector->size;

	return cvector_SUCCESS;
}

inline int cvector_assign(cvector* cvector, size_t index, void* element) {
	assert(cvector != NULL);
	assert(element != NULL);
	assert(index < cvector->size);

	if (cvector == NULL) return cvector_ERROR;
	if (element == NULL) return cvector_ERROR;
	if (cvector->element_size == 0) return cvector_ERROR;
	if (index >= cvector->size) return cvector_ERROR;

	_cvector_assign(cvector, index, element);

	return cvector_SUCCESS;
}

/* Deletion */
inline int cvector_pop_back(cvector* cvector) {
	assert(cvector != NULL);
	assert(cvector->size > 0);

	if (cvector == NULL) return cvector_ERROR;
	if (cvector->element_size == 0) return cvector_ERROR;

	--cvector->size;

#ifndef cvector_NO_SHRINK
	if (_cvector_should_shrink(cvector)) {
		_cvector_adjust_capacity(cvector);
	}
#endif

	return cvector_SUCCESS;
}

inline int cvector_pop_front(cvector* cvector) {
	return cvector_erase(cvector, 0);
}

inline int cvector_erase(cvector* cvector, size_t index) {
	assert(cvector != NULL);
	assert(index < cvector->size);

	if (cvector == NULL) return cvector_ERROR;
	if (cvector->element_size == 0) return cvector_ERROR;
	if (index >= cvector->size) return cvector_ERROR;

	/* Just overwrite */
	_cvector_move_left(cvector, index);

#ifndef cvector_NO_SHRINK
	if (--cvector->size == cvector->capacity / 4) {
		_cvector_adjust_capacity(cvector);
	}
#endif

	return cvector_SUCCESS;
}

inline int cvector_clear(cvector* cvector) {
	return cvector_resize(cvector, 0);
}

/* Lookup */
inline void* cvector_get(cvector* cvector, size_t index) {
	assert(cvector != NULL);
	assert(index < cvector->size);

	if (cvector == NULL) return NULL;
	if (cvector->element_size == 0) return NULL;
	if (index >= cvector->size) return NULL;

	return _cvector_offset(cvector, index);
}

inline const void* cvector_const_get(const cvector* cvector, size_t index) {
	assert(cvector != NULL);
	assert(index < cvector->size);

	if (cvector == NULL) return NULL;
	if (cvector->element_size == 0) return NULL;
	if (index >= cvector->size) return NULL;

	return _cvector_const_offset(cvector, index);
}

inline void* cvector_front(cvector* cvector) {
	return cvector_get(cvector, 0);
}

inline void* cvector_back(cvector* cvector) {
	return cvector_get(cvector, cvector->size - 1);
}

/* Information */

inline bool cvector_is_initialized(const cvector* cvector) {
	return cvector->data != NULL;
}

inline size_t cvector_byte_size(const cvector* cvector) {
	return cvector->size * cvector->element_size;
}

inline size_t cvector_free_space(const cvector* cvector) {
	return cvector->capacity - cvector->size;
}

inline bool cvector_is_empty(const cvector* cvector) {
	return cvector->size == 0;
}

/* Memory management */
inline int cvector_resize(cvector* cvector, size_t new_size) {
	if (new_size <= cvector->capacity * cvector_SHRINK_THRESHOLD) {
		cvector->size = new_size;
		if (_cvector_reallocate(cvector, new_size * cvector_GROWTH_FACTOR) == -1) {
			return cvector_ERROR;
		}
	} else if (new_size > cvector->capacity) {
		if (_cvector_reallocate(cvector, new_size * cvector_GROWTH_FACTOR) == -1) {
			return cvector_ERROR;
		}
	}

	cvector->size = new_size;

	return cvector_SUCCESS;
}

inline int cvector_reserve(cvector* cvector, size_t minimum_capacity) {
	if (minimum_capacity > cvector->capacity) {
		if (_cvector_reallocate(cvector, minimum_capacity) == cvector_ERROR) {
			return cvector_ERROR;
		}
	}
	return cvector_SUCCESS;
}

inline int cvector_shrink_to_fit(cvector* cvector) {
	return _cvector_reallocate(cvector, cvector->size);
}

/* cvector_iterators */
inline cvector_iterator cvector_begin(cvector* cvector) {
	return cvector_cvector_iterator(cvector, 0);
}

inline cvector_iterator cvector_end(cvector* cvector) {
	return cvector_cvector_iterator(cvector, cvector->size);
}

inline cvector_iterator cvector_cvector_iterator(cvector* cvector, size_t index) {
	cvector_iterator cvector_iterator = {NULL, 0};

	assert(cvector != NULL);
	assert(index <= cvector->size);

	if (cvector == NULL) return cvector_iterator;
	if (index > cvector->size) return cvector_iterator;
	if (cvector->element_size == 0) return cvector_iterator;

	cvector_iterator.pointer = _cvector_offset(cvector, index);
	cvector_iterator.element_size = cvector->element_size;

	return cvector_iterator;
}

inline void* cvector_iterator_get(cvector_iterator* cvector_iterator) {
	return cvector_iterator->pointer;
}

inline int cvector_iterator_erase(cvector* cvector, cvector_iterator* cvector_iterator) {
	size_t index = cvector_iterator_index(cvector, cvector_iterator);

	if (cvector_erase(cvector, index) == cvector_ERROR) {
		return cvector_ERROR;
	}

	*cvector_iterator = cvector_cvector_iterator(cvector, index);

	return cvector_SUCCESS;
}

inline void cvector_iterator_increment(cvector_iterator* cvector_iterator_) {
	assert(cvector_iterator_ != NULL);
	cvector_iterator_->pointer = 
        ((byte*)cvector_iterator_->pointer) + cvector_iterator_->element_size;
}

inline void cvector_iterator_decrement(cvector_iterator* cvector_iterator_) {
	assert(cvector_iterator_ != NULL);
	cvector_iterator_->pointer =
        ((byte*)cvector_iterator_->pointer) - cvector_iterator_->element_size;
}

inline void* cvector_iterator_next(cvector_iterator* cvector_iterator_) {
	void* current = cvector_iterator_->pointer;
	cvector_iterator_increment(cvector_iterator_);

	return current;
}

inline void* cvector_iterator_previous(cvector_iterator* cvector_iterator_) {
	void* current = cvector_iterator_->pointer;
	cvector_iterator_decrement(cvector_iterator_);

	return current;
}

inline bool cvector_iterator_equals(cvector_iterator* first, cvector_iterator* second) {
	assert(first->element_size == second->element_size);
	return first->pointer == second->pointer;
}

inline bool cvector_iterator_is_before(cvector_iterator* first, cvector_iterator* second) {
	assert(first->element_size == second->element_size);
	return first->pointer < second->pointer;
}

inline bool cvector_iterator_is_after(cvector_iterator* first, cvector_iterator* second) {
	assert(first->element_size == second->element_size);
	return first->pointer > second->pointer;
}

inline size_t cvector_iterator_index(cvector* cvector, cvector_iterator* cvector_iterator_) {
	assert(cvector != NULL);
	assert(cvector_iterator_ != NULL);
	return ((byte*)cvector_iterator_->pointer - (byte*)cvector->data) / cvector->element_size;
}

/***** PRIVATE *****/

inline bool _cvector_should_grow(cvector* cvector) {
	assert(cvector->size <= cvector->capacity);
	return cvector->size == cvector->capacity;
}

inline bool _cvector_should_shrink(cvector* cvector) {
	assert(cvector->size <= cvector->capacity);
	return cvector->size == cvector->capacity * cvector_SHRINK_THRESHOLD;
}

inline size_t _cvector_free_bytes(const cvector* cvector) {
	return cvector_free_space(cvector) * cvector->element_size;
}

inline void* _cvector_offset(cvector* cvector, size_t index) {
	return (byte*)cvector->data + (index * cvector->element_size);
}

inline const void* _cvector_const_offset(const cvector* cvector, size_t index) {
	return (byte*)cvector->data + (index * cvector->element_size);
}

inline void _cvector_assign(cvector* cvector, size_t index, void* element) {
	/* Insert the element */
	void* offset = _cvector_offset(cvector, index);
	memcpy(offset, element, cvector->element_size);
}

inline int _cvector_move_right(cvector* cvector, size_t index) {
	assert(cvector->size < cvector->capacity);

	/* The location where to start to move from. */
	void* offset = _cvector_offset(cvector, index);

	/* How many to move to the right. */
	size_t elements_in_bytes = (cvector->size - index) * cvector->element_size;

#ifdef __STDC_LIB_EXT1__
	size_t right_capacity_in_bytes =
			(cvector->capacity - (index + 1)) * cvector->element_size;

	/* clang-format off */
	int return_code =  memmove_s(
		offset + cvector->element_size,
		right_capacity_in_bytes,
		offset,
		elements_in_bytes
	);
	/* clang-format on */

	return return_code == 0 ? cvector_SUCCESS : cvector_ERROR;

#else
	memmove((byte*)offset + cvector->element_size, (byte*)offset, elements_in_bytes);
	return cvector_SUCCESS;
#endif
}

inline void _cvector_move_left(cvector* cvector, size_t index) {
	size_t right_elements_in_bytes;
	void* offset;

	/* The offset into the memory */
	offset = _cvector_offset(cvector, index);

	/* How many to move to the left */
	right_elements_in_bytes = (cvector->size - index - 1) * cvector->element_size;

	memmove((byte*)offset, (byte*)offset + cvector->element_size, right_elements_in_bytes);
}

inline int _cvector_adjust_capacity(cvector* cvector) {
	return _cvector_reallocate(cvector,
		SAKURA_MAX(1, cvector->size * cvector_GROWTH_FACTOR));
}

inline int _cvector_reallocate(cvector* cvector, size_t new_capacity) {
	size_t new_capacity_in_bytes;
	void* old;
	assert(cvector != NULL);

	if (new_capacity < cvector_MINIMUM_CAPACITY) {
		if (cvector->capacity > cvector_MINIMUM_CAPACITY) {
			new_capacity = cvector_MINIMUM_CAPACITY;
		} else {
			/* NO-OP */
			return cvector_SUCCESS;
		}
	}

	new_capacity_in_bytes = new_capacity * cvector->element_size;
	old = cvector->data;

	if ((cvector->data = malloc(new_capacity_in_bytes)) == NULL) {
		return cvector_ERROR;
	}

#ifdef __STDC_LIB_EXT1__
	/* clang-format off */
	if (memcpy_s(cvector->data,
							 new_capacity_in_bytes,
							 old,
							 cvector_byte_size(cvector)) != 0) {
		return cvector_ERROR;
	}
/* clang-format on */
#else
	memcpy(cvector->data, old, cvector_byte_size(cvector));
#endif
	cvector->capacity = new_capacity;

	::free(old);

	return cvector_SUCCESS;
}

inline void _cvector_swap(size_t* first, size_t* second) {
	size_t temp = *first;
	*first = *second;
	*second = temp;
}

}