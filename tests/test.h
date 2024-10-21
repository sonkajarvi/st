#ifndef TEST_H
#define TEST_H

#ifdef ST_TESTS_EXIT_ON_FAILURE
#define __test_return_or_not(status) if (status) return status
#else
#define __test_return_or_not(status)
#endif

#define __TEST_SUCCESS 0
#define __TEST_FAILURE 1

#define __TEST_FUNCTION(x) __func_##x
#define __TEST_NAME(x) __name_##x

#include <st/utility/terminal.h>

#define __TEST_SUCCESS_FMT \
    ST_ESC_YELLOW "%s:" \
    ST_ESC_BOLD ST_ESC_GREEN_B " passed" ST_ESC_RESET "\n"

#define __TEST_FAILURE_FMT \
    ST_ESC_YELLOW "%s:"\
    ST_ESC_BOLD ST_ESC_RED_B " failed" \
    ST_ESC_RESET ST_ESC_BLACK_B " (assert '%s', in %s:%d)" ST_ESC_RESET "\n"

#define __test_run(test) ({ \
    const StTestResult __result = test.function(); \
    if (__result.status == __TEST_SUCCESS) \
        printf(__TEST_SUCCESS_FMT, test.name); \
    else \
        printf(__TEST_FAILURE_FMT, \
            test.name, __result.expr, __result.file, __result.line); \
    __test_return_or_not(__result.status); }) \

#define test_declare(name) ({ \
    StTestResult __TEST_FUNCTION(name)(void); \
    StTest __TEST_NAME(name) = { __TEST_FUNCTION(name), #name }; \
    __test_run(__TEST_NAME(name)); })

#define test_case(x) StTestResult __TEST_FUNCTION(x)(void)

#define test_assert(expr) if (!(expr)) \
    return (StTestResult){ __TEST_FAILURE, __LINE__, #expr, __FILE__ }

#define test_success() return (StTestResult){ __TEST_SUCCESS, 0, 0, 0 }

typedef struct StTestResult
{
    int status, line;
    const char *expr, *file;
} StTestResult;

typedef struct StTest
{
    StTestResult (*function)(void);
    const char *name;
} StTest;

#endif // TEST_H
