#ifndef ST_UTILS_VECTOR_H
#define ST_UTILS_VECTOR_H

/*
 * vector_is_empty
 * vector_length
 * vector_capacity
 * vector_front
 * vector_back
 * vector_at
 *
 * vector_push
 * vector_pop
 * vector_insert
 * vector_remove
 * vector_push_range
 * vector_push_copy
 * vector_reserve
 * vector_resize (todo)
 * vector_shrink (todo)
 * vector_clear
 * vector_free
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <st/utils/minmax.h>

#define VECTOR_DEFAULT_CAPACITY 2

/**
 * @brief Checks if vector is empty
 *
 * @param v Vector
 *
 * @returns 1 if empty, 0 if not
 */
#define vector_is_empty(v) \
    (v ? vector_length(v) == 0 : 1)

/**
 * @brief Returns the length of a vector
 *
 * @param v Vector
 *
 * @returns Vector length, or 0 if vector is NULL
 */
#define vector_length(v) \
    (v ? __vector_header((v))->length : 0)

/**
 * @brief Returns the capacity of a vector
 *
 * @param v Vector
 *
 * @returns Vector capacity, or 0 if vector is NULL
 */
#define vector_capacity(v) \
    (v ? __vector_header(v)->capacity : 0)

/**
 * @brief Returns the first element of a vector
 *
 * @param v Vector
 *
 * @returns Pointer to first element, i.e. the vector itself
 */
#define vector_front(v) v

/**
 * @brief Returns the last element of a vector
 *
 * @param v Vector
 *
 * @returns Pointer to last element, or NULL if vector is NULL
 */
#define vector_back(v) \
    (v ? v + vector_length(v) - 1 : NULL)

/**
 * @brief Returns the element at the given index
 *
 * @param v Vector
 * @param index Index
 *
 * @returns Pointer to element,
 *          or NULL if vector is NULL or index is out of bounds
 */
#define vector_at(v, index) ({ \
    const size_t __idx = (index); \
    (v && (__idx) < vector_length(v) ? v + __idx : NULL); })

/**
 * @brief Iterates over a vector
 *
 * @param v Vector
 * @param type Iterator type
 * @param name Iterator name
 */
#define vector_for(v, type, name) \
    for (type *name = vector_front(v); name < (v ? vector_back(v) + 1 : NULL); name++)

/**
 * @brief Iterates over a vector in reverse
 *
 * @param v Vector
 * @param type Iterator type
 * @param name Iterator name
 */
#define vector_rof(v, type, name) \
    for (type *name = vector_back(v); name > (v ? vector_front(v) - 1 : NULL); name--)

/**
 * @brief Pushes a value to a vector
 *
 * @param v Vector
 * @param value Value
 *
 * @returns Pointer to value on success, or NULL
 */
#define vector_push(v, value) ({ \
    __vector_grow(v, 1); \
    v[__vector_header(v)->length++] = (value); \
    vector_back(v); })

/**
 * @brief Pops the last value from a vector
 *
 * @param v Vector
 *
 * @returns 1 on success, 0 if vector is null or empty
 */
#define vector_pop(v) \
    (vector_length(v) > 0 ? (__vector_header(v)->length--, 1) : 0)

/**
 * @brief Inserts a value at the given index
 *
 * @param v Vector
 * @param index Index
 * @param value Value
 *
 * @returns Pointer to value on success, or NULL
 *
 * @note Index greater or equal to length results in a push
 */
#define vector_insert(v, index, value) ({ \
    const size_t __index = (index); \
    __index >= vector_length(v) \
        ? vector_push(v, value) \
        : (__vector_grow(v, 1), \
           memmove(v + __index + 1, v + __index, \
           (__vector_header(v)->length++ - __index) * sizeof(*v)), \
           v[__index] = value, \
           v + __index); })

/**
 * @brief Inserts a value at the given index, by swapping with the last element
 *
 * @param v Vector
 * @param index Index
 * @param value Value
 *
 * @returns Pointer to value on success, or NULL
 *
 * @note Index greater or equal to length results in a push
 */
