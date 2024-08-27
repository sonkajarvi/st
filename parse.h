#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#define parser_printf(parser, fmt, ...) printf("(%lld:%lld) "fmt, (parser).ln, (parser).col, ##__VA_ARGS__)

typedef struct StParser
{
    size_t ln, col;
} StParser;

char consume(StParser *parser, FILE *fp);
char peek(FILE *fp);

#endif // PARSE_H
