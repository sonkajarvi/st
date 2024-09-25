#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/escape_codes.h>
#include <st/print.h>

void __do_print(FILE *fp, const char *pre, const char *func, const int line, const char *fmt, ...)
{
    // time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    fprintf(fp, "[" ST_ESC_BOLD ST_ESC_GREEN_B "%02d:%02d:%02d.%03ld" ST_ESC_RESET "]",
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000)) % 1000);

    // file and line
    fprintf(fp, "[" ST_ESC_BOLD ST_ESC_BLUE_B "%s:%d" ST_ESC_RESET "] ", func, line);

    // level
    fprintf(fp, "%s", pre);

    // message
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

#ifdef ST_HAS_ESCAPE_CODES
    fprintf(fp, ST_ESC_RESET);
#endif
}
