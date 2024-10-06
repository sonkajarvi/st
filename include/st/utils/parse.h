#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

typedef struct StParser
{
    size_t ln, col;
} StParser;

char consume(StParser *parser, FILE *fp);
char peek(FILE *fp);

#endif // PARSE_H
