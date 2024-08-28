#include <st/vector.h>

#include <stdlib.h>

size_t __vector_calc_cap(void *v, const size_t add_len)
{
    const size_t cap = v ? vector_capacity(v) : VECTOR_DEFAULT_CAPACITY;
    size_t new_cap = cap;
    while (new_cap < vector_length(v) + add_len)
        new_cap = __vector_f(new_cap);
    
    return new_cap;
}

void *__vector_realloc(void *v, const size_t cap, const size_t type_width)
{
    const size_t header_size = sizeof(StVectorHeader);

    void *tmp = realloc(v ? __vector_header(v) : NULL, header_size + (cap * type_width));
    tmp = (char *)tmp + header_size;
    
    if (!v)
        __vector_header(tmp)->length = 0;
    __vector_header(tmp)->capacity = cap;

    return tmp;
}
