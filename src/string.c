#include <st/utility/string.h>

void st_string_set_char(st_string *str, const size_t index, const char c)
{
    if (!str)
        return;

    const size_t len = st_string_length(*str);
    if (index >= len) {
        if (index == len)
            st_vector_push(*str, c);
        
        return;
    }

    if (c == '\0')
        __vector_header(*str)->length = index;
    
    (*str)[index] = c;
}

void st_string_append(st_string *str, const char *src)
{
    if (!str || !src || !src[0])
        return;

    const size_t str_len = st_vector_length(*str);
    const size_t src_len = strlen(src);

    st_vector_reserve(*str, str_len + src_len + 1);
    memcpy((*str) + str_len + 1, src + 1, src_len + 1);

    __vector_header(*str)->length += src_len;
    (*str)[str_len] = src[0];
}

void st_string_reserve(st_string *str, const size_t bytes)
{
    if (!str)
        return;

    if (*str) {
        st_vector_reserve(*str, bytes);
    } else {
        st_vector_reserve(*str, bytes);
        (*str)[0] = '\0';
    }
}