#define vector_insert_swap(v, index, value) ({ \
    const size_t __index = (index); \
    __index >= vector_length(v) \
        ? vector_push(v, value) \
        : (vector_push(v, v[__index]), \
           v[__index] = value, \
           v + __index); })

/**
 * @brief remove a value at index from a vector
 *
 * @param v vector
 * @param index index
 *
 * @note does a pop, if index is the back
 */
#define vector_remove(v, index) \
    ((index) == vector_length((v)) - 1 \
        ? vector_pop((v)) \
        : (index) < vector_length((v)) - 1 \
            ? (memmove((v) + (index), (v) + (index) + 1, \
                (__vector_header((v))->length-- - (index)) * sizeof(*(v))), 0) \
            : 0)

/**
 * @brief remove a value at index by swapping with the last element
 *
 * @param v vector
 * @param index index
 */
#define vector_remove_swap(v, index) \
    ((index) == vector_length((v)) - 1 \
        ? vector_pop((v)) \
        : (index) < vector_length((v)) - 1 \
            ? ((v)[(index)] = *vector_back((v)), __vector_header((v))->length--, 0) \
            : 0)

/**
 * @brief push a range of values to a vector
 *
 * @param v vector
 * @param ptr array
 * @param len array length
 *
 * @note does nothing, if ptr is null or len is less than 1
 *
 * @todo fix nonnull warning on linux
 */
#define vector_push_range(v, ptr, len) \
    ((ptr) && (len) > 0 \
        ? __vector_grow((v), (len)), \
            memmove((v) + vector_length((v)), (ptr), (len) * sizeof(*(ptr))), \
            __vector_header((v))->length += (len) \
        : 0)

/**
 * @brief push a value to a vector using memmove
 *
 * @param v vector
 * @param value value
 */
#define vector_push_copy(v, value) \
    (__vector_grow((v), 1), \
        memmove((v) + vector_length((v)), &(value), sizeof((value))), \
        __vector_header((v))->length++)

/**
 * @brief Reserves capacity for a vector
 *
 * @param v Vector
 * @param new_cap New capacity
 *
 * @returns 1 on success, 0 on failure
 *
 * @note If reallocation fails, the original vector is not modified
 */
#define vector_reserve(v, new_cap) ({                                  \
    const size_t __new_cap = st_max(new_cap, VECTOR_DEFAULT_CAPACITY); \
    __typeof__(v) __tmp = NULL;                                        \
    if (__new_cap > vector_capacity(v))                                \
        __tmp = __vector_realloc(v, __new_cap, sizeof(*v));            \
    __tmp ? (v = __tmp, 1) : 0; })

/**
 * @brief Clears a vector, i.e. sets the length to 0
 *
 * @param v Vector
 *
 * @returns 1 on success, 0 if vector is NULL
 */
#define vector_clear(v) ({ \
    (v ? (__vector_header(v)->length = 0, 1) : 0); })

/**
 * @brief Frees a vector
 *
 * @param v Vector
 *
 * @returns 1 on success, 0 if vector is NULL
 *
 * @note Sets vector to NULL, if not already
 */
#define vector_free(v) ({ \
    (v ? (free(__vector_header(v)), v = NULL, 1) : 0); })

#define __vector_header(v) ((StVectorHeader *)(v) - 1)

#define __vector_dbg(v) \
    printf("(%lld %lld)\n", vector_length((v)), vector_capacity((v)))

#define __vector_dbg2(v, fmt) \
    do { \
        for (size_t i = 0; i < vector_length((v)); i++) \
            printf(fmt" ", v[i]); \
        printf("\n"); \
    } while (0)

#define __vector_grow(v, add_len) \
    (!(v) || vector_capacity((v)) < vector_length((v)) + (add_len) \
        ? (v) = __vector_realloc((v), __vector_calc_cap((v), (add_len)), sizeof(*(v))) \
        : 0)

typedef struct StVectorHeader
{
    size_t length;
    size_t capacity;
} StVectorHeader;

size_t __vector_calc_cap(void *, const size_t);
void *__vector_realloc(void *, const size_t, const size_t);

#endif // ST_UTILS_VECTOR_H
