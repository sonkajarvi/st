#include <stdlib.h>
#include <string.h>

#include <st/util/stringbuilder.h>
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
    test_assert(sb.length == 5);

    st_strbuilder_append(&sb, ", world");
    test_assert(sb.head != NULL);
    test_assert(sb.length == 12);

    char *s = st_strbuilder_concat(&sb);
    test_assert(strcmp(s, "hello, world") == 0);

    free(s);
    st_strbuilder_free(&sb);
    test_assert(sb.head == NULL);

    test_success();
}

test_case(st_strbuilder_insert)
{
    struct st_strbuilder sb = { 0 };

    // At index 0
    st_strbuilder_insert(&sb, 0, "246");
    test_assert(sb.length == 3);

    // Again, at index 0
    st_strbuilder_insert(&sb, 0, "1");
    test_assert(sb.length == 4);

    // Out of bounds
    st_strbuilder_insert(&sb, 100, "8");
    test_assert(sb.length == 5);

    // Between strings
    st_strbuilder_insert(&sb, 4, "7");
    test_assert(sb.length == 6);

    // Inside string
    st_strbuilder_insert(&sb, 3, "3");
    test_assert(sb.length == 7);

    // Again, inside string
    st_strbuilder_insert(&sb, 4, "5");
    test_assert(sb.length == 8);

    char *s = st_strbuilder_concat(&sb);
    test_assert(strcmp(s, "12345678") == 0);

    free(s);
    st_strbuilder_free(&sb);

    test_success();
}

test_case(st_strbuilder_empty)
{
    struct st_strbuilder sb = { 0 };

    // Null string
    st_strbuilder_append(&sb, NULL);
    test_assert(sb.head == NULL);
    test_assert(sb.length == 0);

    // Empty string
    st_strbuilder_append(&sb, "");
    test_assert(sb.head == NULL);
    test_assert(sb.length == 0);

    char *s = st_strbuilder_concat(&sb);
    test_assert(s == NULL);

    test_success();
}
