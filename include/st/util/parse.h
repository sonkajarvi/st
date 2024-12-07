#ifndef ST_UTILITY_PARSE_H
#define ST_UTILITY_PARSE_H

#include <stdio.h>

typedef struct StParser
{
    size_t ln, col;
} StParser;

char consume(StParser *parser, FILE *fp);
char peek(FILE *fp);

#endif // ST_UTILITY_PARSE_H
