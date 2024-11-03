#include <st/utility/compiler.h>
#include <st/utility/vector.h>

#include "test.h"

test_case(st_vector_empty)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_empty(v) == 1);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_empty(v) == 0);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_length)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_length(v) == 0);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_length(v) == 3);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_capacity)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_capacity(v) == 0);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_capacity(v) == 3);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_at)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_at(v, 0) == NULL);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(*st_vector_at(v, 0) == 1);
    test_assert(*st_vector_at(v, 1) == 2);
    test_assert(*st_vector_at(v, 2) == 3);
    test_assert(st_vector_at(v, 3) == NULL);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_begin)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_begin(v) == NULL);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_begin(v) == &v[0]);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_end)
{
    int *v = NULL;

    // Null vector
    test_assert(st_vector_end(v) == NULL);

    // Vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_end(v) == &v[2]);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_for)
{
    int *v = NULL, *w = NULL;

    // Iterate null vector
    st_vector_for(v, i) {
        test_assert(0);
    }

    // Iterate vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);

    int count = 0;
    st_vector_for(v, i)
        test_assert(v[count++] == *i);
    test_assert(count == 3);

    st_vector_free(v);

    test_success();
}

test_case(st_vector_rof)
{
    int *v = NULL;

    // Iterate null vector
    st_vector_rof(v, i) {
        test_assert(0);
    }

    // Iterate vector with elements
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);

    int count = 3;
    st_vector_rof(v, i)
        test_assert(v[(count--) - 1] == *i);
    test_assert(count == 0);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_push)
{
    int *v = NULL, *a;

    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    st_vector_push(v, 4);
    a = st_vector_push(v, 5);
    test_assert(a == &v[4]);

    test_assert(st_vector_length(v) == 5);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    test_assert(v[4] == 5);

    st_vector_free(v);

    test_success();
}

test_case(st_vector_push_range)
{
    int *v = NULL;
    static int a[] = { 1, 2, 3, 4, 5 };

    st_vector_push_range(v, NULL, 5);
    test_assert(v == NULL);

    st_vector_push_range(v, a, 0);
    test_assert(v == NULL);

    st_vector_push_range(v, a, 0);
    test_assert(v == NULL);

    st_vector_push_range(v, a, 5);
    test_assert(st_vector_length(v) == 5);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    test_assert(v[4] == 5);

    st_vector_free(v);

    test_success();
}

test_case(st_vector_insert)
{
    int *v = NULL, *t;

    // Insert in null vector
    t = st_vector_insert(v, 0, 1);
    test_assert(t == &v[0]);
    test_assert(st_vector_length(v) == 1);

    // Insert out of bounds
    t = st_vector_insert(v, 100, 3);
    test_assert(t == &v[1]);
    test_assert(st_vector_length(v) == 2);

    // Insert between elements
    t = st_vector_insert(v, 1, 2);
    test_assert(t == &v[1]);
    test_assert(st_vector_length(v) == 3);

    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_insert_swap)
{
    int *v = NULL, *t;

    // Insert in null vector
    t = st_vector_insert_swap(v, 0, 4);
    test_assert(t == &v[0]);
    test_assert(st_vector_length(v) == 1);

    // Insert out of bounds
    t = st_vector_insert_swap(v, 100, 2);
    test_assert(t == &v[1]);
    test_assert(st_vector_length(v) == 2);

    // Insert between elements
    t = st_vector_insert_swap(v, 0, 1);
    test_assert(t == &v[0]);
    test_assert(st_vector_length(v) == 3);

    // Insert at the end
    t = st_vector_insert_swap(v, 2, 3);
    test_assert(t == &v[2]);
    test_assert(st_vector_length(v) == 4);

    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_pop)
{
    int *v = NULL;

    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_length(v) == 3);

    st_vector_pop(v);
    test_assert(st_vector_length(v) == 2);

    st_vector_pop(v);
    test_assert(st_vector_length(v) == 1);

    st_vector_pop(v);
    test_assert(st_vector_length(v) == 0);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_remove)
{
    int *v = NULL;

    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);

    st_vector_remove(v, 0);
    test_assert(st_vector_length(v) == 2);
    test_assert(v[0] == 2);
    test_assert(v[1] == 3);

    // Remove last element
    st_vector_remove(v, 1);
    test_assert(st_vector_length(v) == 1);
    test_assert(v[0] == 2);

    // Remove out of bounds
    st_vector_remove(v, 100);
    test_assert(st_vector_length(v) == 1);
    test_assert(v[0] == 2);

    st_vector_free(v);

    test_success();
}

test_case(st_vector_remove_swap)
{
    int *v = NULL;

    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);

    st_vector_remove_swap(v, 0);
    test_assert(st_vector_length(v) == 2);
    test_assert(v[0] == 3);
    test_assert(v[1] == 2);

    // Remove last element
    st_vector_remove_swap(v, 1);
    test_assert(st_vector_length(v) == 1);
    test_assert(v[0] == 3);

    // Remove out of bounds
    st_vector_remove_swap(v, 100);
    test_assert(st_vector_length(v) == 1);
    test_assert(v[0] == 3);

    st_vector_free(v);

    test_success();
}

test_case(st_vector_reserve)
{
    int *v = NULL;

    // Reserve 0
    st_vector_reserve(v, 0);
    test_assert(st_vector_length(v) == 0);
    test_assert(st_vector_capacity(v) == 0);
    test_assert(v == NULL);

    // Reserve 1
    st_vector_reserve(v, 1);
    test_assert(st_vector_length(v) == 0);
    test_assert(st_vector_capacity(v) == 1);

    // Reserve more
    st_vector_reserve(v, 100);
    test_assert(st_vector_length(v) == 0);
    test_assert(st_vector_capacity(v) == 100);

    // Reserve less than current capacity
    st_vector_reserve(v, 50);
    test_assert(st_vector_length(v) == 0);
    test_assert(st_vector_capacity(v) == 100);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_resize)
{
    int *v = NULL;

    // Resize null vector to 0
    st_vector_resize(v, 0);

    // Resize null vector
    st_vector_resize(v, 10);
    test_assert(st_vector_capacity(v) == 10);

    // Resize vector with capacity
    st_vector_resize(v, 5);
    test_assert(st_vector_capacity(v) == 5);

    // Resize to 0
    st_vector_resize(v, 0);
    test_assert(v == NULL);

    test_success();
}

test_case(st_vector_shrink)
{
    int *v = NULL;

    // Resize vector with elements
    st_vector_reserve(v, 10);
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    st_vector_shrink(v);
    test_assert(st_vector_capacity(v) == 3);

    // Resize vector with equal length and capacity
    st_vector_shrink(v);
    test_assert(st_vector_capacity(v) == 3);

    // Resize vector with no elements
    st_vector_clear(v);
    st_vector_shrink(v);
    test_assert(v == NULL);

    test_success();
}

test_case(st_vector_clear)
{
    int *v = NULL;

    st_vector_reserve(v, 10);
    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(st_vector_length(v) == 3);
    test_assert(st_vector_capacity(v) == 10);
    
    st_vector_clear(v);
    test_assert(st_vector_length(v) == 0);
    test_assert(st_vector_capacity(v) == 10);
    st_vector_free(v);

    test_success();
}

test_case(st_vector_free)
{
    int *v = NULL;

    st_vector_push(v, 1);
    st_vector_push(v, 2);
    st_vector_push(v, 3);
    test_assert(v != NULL);

    st_vector_free(v);
    test_assert(v == NULL);

    test_success();
}
