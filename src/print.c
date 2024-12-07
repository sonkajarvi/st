#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/util/assert.h>
#include <st/util/print.h>
#include <st/util/terminal.h>

static const char *const prefixes[] = {
    ST_ESC_BLACK_B "debug:",
    "log:",
    ST_ESC_YELLOW_B "*warn*:",
    ST_ESC_BOLD ST_ESC_RED_B "*ERROR*:",
    ST_ESC_BOLD ST_ESC_MAGENTA_B " **ASSERT FAILED**:"
};

void st_print(const int level, const char *const file, const char *const func,
              const int line, const char *const fmt, ...)
{
    FILE *fp = stdout;
    if (level >= ST_PRINT_WARN)
        fp = stderr;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(fp, ST_ESC_RESET ST_ESC_GREEN "[%02d:%02d:%02d]" ST_ESC_RESET,
        tm->tm_hour, tm->tm_min, tm->tm_sec);

    fprintf(fp, ST_ESC_YELLOW " %s @ %s:%d" ST_ESC_RESET, func, file, line);
    fprintf(fp, " %s ", prefixes[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "%s", ST_ESC_RESET);
}
