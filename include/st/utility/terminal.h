#ifndef ST_ESCAPE_CODES_H
#define ST_ESCAPE_CODES_H

#ifdef ST_CONFIG_COLORFUL_OUTPUT

#define ST_ESC_BOLD             "\033[1m"
#define ST_ESC_ITALIC           "\033[3m"
#define ST_ESC_UNDERLINE        "\033[4m"
#define ST_ESC_REVERSE          "\033[7m"
#define ST_ESC_STRIKE           "\033[9m"

#define ST_ESC_RESET            "\033[0m"
#define ST_ESC_RESET_BOLD       "\033[22m"
#define ST_ESC_RESET_ITALIC     "\033[23m"
#define ST_ESC_RESET_UNDERLINE  "\033[24m"
#define ST_ESC_RESET_REVERSE    "\033[27m"
#define ST_ESC_RESET_STRIKE     "\033[29m"

#define ST_ESC_BLACK            "\033[30m"
#define ST_ESC_RED              "\033[31m"
#define ST_ESC_GREEN            "\033[32m"
#define ST_ESC_YELLOW           "\033[33m"
#define ST_ESC_BLUE             "\033[34m"
#define ST_ESC_MAGENTA          "\033[35m"
#define ST_ESC_CYAN             "\033[36m"
#define ST_ESC_WHITE            "\033[37m"

#define ST_ESC_BG_BLACK         "\033[40m"
#define ST_ESC_BG_RED           "\033[41m"
#define ST_ESC_BG_GREEN         "\033[42m"
#define ST_ESC_BG_YELLOW        "\033[43m"
#define ST_ESC_BG_BLUE          "\033[44m"
#define ST_ESC_BG_MAGENTA       "\033[45m"
#define ST_ESC_BG_CYAN          "\033[46m"
#define ST_ESC_BG_WHITE         "\033[47m"

#define ST_ESC_BLACK_B          "\033[90m"
#define ST_ESC_RED_B            "\033[91m"
#define ST_ESC_GREEN_B          "\033[92m"
#define ST_ESC_YELLOW_B         "\033[93m"
#define ST_ESC_BLUE_B           "\033[94m"
#define ST_ESC_MAGENTA_B        "\033[95m"
#define ST_ESC_CYAN_B           "\033[96m"
#define ST_ESC_WHITE_B          "\033[97m"

#define ST_ESC_BG_BLACK_B       "\033[100m"
#define ST_ESC_BG_RED_B         "\033[101m"
#define ST_ESC_BG_GREEN_B       "\033[102m"
#define ST_ESC_BG_YELLOW_B      "\033[103m"
#define ST_ESC_BG_BLUE_B        "\033[104m"
#define ST_ESC_BG_MAGENTA_B     "\033[105m"
#define ST_ESC_BG_CYAN_B        "\033[106m"
#define ST_ESC_BG_WHITE_B       "\033[107m"

#else

#define ST_ESC_BOLD             ""
#define ST_ESC_ITALIC           ""
#define ST_ESC_UNDERLINE        ""
#define ST_ESC_REVERSE          ""
#define ST_ESC_STRIKE           ""

#define ST_ESC_RESET            ""
#define ST_ESC_RESET_BOLD       ""
#define ST_ESC_RESET_ITALIC     ""
#define ST_ESC_RESET_UNDERLINE  ""
#define ST_ESC_RESET_REVERSE    ""
#define ST_ESC_RESET_STRIKE     ""

#define ST_ESC_BLACK            ""
#define ST_ESC_RED              ""
#define ST_ESC_GREEN            ""
#define ST_ESC_YELLOW           ""
#define ST_ESC_BLUE             ""
#define ST_ESC_MAGENTA          ""
#define ST_ESC_CYAN             ""
#define ST_ESC_WHITE            ""

#define ST_ESC_BG_BLACK         ""
#define ST_ESC_BG_RED           ""
#define ST_ESC_BG_GREEN         ""
#define ST_ESC_BG_YELLOW        ""
#define ST_ESC_BG_BLUE          ""
#define ST_ESC_BG_MAGENTA       ""
#define ST_ESC_BG_CYAN          ""
#define ST_ESC_BG_WHITE         ""

#define ST_ESC_BLACK_B          ""
#define ST_ESC_RED_B            ""
#define ST_ESC_GREEN_B          ""
#define ST_ESC_YELLOW_B         ""
#define ST_ESC_BLUE_B           ""
#define ST_ESC_MAGENTA_B        ""
#define ST_ESC_CYAN_B           ""
#define ST_ESC_WHITE_B          ""

#define ST_ESC_BG_BLACK_B       ""
#define ST_ESC_BG_RED_B         ""
#define ST_ESC_BG_GREEN_B       ""
#define ST_ESC_BG_YELLOW_B      ""
#define ST_ESC_BG_BLUE_B        ""
#define ST_ESC_BG_MAGENTA_B     ""
#define ST_ESC_BG_CYAN_B        ""
#define ST_ESC_BG_WHITE_B       ""

#endif

#endif // ST_ESCAPE_CODES_H
