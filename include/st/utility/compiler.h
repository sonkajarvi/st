#ifndef ST_UTILITY_COMPILER_H
#define ST_UTILITY_COMPILER_H

#define pragma_helper(x) _Pragma(#x)
#define pragma(x) pragma_helper(x)

#define st_ignore_diagnostic(rule, expr)     \
    do {                                     \
        pragma(GCC diagnostic push);         \
        pragma(GCC diagnostic ignored rule); \
        expr;                                \
        pragma(GCC diagnostic pop);          \
    } while (0)

#define st_lambda(__type, __args, __body) ({ \
    __type __id __args \
    __body \
    &__id; \
})

#define st_likely(x) __builtin_expect(!!(x), 1)
#define st_unlikely(x) __builtin_expect(!!(x), 0)

#define ST_NORETURN __attribute__((noreturn))

#endif // ST_UTILITY_COMPILER_H
