#ifndef ST_UTILITY_PRINT_H
#define ST_UTILITY_PRINT_H

#include <stdio.h>

#define _ST_PRINT_SOH      "\01"
#define _ST_PRINT_SOH_CHAR '\01'

#define ST_PRINT_DEBUG     _ST_PRINT_SOH "\01"
#define ST_PRINT_LOG       _ST_PRINT_SOH "\02"
#define ST_PRINT_WARN      _ST_PRINT_SOH "\03"
#define ST_PRINT_ERROR     _ST_PRINT_SOH "\04"
#define ST_PRINT_ASSERT    _ST_PRINT_SOH "\05"

#define st_debug(fmt, ...) st_fprint(stdout, ST_PRINT_DEBUG fmt, ##__VA_ARGS__)
#define st_log(fmt, ...)   st_fprint(stdout, ST_PRINT_LOG fmt, ##__VA_ARGS__)
#define st_warn(fmt, ...)  st_fprint(stdout, ST_PRINT_WARN fmt, ##__VA_ARGS__)
#define st_error(fmt, ...) st_fprint(stdout, ST_PRINT_ERROR fmt, ##__VA_ARGS__)

#define st_print(fmt, ...) st_fprint(stdout, fmt,  ##__VA_ARGS__)
#define st_fprint(fp, fmt, ...) _st_fprint(fp, __func__, __LINE__, fmt, ##__VA_ARGS__)

void _st_fprint(FILE *const fp, const char *func, const int line, const char *fmt, ...);

// note: Also used by st_assert
void _st_print_time(FILE *fp);

#endif // ST_UTILITY_PRINT_H
