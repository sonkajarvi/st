#include <stdlib.h>

#include <st/utility/vector.h>

static inline size_t __new_cap_fn(const size_t cap)
{
    if (cap == 1)
        return 2;

    return cap + (cap >> 1) + (cap >> 3);
}

size_t __vector_calc_cap(void *v, const size_t add_len)
{
    size_t new_cap = v ? st_vector_capacity(v) : 1;
    while (new_cap < st_vector_length(v) + add_len)
        new_cap = __new_cap_fn(new_cap);
    
    return new_cap;
}

void *__vector_realloc(void *v, const size_t cap, const size_t type_width)
{
    const size_t header_size = sizeof(struct st_vector_header);

    void *tmp = realloc(v ? __vector_header(v) : NULL, header_size + (cap * type_width));
    tmp = (char *)tmp + header_size;
    
    if (!v)
        __vector_header(tmp)->length = 0;
    __vector_header(tmp)->capacity = cap;

    return tmp;
}
