#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>
#include <st/utility/terminal.h>

static const char *const __PREFIXES[] = {
    ST_ESC_BLACK_B "debug:",
    "log:",
    ST_ESC_YELLOW_B "*warn*:",
    ST_ESC_BOLD ST_ESC_RED_B "*ERROR*:",
    ST_ESC_BOLD ST_ESC_MAGENTA_B " **ASSERT FAILED**:"
};

static inline void print_time(FILE *const fp)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    fprintf(fp, ST_ESC_RESET ST_ESC_GREEN "[%02d:%02d:%02d.%03ld]" ST_ESC_RESET,
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000)) % 1000);
}

void st_print(const int level, const char *const file, const char *const func,
              const int line, const char *const fmt, ...)
{
    FILE *fp = stdout;
    if (level >= ST_PRINT_WARN)
        fp = stderr;

    print_time(fp);
    fprintf(fp, ST_ESC_YELLOW " %s:%s:%d:" ST_ESC_RESET, file, func, line);

    fprintf(fp, " %s ", __PREFIXES[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "%s", ST_ESC_RESET);
}
