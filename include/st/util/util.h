#ifndef ST_UTILITY_UTIL_H
#define ST_UTILITY_UTIL_H

#include <stddef.h> // offsetof

// todo: Move somewhere else
#define st_container_of(ptr, type, member) ({             \
    const __typeof__(((type *)0)->member) *__ptr = (ptr); \
    (type *)((char *)__ptr - offsetof(type, member));     \
})

char *read_file(const char *path);

#endif // ST_UTILITY_UTIL_H
