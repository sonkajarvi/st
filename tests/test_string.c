#include <stdio.h>
#include <stddef.h> // NULL
#include <string.h> // strcmp

#include <st/utility/string.h>

#include "test.h"

test_case(st_string_empty)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_empty(s) == true);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_empty(s) == true);

    // Non-empty string
    st_string_set_char(&s, 0, 'a');
    test_assert(st_string_empty(s) == false);

    st_string_free(&s);

    test_success();
}

test_case(st_string_length)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_length(s) == 0);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_length(s) == 0);

    // Non-empty string
    st_string_set_char(&s, 0, 'a');
    test_assert(st_string_length(s) == 1);

    st_string_free(&s);

    test_success();
}

test_case(st_string_capacity)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_capacity(s) == 0);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_capacity(s) == 1);

    // Non-empty string
    st_string_set_char(&s, 0, 'a');
    test_assert(st_string_capacity(s) == 1);

    st_string_free(&s);

    test_success();
}

test_case(st_string_first)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_first(s) == 0);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_first(s) == 0);

    // Non-empty string
    st_string_append(&s, "hello");
    test_assert(st_string_first(s) == 'h');

    st_string_free(&s);

    test_success();
}

test_case(st_string_last)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_last(s) == 0);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_last(s) == 0);

    // Non-empty string
    st_string_append(&s, "hello");
    test_assert(st_string_last(s) == 'o');

    st_string_free(&s);

    test_success();
}

test_case(st_string_at)
{
    st_string s = NULL;

    // Null string
    test_assert(st_string_at(s, 0) == 0);

    // Empty string
    st_string_reserve(&s, 1);
    test_assert(st_string_at(s, 0) == 0);

    // Non-empty string
    st_string_append(&s, "hello");
    test_assert(st_string_at(s, 0) == 'h');
    test_assert(st_string_at(s, 1) == 'e');
    test_assert(st_string_at(s, 2) == 'l');
    test_assert(st_string_at(s, 3) == 'l');
    test_assert(st_string_at(s, 4) == 'o');
    test_assert(st_string_at(s, 5) == 0);

    st_string_free(&s);

    test_success();
}

test_case(st_string_set)
{
    st_string s = NULL;

    // Last index
    st_string_set_char(&s, 0, 'a');
    test_assert(st_string_length(s) == 1);
    test_assert(st_string_at(s, 0) == 'a');

    // In string
    st_string_append(&s, "ello");
    st_string_set_char(&s, 0, 'h');
    test_assert(st_string_length(s) == 5);
    test_assert(strcmp(s, "hello") == 0);

    // Out of bounds
    const size_t cap = st_string_capacity(s);
    st_string_set_char(&s, 100, 'a');
    test_assert(st_string_length(s) == 5);
    test_assert(st_string_capacity(s) == cap);
    test_assert(strcmp(s, "hello") == 0);

    // In string, null character
    st_string_set_char(&s, 2, '\0');
    test_assert(st_string_length(s) == 2);
    test_assert(strcmp(s, "he") == 0);

    st_string_free(&s);

    test_success();
}

test_case(st_string_append)
{
    st_string s = NULL;

    // Append null
    st_string_append(&s, NULL);
    test_assert(s == NULL);

    // Append empty
    st_string_append(&s, "");
    test_assert(s == NULL);

    st_string_append(&s, "hello");
    test_assert(st_string_length(s) == 5);
    test_assert(strcmp(s, "hello") == 0);

    st_string_append(&s, ", world");
    test_assert(st_string_length(s) == 12);
    test_assert(strcmp(s, "hello, world") == 0);

    // Append self
    st_string_append(&s, s);
    test_assert(st_string_length(s) == 24);
    test_assert(strcmp(s, "hello, worldhello, world") == 0);

    st_string_free(&s);

    test_success();
}

test_case(st_string_free)
{
    st_string s = NULL;

    st_string_append(&s, "hello");
    test_assert(s != NULL);
    st_string_free(&s);
    test_assert(s == NULL);

    test_success();
}