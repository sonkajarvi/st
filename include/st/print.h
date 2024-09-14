#ifndef ST_PRINT_H
#define ST_PRINT_H

#include <stdio.h>

#include <st/escape_codes.h>

#define st_debug(fmt, ...)  __do_print(stdout, ST_ESC_BLACK_B"DEBUG: ", __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_log(fmt, ...)    __do_print(stdout, ST_ESC_RESET"LOG: ", __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_warn(fmt, ...)   __do_print(stderr, ST_ESC_YELLOW_B"WARN: ", __func__, __LINE__, fmt, ##__VA_ARGS__)
#define st_error(fmt, ...)  __do_print(stderr, ST_ESC_RED"ERROR: ", __func__, __LINE__, fmt, ##__VA_ARGS__)

void __do_print(FILE *fp, const char *pre, const char *func, const int line, const char *fmt, ...);

#endif // ST_PRINT_H
