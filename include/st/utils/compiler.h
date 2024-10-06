#ifndef ST_UTILS_COMPILER_H
#define ST_UTILS_COMPILER_H

#define pragma_helper(x) _Pragma(#x)
#define pragma(x) pragma_helper(x)

#define st_ignore_diagnostic(rule, expr)     \
    do {                                     \
        pragma(GCC diagnostic push);         \
        pragma(GCC diagnostic ignored rule); \
        expr;                                \
        pragma(GCC diagnostic pop);          \
    } while (0)

#endif // ST_UTILS_COMPILER_H
