#ifndef ST_UTILITY_ASSERT_H
#define ST_UTILITY_ASSERT_H

#include <st/utility/compiler.h>

#define st_assert(x) ({ \
    if (!st_unlikely(x)) \
        _st_assert_exit(#x, __func__, __LINE__, __FILE__); \
})

ST_NORETURN void _st_assert_exit(const char *expr, const char *function,
    const int line, const char *file);

#endif // ST_UTILITY_ASSERT_H
