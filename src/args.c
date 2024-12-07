#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <st/util/assert.h>
#include <st/util/args.h>
#include <st/util/vector.h>

static struct {
    int argc;
    char **argv;
    int arg_index;
    int str_index;
} p = {0};

static char peek_ch(void) {
    return p.argv[p.arg_index][p.str_index];
}

static char consume_ch(void) {
    return p.argv[p.arg_index][p.str_index++];
}

// arg_index and str_index set by caller
static int parse_option_args(StArgsOption *opt)
{
    char c;
    for (;;) {
        bool should_continue = false;

        if (peek_ch() == '\0') {
            if (p.arg_index == p.argc - 1)
                return ARGS_OK;

            p.arg_index++;
            p.str_index = 0;
        }

        int start = p.str_index;
        for (;;) {
            c = consume_ch();

            if (c == '\0')
                break;
            if (c == ',') {
                p.argv[p.arg_index][p.str_index - 1] = '\0';
                should_continue = true;
                break;
            }
            if (c == '\\' && peek_ch() == ',') {
                char *ps = &p.argv[p.arg_index][start];
                memmove(ps + 1, ps, p.str_index - start - 1);
                start++;
                consume_ch();
            }
        }

        if (start - p.str_index == 0)
            return p.arg_index;

        char *tmp = &p.argv[p.arg_index][start];
        vector_push(*opt->out.args.arr, tmp);
        (*opt->out.args.len)++;

        if (!should_continue)
            break;
    }

    return ARGS_OK;
}

static StArgsOption *find_short(StArgsParser *parser, char id)
{
    vector_for(parser->options, StArgsOption, opt) {
        if (opt->short_id == id)
            return opt;
    }

    return NULL;
}

static int parse_short(StArgsParser *parser)
{
    while (isalnum(peek_ch())) {
        char id = consume_ch();

        StArgsOption *opt = find_short(parser, id);
        if (!opt) {
            return p.arg_index;
        }

        if (!opt->takes_args) {
            *opt->out.boolean = true;
            continue;
        }

        if (peek_ch() == '\0') {
            if (p.arg_index == p.argc - 1)
                return p.arg_index;

            p.arg_index++;
            p.str_index = 0;
        }

        return parse_option_args(opt);
    }

    return ARGS_OK;
}

static StArgsOption *find_long(StArgsParser *parser, char *id)
{
    vector_for(parser->options, StArgsOption, opt) {
        if (strcmp(opt->long_id, id) == 0)
            return opt;
    }

    return NULL;
}

static int parse_long(StArgsParser *parser)
{
    int start = p.str_index;
    while (isalnum(peek_ch()))
        consume_ch();

    char tmp = p.argv[p.arg_index][p.str_index];

    p.argv[p.arg_index][p.str_index] = '\0';

    char *id = &p.argv[p.arg_index][start];
    StArgsOption *opt = find_long(parser, id);
    if (!opt)
        return p.arg_index;

    p.argv[p.arg_index][p.str_index] = tmp;

    if (!opt->takes_args) {
        *opt->out.boolean = true;
        return ARGS_OK;
    }

    if (peek_ch() == '\0') {
        if (p.arg_index == p.argc - 1)
            return p.arg_index;

        p.arg_index++;
        p.str_index = 0;
    }

    else if (peek_ch() == '=') {
        consume_ch();

        if (peek_ch() == '\0')
            return p.arg_index;
    }

    return parse_option_args(opt);
}

static int parse_fixed(StArgsParser *parser)
{
    if (peek_ch() == '\0')
        return p.arg_index;

    int start = p.str_index;
    while (peek_ch())
        consume_ch();

    char *tmp = &p.argv[p.arg_index][start];
    vector_push(*parser->fixed_out, tmp);
    (*parser->fixed_out_len)++;

    return ARGS_OK;
}

int args_parse(StArgsParser *parser, int argc, char **argv)
{
    st_assert(parser);
    st_assert(argc > 0);
    st_assert(argv);

    p.argc = argc;
    p.argv = argv;

    int retval = ARGS_OK;

    for (p.arg_index = 1; p.arg_index < p.argc; p.arg_index++) {
        p.str_index = 0;
        if (peek_ch()) {
            // Starts with a delimiter, short or long
            if (peek_ch() == '-') {
                consume_ch();

                // Another delimiter, long option
                if (peek_ch() == '-') {
                    consume_ch();
                    if ((retval = parse_long(parser)) != ARGS_OK)
                        return retval;
                    continue;
                }
                // Short option
                else if (isalnum(peek_ch())) {
                    if ((retval = parse_short(parser)) != ARGS_OK)
                        return retval;
                    continue;
                }
                else {
                    return p.arg_index;
                }
            }
            // No delimiter, fixed option
            else {
                if ((retval = parse_fixed(parser)) != ARGS_OK)
                    return retval;
                continue;
            }
        }
    }

    return retval;
}

void args_free(StArgsParser *parser)
{
    if (!parser)
        return;

    vector_for(parser->options, StArgsOption, opt)
        if (opt->takes_args && opt->out.args.arr)
            vector_free(*opt->out.args.arr);
    vector_free(parser->options);

    if (parser->fixed_out)
        vector_free(*parser->fixed_out);
}

void args_add_option(StArgsParser *parser, char short_id, char *long_id, bool *out)
{
    st_assert(parser);

    *out = false;

    StArgsOption tmp;
    tmp.short_id = short_id;
    tmp.long_id = long_id;
    tmp.takes_args = false;
    tmp.out.boolean = out;

    vector_push_copy(parser->options, tmp);
}

void args_add_option_args(StArgsParser *parser, char short_id, char *long_id,
    char ***out, int *out_len)
{
    st_assert(parser);

    *out = NULL;
    *out_len = 0;

    StArgsOption tmp;
    tmp.short_id = short_id;
    tmp.long_id = long_id;
    tmp.takes_args = true;
    tmp.out.args.arr = out;
    tmp.out.args.len = out_len;

    vector_push_copy(parser->options, tmp);
}

void args_add_fixed(StArgsParser *parser, char ***out, int *out_len)
{
    st_assert(parser);

    *out = NULL;
    *out_len = 0;

    parser->fixed_out = out;
    parser->fixed_out_len = out_len;
}
