#include <st/parse.h>

char consume(StParser *parser, FILE *fp)
{
    char c = getc(fp);

    if (parser) {
        parser->col++;

        if (c == '\n') {
            parser->ln++;
            parser->col = 0;
        }
    }

    return c;
}

char peek(FILE *fp)
{
    return ungetc(getc(fp), fp);
}
