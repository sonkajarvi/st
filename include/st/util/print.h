#ifndef ST_UTILITY_PRINT_H
#define ST_UTILITY_PRINT_H

#define ST_PRINT_DEBUG  0
#define ST_PRINT_LOG    1
#define ST_PRINT_WARN   2
#define ST_PRINT_ERROR  3
#define ST_PRINT_ASSERT 4

#define __ST_PRINT_HELPER(level, fmt, ...) \
    st_print(level, __FILE_NAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define st_debug(fmt, ...) __ST_PRINT_HELPER(ST_PRINT_DEBUG, fmt, ##__VA_ARGS__)
#define st_log(fmt, ...)   __ST_PRINT_HELPER(ST_PRINT_LOG, fmt, ##__VA_ARGS__)
#define st_warn(fmt, ...)  __ST_PRINT_HELPER(ST_PRINT_WARN, fmt, ##__VA_ARGS__)
#define st_error(fmt, ...) __ST_PRINT_HELPER(ST_PRINT_ERROR, fmt, ##__VA_ARGS__)

__attribute__((format(printf, 5, 6)))
void st_print(const int level, const char *const file, const char *const func,
              const int line, const char *const fmt, ...);

#endif // ST_UTILITY_PRINT_H
