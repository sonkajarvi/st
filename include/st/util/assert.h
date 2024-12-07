#ifndef ST_UTILITY_ASSERT_H
#define ST_UTILITY_ASSERT_H

#include <st/util/compiler.h>

#define st_assert(x) ({  \
    if (!st_unlikely(x)) \
        __st_assert_exit(#x, __FILE_NAME__, __func__, __LINE__); })

ST_NORETURN
void __st_assert_exit(const char *const expr, const char *const file,
                      const char *const func, const int line);

#endif // ST_UTILITY_ASSERT_H
