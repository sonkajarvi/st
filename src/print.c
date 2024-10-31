#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>
#include <st/utility/terminal.h>

static const char *const __prefixes[] = {
    ST_ESC_BLACK_B "debug: ",
    "log: ",
    ST_ESC_YELLOW_B "*warn*: ",
    ST_ESC_BOLD ST_ESC_RED_B "*ERROR*: ",
    ST_ESC_BOLD ST_ESC_MAGENTA_B " **ASSERT FAILED**: "
};

static inline int __get_print_level(const char *fmt)
{
    if (fmt[0] == _ST_PRINT_SOH_CHAR && fmt[1]) {
        if (fmt[1] >= 1 && fmt[1] <= 5)
            return fmt[1];
    }
    return 0;
}

void _st_print_time(FILE *fp)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    fprintf(fp, ST_ESC_RESET ST_ESC_GREEN "[%02d:%02d:%02d.%03ld]" ST_ESC_RESET,
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000)) % 1000);
}

// todo: Extend formatting options, e.g. v2, v3, v4 for vectors
void _st_fprint(FILE *const fp, const char *func, const int line, const char *fmt, ...)
{
    st_assert(fp);
    st_assert(fmt);

    const int level = __get_print_level(fmt);
    if (level) {
        fmt += 2; // Skip level

        _st_print_time(fp);
        if (level <= 4)
            fprintf(fp, ST_ESC_YELLOW " %s:%d: " ST_ESC_RESET, func, line);
        fprintf(fp, "%s", __prefixes[level - 1]);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "%s", ST_ESC_RESET);
}
