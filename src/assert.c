#include <stdio.h>
#include <stdlib.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>
#include <st/utility/terminal.h>

ST_NORETURN void _st_assert_exit(const char *expr, const char *function,
    const int line, const char *file)
{
    _st_print_time(stderr);

    fprintf(stderr, ST_ESC_BOLD ST_ESC_RED_B " **ASSERT FAILED**: '%s', in %s(), at %s:%d" ST_ESC_RESET "\n", expr, function, file, line);

    exit(1);
}
