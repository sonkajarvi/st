#ifndef ST_UTIL_STRING_H
#define ST_UTIL_STRING_H

// Read-only:
//  st_string_empty
//  st_string_length
//  st_string_capacity
//  st_string_first
//  st_string_last
//  st_string_at

// set
//  st_string_set_char
//  st_string_append
// append_char
// insert
// insert_char

// pop
// remove
// remove_range

// reserve
// resize
// shrink
// clear
//  st_string_free

// copy
// find
// find_first
// find_last
// starts_with
// ends_with

#include <stdbool.h>

#include <st/utility/compiler.h>
#include <st/utility/vector.h>

typedef char *st_string;

// st_string_empty - Check if string is empty
// @str: String
//
// note: Returns true if 'str' is NULL
ST_NODISCARD static inline bool st_string_empty(const st_string str)
{
    return st_vector_empty(str);
}

// st_string_length - Get length of string
// @str: String
ST_NODISCARD static inline size_t st_string_length(const st_string str)
{
    return st_vector_length(str);
}

// st_string_capacity - Get capacity of string
// @str: String
ST_NODISCARD static inline size_t st_string_capacity(const st_string str)
{
    return st_vector_capacity(str);
}

// st_string_front - Get first character of string
// @str: String
//
// note: Returns 0 if 'str' is NULL or empty
ST_NODISCARD static inline char st_string_first(const st_string str)
{
    if (!str)
        return 0;
    
    return str[0];
}

// st_string_back - Get last character of string
// @str: String
//
// note: Returns 0 if 'str' is NULL or empty
ST_NODISCARD static inline char st_string_last(const st_string str)
{
    if (!str || !str[0])
        return 0;

    return str[st_string_length(str) - 1];
}

// st_string_at - Get character at index
// @str: String
// @index: Index to get
//
// note: Returns 0 if 'str' is NULL or 'index' is out of bounds
ST_NODISCARD static inline char st_string_at(const st_string str, const size_t index)
{
    if (!str || index >= st_string_length(str))
        return 0;

    return str[index];
}

// st_string_set_char - Set character at index
// @str: Pointer to string
// @index: Index to set at
// @c: Character to set
//
// note: Adjusts length if 'c' is 0
// note: Does nothing if 'str' is NULL or 'index' is greater than length
void st_string_set_char(st_string *str, const size_t index, const char c);

// st_string_append - Append string to string
// @str: Pointer to string
// @src: String to add
void st_string_append(st_string *str, const char *src);

// st_string_reserve - Reserve capacity for string
// @str: Pointer to string
// @bytes: Amount to reserve
void st_string_reserve(st_string *str, const size_t bytes);

static inline void st_string_free(st_string *str)
{
    if (!str)
        return;

    st_vector_free(*str);
}

#endif // ST_UTIL_STRING_H
