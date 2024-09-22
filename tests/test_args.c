#include <stdio.h>
#include <string.h>

#include <st/args.h>
#include <st/vector.h>

#include "test.h"

#define __create_args(...) \
    const char *__argv[] = { __VA_ARGS__ }; \
    const int argc = sizeof(__argv) / sizeof(*__argv); \
    char **argv = malloc(argc * sizeof(*argv)); \
    for (int i = 0; i < argc; i++) { \
        int len = strlen(__argv[i]) + 1; \
        argv[i] = malloc(len); \
        strcpy(argv[i], __argv[i]); \
    }

#define __free_args() \
    for (int i = 0; i < argc; i++) \
        free(argv[i]); \
    free(argv)

test_case(args_short_option)
{
    __create_args("", "-a", "-b");

    bool a, b, c;

    StArgsParser parser = {0};
    args_add_option(&parser, 'a', NULL, &a);
    args_add_option(&parser, 'b', NULL, &b);
    args_add_option(&parser, 'c', NULL, &c);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(a);
    test_assert(b);
    test_assert(!c);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_combined)
{
    __create_args("", "-ab");

    bool a, b;

    StArgsParser parser = {0};
    args_add_option(&parser, 'a', NULL, &a);
    args_add_option(&parser, 'b', NULL, &b);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(a);
    test_assert(b);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args)
{
    __create_args("", "-ahello", "-b", "world");

    char **a, **b;
    int al, bl;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);
    args_add_option_args(&parser, 'b', NULL, &b, &bl);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(al == 1);
    test_assert(strcmp(b[0], "world") == 0);
    test_assert(bl == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_combined)
{
    __create_args("", "-abhello", "-cd", "world");

    bool a, c;
    char **b, **d;
    int bl, dl;

    StArgsParser parser = {0};
    args_add_option(&parser, 'a', NULL, &a);
    args_add_option_args(&parser, 'b', NULL, &b, &bl);
    args_add_option(&parser, 'c', NULL, &c);
    args_add_option_args(&parser, 'd', NULL, &d, &dl);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(a);
    test_assert(strcmp(b[0], "hello") == 0);
    test_assert(bl == 1);
    test_assert(c);
    test_assert(strcmp(d[0], "world") == 0);
    test_assert(dl == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_long_option)
{
    __create_args("", "--apple", "--banana");

    bool a, b, c;

    StArgsParser parser = {0};
    args_add_option(&parser, 0, "apple", &a);
    args_add_option(&parser, 0, "banana", &b);
    args_add_option(&parser, 0, "coconut", &c);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(a);
    test_assert(b);
    test_assert(!c);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_long_option_args)
{
    __create_args("", "--apple=hello", "--banana", "world");

    char **a, **b, **c;
    int al, bl, cl;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 0, "apple", &a, &al);
    args_add_option_args(&parser, 0, "banana", &b, &bl);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(al == 1);
    test_assert(strcmp(b[0], "world") == 0);
    test_assert(bl == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_fixed)
{
    __create_args("", "hello", "world");

    char **x;
    int xl;

    StArgsParser parser = {0};
    args_add_fixed(&parser, &x, &xl);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(x[0], "hello") == 0);
    test_assert(strcmp(x[1], "world") == 0);
    test_assert(xl == 2);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_array)
{
    __create_args("", "-ahello,world");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(strcmp(a[1], "world") == 0);
    test_assert(al == 2);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_array_combined)
{
    __create_args("", "-ab", "hello,world");

    bool a;
    char **b;
    int bl;

    StArgsParser parser = {0};
    args_add_option(&parser, 'a', NULL, &a);
    args_add_option_args(&parser, 'b', NULL, &b, &bl);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(a);
    test_assert(strcmp(b[0], "hello") == 0);
    test_assert(strcmp(b[1], "world") == 0);
    test_assert(bl == 2);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_array_next)
{
    __create_args("", "-a", "hello,", "world");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(strcmp(a[1], "world") == 0);
    test_assert(al == 2);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_array_trailing)
{
    __create_args("", "-a", "hello,");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(al == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_long_option_args_array)
{
    __create_args("", "--apple=hello,world");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 0, "apple", &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(strcmp(a[1], "world") == 0);
    test_assert(al == 2);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_long_option_args_array_next)
{
    __create_args("", "--apple", "hello,", "world");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 0, "apple", &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello") == 0);
    test_assert(strcmp(a[1], "world") == 0);
    test_assert(al == 2);

    args_free(&parser);

    __free_args();
    test_success();
}


// other cases

test_case(args_array_escaped_separator)
{
    __create_args("", "-a", "hello\\,world");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello,world") == 0);
    test_assert(al == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_array_escaped_separators)
{
    __create_args("", "-a", "h\\,e\\,l\\,l\\,o\\,w\\,o\\,r\\,l\\,d");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "h,e,l,l,o,w,o,r,l,d") == 0);
    test_assert(al == 1);

    args_free(&parser);

    __free_args();
    test_success();
}


test_case(args_array_escaped_separator_end)
{
    __create_args("", "-a", "hello\\,");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);

    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);
    test_assert(strcmp(a[0], "hello,") == 0);
    test_assert(al == 1);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_empty)
{
    __create_args("");

    StArgsParser parser = {0};
    test_assert(args_parse(&parser, argc, argv) == ARGS_OK);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_no_id)
{
    __create_args("", "-");

    StArgsParser parser = {0};
    test_assert(args_parse(&parser, argc, argv) != ARGS_OK);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_long_option_no_id)
{
    __create_args("", "--");

    StArgsParser parser = {0};
    test_assert(args_parse(&parser, argc, argv) != ARGS_OK);

    args_free(&parser);

    __free_args();
    test_success();
}

test_case(args_short_option_args_no_args)
{
    __create_args("", "-a");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 'a', NULL, &a, &al);
    test_assert(args_parse(&parser, argc, argv) != ARGS_OK);
    test_assert(a == NULL);
    test_assert(al == 0);

    __free_args();
    test_success();
}

test_case(args_long_option_args_no_args)
{
    __create_args("", "--apple=");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 0, "apple", &a, &al);
    test_assert(args_parse(&parser, argc, argv) != ARGS_OK);
    test_assert(a == NULL);
    test_assert(al == 0);

    __free_args();
    test_success();
}

test_case(args_long_option_args_no_args2)
{
    __create_args("", "--apple");

    char **a;
    int al;

    StArgsParser parser = {0};
    args_add_option_args(&parser, 0, "apple", &a, &al);
    test_assert(args_parse(&parser, argc, argv) != ARGS_OK);
    test_assert(a == NULL);
    test_assert(al == 0);

    __free_args();
    test_success();
}
