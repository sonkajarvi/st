#ifndef ST_UTILITY_STRINGBUILDER_H
#define ST_UTILITY_STRINGBUILDER_H

#include <stddef.h>

#include <st/utility/list.h>

struct st_strbuilder
{
    struct st_lsnode *head;
};

// st_strbuilder_free - Free string builder
// @sb: Pointer to string builder
void st_strbuilder_free(struct st_strbuilder *sb);

// st_strbuilder_append - Append a string to string builder
// @sb: Pointer to string builder
// @str: String to append
//
// note: Does nothing if 'str' is NULL or empty.
// note: 'str' must stay valid until 'st_strbuilder_concat' is called
void st_strbuilder_append(struct st_strbuilder *sb, const char *str);

// st_strbuilder_concat - Concatenate all strings in string builder
// @sb: Pointer to string builder
//
// note: Returns NULL if string builder is empty.
// note: Returned string is heap allocated and must be freed
char *st_strbuilder_concat(struct st_strbuilder *sb);

// st_strbuilder_length - Get length of string in string builder
// @sb: Pointer to string builder
//
// todo: Modifying a string after appending it might cause the length
//       to be incorrect, since lengths are only calculated during appends.
//
//       Should all lengths be (re)calculated everytime this is called?
size_t st_strbuilder_length(struct st_strbuilder *sb);

#endif // ST_UTILITY_STRINGBUILDER_H
