#ifndef ST_UTILS_ARGS_H
#define ST_UTILS_ARGS_H

#include <stdbool.h>

#define ARGS_OK 0

typedef struct
{
    char short_id;
    char *long_id;
    bool takes_args;
    union {
        bool *boolean;
        struct {
            char ***arr;
            int *len;
        } args;
    } out;
} StArgsOption;

typedef struct StArgsParser
{
    StArgsOption *options;
    char ***fixed_out;
    int *fixed_out_len;
} StArgsParser;

int args_parse(StArgsParser *parser, int argc, char **argv);
void args_free(StArgsParser *parser);

void args_add_option(StArgsParser *parser, char short_name,
    char *long_name, bool *out);
void args_add_option_args(StArgsParser *parser, char short_name,
    char *long_name, char ***out, int *out_len);
void args_add_fixed(StArgsParser *parser, char ***out, int *out_len);

#endif // ST_UTILS_ARGS_H
