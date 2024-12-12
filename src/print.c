#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <st/util/assert.h>
#include <st/util/print.h>
#include <st/util/terminal.h>

static int last_level = -1;

static const char *const color_prefixes[] = {
    ST_ESC_BLACK_B,
    "",
    ST_ESC_YELLOW_B,
    ST_ESC_BOLD ST_ESC_RED_B,
    ST_ESC_BOLD ST_ESC_MAGENTA_B
};

static const char *const level_prefixes[] = {
    "debug:",
    "log:",
    "*warn*:",
    "*ERROR*:",
    " **ASSERT FAILED**:"
};

void st_print(int level, const char *const file, const char *const func,
              const int line, const char *const fmt, ...)
{
    va_list args;
    FILE *fp = stdout;

    if (level == ST_PRINT_CONT) {
        st_assert(last_level != -1 && "Nothing to continue printing from");

        if ((level = last_level) >= ST_PRINT_WARN)
            fp = stderr;
        goto cont;
    }

    if (level >= ST_PRINT_WARN)
        fp = stderr;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(fp, ST_ESC_RESET ST_ESC_GREEN "[%02d:%02d:%02d]" ST_ESC_YELLOW " %s @ %s:%d" ST_ESC_RESET " %s%s ",
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        func, file, line,
        color_prefixes[level], level_prefixes[level]);
    goto no_cont;

cont:
    fprintf(fp, "%s", color_prefixes[level]);

no_cont:
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "%s", ST_ESC_RESET);
    last_level = level;
}
