#ifndef ST_UTILITY_PRINT_H
#define ST_UTILITY_PRINT_H

#include <stdio.h>

#include <st/utility/terminal.h>

#define ST_LEVEL_DEBUG 0
#define ST_LEVEL_LOG 1
#define ST_LEVEL_WARN 2
#define ST_LEVEL_ERROR 3

#define st_debug(fmt, ...) _st_fprintf(stdout, ST_LEVEL_DEBUG, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_log(fmt, ...) _st_fprintf(stdout, ST_LEVEL_LOG, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_warn(fmt, ...) _st_fprintf(stdout, ST_LEVEL_WARN, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_error(fmt, ...) _st_fprintf(stdout, ST_LEVEL_ERROR, __func__, __LINE__, fmt, ##__VA_ARGS__)

void _st_fprintf(FILE *fp, const int level,
    const char *function, const int line, const char *fmt, ...);

#endif // ST_UTILITY_PRINT_H
