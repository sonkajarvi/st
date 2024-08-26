#include "vector.h"

#include <stdlib.h>

void *vector_realloc(void *v, const size_t bytes, const size_t byte_width)
{
    const size_t header_size = sizeof(StVectorHeader);

    void *tmp = realloc(v ? vector_header(v) : NULL, header_size + (bytes * byte_width));
    tmp = (char *)tmp + header_size;
    
    if (!v)
        vector_header(tmp)->length = 0;
    vector_header(tmp)->capacity = bytes;

    return tmp;
}
