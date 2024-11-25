#include <stdlib.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>

void __st_assert_exit(const char *const expr, const char *const file,
                      const char *const func, const int line)
{
    st_print(ST_PRINT_ASSERT, file, func, line, "\"%s\"\n", expr);
    abort();
}
