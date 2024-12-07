#ifndef ST_UTILITY_VECTOR_H
#define ST_UTILITY_VECTOR_H

/*
 * st_vector implements the following:
 *
 * Read-only:
 *  st_vector_empty
 *  st_vector_length
 *  st_vector_capacity
 *  st_vector_at
 *
 * Iterators:
 *  st_vector_begin
 *  st_vector_end
 *  st_vector_for
 *  st_vector_rof
 *
 * Modifiers:
 *  st_vector_push
 *  st_vector_push_range
 *  todo: st_vector_push_vector
 *  st_vector_insert
 *  todo: st_vector_insert_range
 *  st_vector_insert_swap
 *
 *  st_vector_pop
 *  st_vector_remove
 *  todo: st_vector_remove_range
 *  st_vector_remove_swap
 *
 *  st_vector_reserve
 *  st_vector_resize
 *  st_vector_shrink
 *  st_vector_clear
 *  st_vector_free
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// st_vector_empty - Check if vector is empty
// @v: Vector
#define st_vector_empty(v) ({ \
    (v) ? st_vector_length(v) == 0 : 1; })

// st_vector_length - Get length of vector
// @v: Vector
//
// note: Returns 0 if vector is NULL
#define st_vector_length(v) ({ \
    (v) ? __vector_header(v)->length : 0; })

// st_vector_capacity - Get capacity of vector
// @v: Vector
//
// note: Returns 0 if 'v' is NULL
#define st_vector_capacity(v) ({ \
    (v) ? __vector_header(v)->capacity : 0; })

// st_vector_at - Get element at index
// @v: Vector
// @index: Index
//
// note: Returns NULL if 'v' is NULL or 'index' is out of bounds
#define st_vector_at(v, index) ({   \
    const size_t __index = (index); \
    (v) && __index < st_vector_length(v) ? (v) + __index : NULL; })

// st_vector_begin - Get pointer to first element of vector
// @v: Vector
#define st_vector_begin(v) (v)

// st_vector_end - Get pointer to last element of vector
// @v: Vector
#define st_vector_end(v) ({ \
    (v) ? (v) + st_vector_length(v) - 1 : NULL; })

// st_vector_for - Iterate over vector
// @v: Vector
// @it: Iterator name
#define st_vector_for(v, it)                    \
    for (__typeof__(v) it = st_vector_begin(v); \
        it < ((v) ? st_vector_end(v) + 1 : NULL); it++)

// st_vector_rof - Iterate over vector in reverse
// @v: Vector
// @it: Iterator name
#define st_vector_rof(v, it)                  \
    for (__typeof__(v) it = st_vector_end(v); \
        it > ((v) ? st_vector_begin(v) - 1 : NULL); it--)

// st_vector_push - Push value to vector
// @v: Vector
// @value: Value to add
//
// note: Returns a pointer to added element
#define st_vector_push(v, ...) ({                    \
    __vector_grow(v, 1);                             \
    (v)[__vector_header(v)->length++] = __VA_ARGS__; \
    st_vector_end(v); })

// st_vector_push_range - Push range of values to vector
// @v: Vector
// @ptr: Pointer to array
// @len: Length of array
//
// note: Does nothing if 'ptr' is NULL or 'len' is 0
#define st_vector_push_range(v, ptr, len) ({                             \
    const size_t __len = (len);                                          \
    const __typeof__(v) __ptr = (ptr);                                   \
    __ptr && __len > 0                                                   \
        ? (__vector_grow(v, __len),                                      \
            memmove(v + st_vector_length(v), __ptr, __len * sizeof(*v)), \
            __vector_header(v)->length += __len, 0)                      \
        : 0; })

// st_vector_insert - Insert value at index
// @v: Vector
// @index: Index to insert at
// @value: Value to add
//
// note: Does a push if 'index' is greater or equal to length
// note: Returns a pointer to added element
#define st_vector_insert(v, index, value) ({                       \
    const size_t __index = (index);                                \
    __index >= st_vector_length(v)                                 \
        ? st_vector_push(v, value)                                 \
        : (__vector_grow(v, 1),                                    \
           memmove(v + __index + 1, v + __index,                   \
           (__vector_header(v)->length++ - __index) * sizeof(*v)), \
           v[__index] = value,                                     \
           v + __index); })

// st_vector_insert_swap - Insert value at index, by swapping with the last element
// @v: Vector
// @index: Index to insert at
// @value: Value to add
//
// note: Does a push if 'index' is greater or equal to length
// note: Returns a pointer to added element
#define st_vector_insert_swap(v, index, value) ({ \
    const size_t __index = (index);               \
    __index >= st_vector_length(v)                \
        ? st_vector_push(v, (value))              \
        : (st_vector_push(v, v[__index]),         \
           v[__index] = (value),                  \
           v + __index); })

// st_vector_pop - Pop element from vector
// @v: Vector
#define st_vector_pop(v) \
    (st_vector_length(v) > 0 ? (__vector_header(v)->length--, 0) : 0)

// st_vector_remove - Remove element at index
// @v: Vector
// @index: Index
//
// note: Does a pop if 'index' is the last element
#define st_vector_remove(v, index) ({                                      \
    const size_t __index = (index);                                        \
    __index == st_vector_length(v) - 1                                     \
        ? st_vector_pop(v)                                                 \
        : __index < st_vector_length(v) - 1                                \
            ? (memcpy(&v[__index], &v[__index + 1],                        \
                (--__vector_header(v)->length - __index) * sizeof(*v)), 0) \
            : 0; })

// st_vector_remove_swap - Remove element at index, by swapping with the last element
// @v: Vector
// @index: Index
//
// note: Does a pop if 'index' is the last element
#define st_vector_remove_swap(v, index) ({       \
    const size_t __index = (index);              \
    __index == st_vector_length(v) - 1           \
        ? st_vector_pop(v)                       \
        : __index < st_vector_length(v) - 1      \
            ? (v[__index] = *st_vector_end(v),  \
                __vector_header(v)->length--, 0) \
            : 0; })

// st_vector_reserve - Reserve capacity for vector
// @v: Vector
// @new_cap: New capacity
//
// note: Does nothing if 'new_cap' is less than current capacity
#define st_vector_reserve(v, new_cap) ({                    \
    const size_t __new_cap = new_cap;                       \
    __typeof__(v) __tmp = NULL;                             \
    if (__new_cap > st_vector_capacity(v))                  \
        __tmp = __vector_realloc(v, __new_cap, sizeof(*v)); \
    __tmp ? (v = __tmp, 0) : 0; })

// st_vector_resize - Resize vector to new capacity
// @v: Vector
// @new_cap: New capacity
//
// note: Frees 'v' if 'new_cap' is 0
#define st_vector_resize(v, new_cap) ({ \
    const size_t __new_cap = new_cap; \
    __new_cap == 0 \
        ? st_vector_free(v) \
        : (v = __vector_realloc(v, __new_cap, sizeof(*v)), \
           __vector_header(v)->capacity = __new_cap, 0); })

// st_vector_shrink - Shrink vector capacity to fit length
// @v: Vector
//
// note: Frees 'v' if length is 0
#define st_vector_shrink(v) ({                  \
    st_vector_capacity(v) > st_vector_length(v) \
        ? st_vector_resize(v, st_vector_length(v)) \
        : 0; })

// st_vector_clear - Clear vector
// @v: Vector
//
// note: Does not free any memory
#define st_vector_clear(v) ({ \
    v ? (__vector_header(v)->length = 0, 0) : 0; })

// st_vector_free - Free vector
// @v: Vector
//
// note: Sets 'v' to NULL
#define st_vector_free(v) ({ \
    v ? (free(__vector_header(v)), v = NULL, 0) : 0; })

#define __vector_header(v) ((struct st_vector_header *)(v) - 1)

#define __vector_grow(v, add_len)                                        \
    (!(v) || st_vector_capacity((v)) < st_vector_length((v)) + (add_len) \
        ? (v) = __vector_realloc((v), __vector_calc_cap((v), (add_len)), sizeof(*(v))) \
        : 0)

struct st_vector_header
{
    size_t length;
    size_t capacity;
};

size_t __vector_calc_cap(void *v, const size_t add_len);
void *__vector_realloc(void *v, const size_t cap, const size_t type_width);

#endif // ST_UTILITY_VECTOR_H
