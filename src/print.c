#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/utils/terminal.h>
#include <st/utils/print.h>

void _st_fprintf(FILE *fp, const int level,
    const char *function, const int line, const char *fmt, ...)
{
    // timestamp
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    fprintf(fp, ST_ESC_GREEN "[%02d:%02d:%02d.%03ld] " ST_ESC_RESET,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000)) % 1000);

    // function and line
    fprintf(fp, ST_ESC_YELLOW "%s:%d: " ST_ESC_RESET, function, line);

    // level
    switch (level) {
    case ST_LEVEL_DEBUG:
        fprintf(fp, ST_ESC_BLACK_B "debug: ");
        break;

    case ST_LEVEL_LOG:
        fprintf(fp, "log: ");
        break;

    case ST_LEVEL_WARN:
        fprintf(fp, ST_ESC_YELLOW_B "*warn*: ");
        break;

    case ST_LEVEL_ERROR:
        fprintf(fp, ST_ESC_BOLD ST_ESC_RED_B "*ERROR*: ");
        break;

    default:
        fprintf(fp, "(unknown): ");
        break;
    }

    // message
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "%s", ST_ESC_RESET);
}
