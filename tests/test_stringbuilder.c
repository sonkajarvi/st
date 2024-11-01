#include <stdlib.h>
#include <string.h>

#include <st/utility/stringbuilder.h>
#include "test.h"

test_case(st_strbuilder_free)
{
    struct st_strbuilder sb = { 0 };

    st_strbuilder_append(&sb, "hello");
    test_assert(sb.head != NULL);

    st_strbuilder_free(&sb);
    test_assert(sb.head == NULL);

    test_success();
}

test_case(st_strbuilder_append)
{
    struct st_strbuilder sb = { 0 };

    st_strbuilder_append(&sb, "hello");
    test_assert(sb.head != NULL);
    test_assert(st_strbuilder_length(&sb) == 5);

    st_strbuilder_append(&sb, ", world");
    test_assert(sb.head != NULL);
    test_assert(st_strbuilder_length(&sb) == 12);

    char *s = st_strbuilder_concat(&sb);
    test_assert(strcmp(s, "hello, world") == 0);

    free(s);
    st_strbuilder_free(&sb);
    test_assert(sb.head == NULL);

    test_success();
}

test_case(st_strbuilder_length)
{
    struct st_strbuilder sb = { 0 };
    test_assert(st_strbuilder_length(&sb) == 0);

    st_strbuilder_append(&sb, "abc");
    test_assert(st_strbuilder_length(&sb) == 3);

    st_strbuilder_append(&sb, "def");
    test_assert(st_strbuilder_length(&sb) == 6);

    st_strbuilder_append(&sb, "ghi");
    test_assert(st_strbuilder_length(&sb) == 9);

    st_strbuilder_free(&sb);
    test_assert(st_strbuilder_length(&sb) == 0);

    test_success();
}

test_case(st_strbuilder_empty)
{
    struct st_strbuilder sb = { 0 };

    // Null string
    st_strbuilder_append(&sb, NULL);
    test_assert(sb.head == NULL);
    test_assert(st_strbuilder_length(&sb) == 0);

    // Empty string
    st_strbuilder_append(&sb, "");
    test_assert(sb.head == NULL);
    test_assert(st_strbuilder_length(&sb) == 0);

    char *s = st_strbuilder_concat(&sb);
    test_assert(s == NULL);

    // st_strbuilder_free(&sb);
    // free(s);

    test_success();
}
