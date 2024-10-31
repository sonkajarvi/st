#include <stdlib.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>

ST_NORETURN void _st_assert_exit(const char *expr,
    const char *func, const int line, const char *file)
{
    st_fprint(stderr,
        ST_PRINT_ASSERT "'%s', in %s(), at %s:%d\n",
        expr, func, file, line);

    exit(1);
}
