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
    st_strbuilder_insert(&sb, 0, "defjkl");
    test_assert(sb.length == 6);

    // Again, at index 0
    st_strbuilder_insert(&sb, 0, "abc");
    test_assert(sb.length == 9);

    // Out of bounds
    st_strbuilder_insert(&sb, 100, "pqr");
    test_assert(sb.length == 12);

    // Between strings
    st_strbuilder_insert(&sb, 9, "mno");
    test_assert(sb.length == 15);

    // Inside string
    st_strbuilder_insert(&sb, 6, "ghi");
    test_assert(sb.length == 18);

    char *s = st_strbuilder_concat(&sb);
    test_assert(strcmp(s, "abcdefghijklmnopqr") == 0);

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
