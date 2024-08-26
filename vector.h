#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_DEFAULT_CAPACITY 4

#define vector_is_empty(v)  ((v) ? vector_length((v)) == 0 : 1)
#define vector_length(v)    ((v) ? vector_header((v))->length : 0)
#define vector_capacity(v)  ((v) ? vector_header((v))->capacity : 0)
#define vector_front(v)     (*vector_frontref((v)))
#define vector_back(v)      (*vector_backref((v)))
#define vector_frontref(v)  (v)
#define vector_backref(v)   ((v) ? (v) + vector_length((v)) - 1 : NULL)

#define vector_push(v, value) (vector_grow((v), 1), (v)[vector_header(v)->length++] = (n));

#define vector_clear(v) (vector_header((v))->length = 0)

#define vector_free(v) (free(vector_header((v))), (v) = NULL)

#define vector_grow(v, new_len) \
    (!(v) || vector_capacity((v)) < (vector_length((v)) + (new_len)) \
        ? (v) = vector_realloc((v), (v) ? 2 * vector_capacity((v)) : VECTOR_DEFAULT_CAPACITY, sizeof(*(v))) \
        : 0)

#define vector_header(v) ((StVectorHeader *)(v) - 1)

typedef struct StVectorHeader
{
    size_t length;
    size_t capacity;
} StVectorHeader;

void *vector_realloc(void *v, const size_t bytes, const size_t byte_width);

#endif // VECTOR_H
