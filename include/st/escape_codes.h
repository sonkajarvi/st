#ifndef ST_ESCAPE_CODES_H
#define ST_ESCAPE_CODES_H

#define __ESC "\033"

#define ST_ESC_BOLD             __ESC "[1m"
#define ST_ESC_ITALIC           __ESC "[3m"
#define ST_ESC_UNDERLINE        __ESC "[4m"
#define ST_ESC_REVERSE          __ESC "[7m"
#define ST_ESC_STRIKE           __ESC "[9m"

#define ST_ESC_RESET            __ESC "[0m"
#define ST_ESC_RESET_BOLD       __ESC "[22m"
#define ST_ESC_RESET_ITALIC     __ESC "[23m"
#define ST_ESC_RESET_UNDERLINE  __ESC "[24m"
#define ST_ESC_RESET_REVERSE    __ESC "[27m"
#define ST_ESC_RESET_STRIKE     __ESC "[29m"

#define ST_ESC_BLACK            __ESC "[30m"
#define ST_ESC_RED              __ESC "[31m"
#define ST_ESC_GREEN            __ESC "[32m"
#define ST_ESC_YELLOW           __ESC "[33m"
#define ST_ESC_BLUE             __ESC "[34m"
#define ST_ESC_MAGENTA          __ESC "[35m"
#define ST_ESC_CYAN             __ESC "[36m"
#define ST_ESC_WHITE            __ESC "[37m"

#define ST_ESC_BG_BLACK         __ESC "[40m"
#define ST_ESC_BG_RED           __ESC "[41m"
#define ST_ESC_BG_GREEN         __ESC "[42m"
#define ST_ESC_BG_YELLOW        __ESC "[43m"
#define ST_ESC_BG_BLUE          __ESC "[44m"
#define ST_ESC_BG_MAGENTA       __ESC "[45m"
#define ST_ESC_BG_CYAN          __ESC "[46m"
#define ST_ESC_BG_WHITE         __ESC "[47m"

#define ST_ESC_BLACK_B          __ESC "[90m"
#define ST_ESC_RED_B            __ESC "[91m"
#define ST_ESC_GREEN_B          __ESC "[92m"
#define ST_ESC_YELLOW_B         __ESC "[93m"
#define ST_ESC_BLUE_B           __ESC "[94m"
#define ST_ESC_MAGENTA_B        __ESC "[95m"
#define ST_ESC_CYAN_B           __ESC "[96m"
#define ST_ESC_WHITE_B          __ESC "[97m"

#define ST_ESC_BG_BLACK_B       __ESC "[100m"
#define ST_ESC_BG_RED_B         __ESC "[101m"
#define ST_ESC_BG_GREEN_B       __ESC "[102m"
#define ST_ESC_BG_YELLOW_B      __ESC "[103m"
#define ST_ESC_BG_BLUE_B        __ESC "[104m"
#define ST_ESC_BG_MAGENTA_B     __ESC "[105m"
#define ST_ESC_BG_CYAN_B        __ESC "[106m"
#define ST_ESC_BG_WHITE_B       __ESC "[107m"

#endif // ST_ESCAPE_CODES_H
