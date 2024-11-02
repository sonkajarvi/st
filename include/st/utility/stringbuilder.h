#ifndef ST_UTILITY_STRINGBUILDER_H
#define ST_UTILITY_STRINGBUILDER_H

#include <stddef.h>

#include <st/utility/list.h>

struct st_strbuilder
{
    struct st_lsnode *head;
    size_t length;
};

// st_strbuilder_free - Free string builder
// @sb: Pointer to string builder
void st_strbuilder_free(struct st_strbuilder *sb);

// st_strbuilder_append - Append a string to string builder
// @sb: Pointer to string builder
// @str: String to append
//
// note: 'sb' takes ownership of 'str', until st_strbuilder_free() is called.
// note: Does nothing if 'str' is NULL or empty
void st_strbuilder_append(struct st_strbuilder *sb, const char *str);

// st_strbuilder_insert - Insert a string at index in string builder
// @sb: Pointer to string builder
// @index: Index to insert at
// @str: String to insert
//
// note: 'sb' takes ownership of 'str', until st_strbuilder_free() is called.
// note: Does nothing if 'str' is NULL or empty.
// note: If 'index' is out of bounds, 'str' is appended instead
void st_strbuilder_insert(struct st_strbuilder *sb, const size_t index, const char *str);

// st_strbuilder_concat - Concatenate all strings in string builder
// @sb: Pointer to string builder
//
// note: Returns NULL if string builder is empty.
// note: Returned string is heap allocated and must be freed
char *st_strbuilder_concat(struct st_strbuilder *sb);

#endif // ST_UTILITY_STRINGBUILDER_H
