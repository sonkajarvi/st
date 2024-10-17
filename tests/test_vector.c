#include <st/utils/compiler.h>
#include <st/utils/vector.h>

#include "test.h"

test_case(vector_isempty)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_isempty(v) == 1);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_isempty(v) == 0);
    vector_free(v);

    test_success();
}

test_case(vector_length)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_length(v) == 0);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_length(v) == 3);
    vector_free(v);

    test_success();
}

test_case(vector_capacity)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_capacity(v) == 0);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_capacity(v) == 3);
    vector_free(v);

    test_success();
}

test_case(vector_front)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_front(v) == NULL);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_front(v) == &v[0]);
    vector_free(v);

    test_success();
}

test_case(vector_back)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_back(v) == NULL);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_back(v) == &v[2]);
    vector_free(v);

    test_success();
}

test_case(vector_at)
{
    int *v = NULL;

    // Null vector
    test_assert(vector_at(v, 0) == NULL);

    // Vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(*vector_at(v, 0) == 1);
    test_assert(*vector_at(v, 1) == 2);
    test_assert(*vector_at(v, 2) == 3);
    test_assert(vector_at(v, 3) == NULL);
    vector_free(v);

    test_success();
}

test_case(vector_for)
{
    int *v = NULL;

    // Iterate null vector
    vector_for(v, int, i) {
        test_assert(0);
    }

    // Iterate vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    int count = 0;
    vector_for(v, int, i)
        test_assert(v[count++] == *i);
    test_assert(count == 3);
    vector_free(v);

    test_success();
}

test_case(vector_rof)
{
    int *v = NULL;

    // Iterate null vector
    vector_for(v, int, i) {
        test_assert(0);
    }

    // Iterate vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    int count = 3;
    vector_rof(v, int, i)
        test_assert(v[(count--) - 1] == *i);
    test_assert(count == 0);
    vector_free(v);

    test_success();
}

test_case(vector_push)
{
    int *v = NULL, *t;

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    vector_push(v, 4);
    t = vector_push(v, 5);
    test_assert(t == &v[4]);

    test_assert(vector_length(v) == 5);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    test_assert(v[4] == 5);
    vector_free(v);

    test_success();
}

test_case(vector_push_1000000)
{
    int *v = NULL;

    for (int i = 0; i < 1000000; i++)
        vector_push(v, i);

    test_assert(vector_length(v) == 1000000);
    int count = 0;
    vector_for(v, int, i)
        test_assert(v[count++] == *i);
    vector_free(v);

    test_success();
}

test_case(vector_pop)
{
    int *v = NULL;

    // Pop null vector
    test_assert(vector_pop(v) == 0);

    // Pop vector without elements
    vector_reserve(v, 3);
    test_assert(vector_pop(v) == 0);

    // Pop vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    test_assert(vector_pop(v) == 1);
    test_assert(vector_length(v) == 2);

    test_assert(vector_pop(v) == 1);
    test_assert(vector_length(v) == 1);
    
    test_assert(vector_pop(v) == 1);
    test_assert(vector_length(v) == 0);
    vector_free(v);

    test_success();
}

test_case(vector_insert)
{
    int *v = NULL, *t;

    // Insert in null vector
    t = vector_insert(v, 0, 1);
    test_assert(t == &v[0]);
    test_assert(vector_length(v) == 1);

    // Insert out of bounds
    t = vector_insert(v, 100, 3);
    test_assert(t == &v[1]);
    test_assert(vector_length(v) == 2);

    // Insert between elements
    t = vector_insert(v, 1, 2);
    test_assert(t == &v[1]);
    test_assert(vector_length(v) == 3);

    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    vector_free(v);

    test_success();
}

test_case(vector_insert_swap)
{
    int *v = NULL, *t;

    // Insert in null vector
    t = vector_insert_swap(v, 0, 3);
    test_assert(t == &v[0]);
    test_assert(vector_length(v) == 1);

    // Insert out of bounds
    t = vector_insert_swap(v, 100, 2);
    test_assert(t == &v[1]);
    test_assert(vector_length(v) == 2);

    // Insert between elements
    t = vector_insert_swap(v, 0, 1);
    test_assert(t == &v[0]);
    test_assert(vector_length(v) == 3);

    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    vector_free(v);

    test_success();
}

test_case(vector_remove)
{
    int *v = NULL;

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    vector_remove(v, 0);
    test_assert(vector_length(v) == 2);
    test_assert(v[0] == 2);
    test_assert(v[1] == 3);

    vector_remove(v, 1);
    test_assert(vector_length(v) == 1);
    test_assert(v[0] == 2);

    vector_remove(v, 1);
    test_assert(vector_length(v) == 1);
    test_assert(v[0] == 2);

    vector_free(v);

    test_success();
}

test_case(vector_remove_swap)
{
    int *v = NULL;

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    vector_remove_swap(v, 0);
    test_assert(vector_length(v) == 2);
    test_assert(v[0] == 3);
    test_assert(v[1] == 2);

    vector_remove_swap(v, 1);
    test_assert(vector_length(v) == 1);
    test_assert(v[0] == 3);

    vector_remove_swap(v, 1);
    test_assert(vector_length(v) == 1);
    test_assert(v[0] == 3);

    vector_free(v);

    test_success();
}

test_case(vector_push_range)
{
    int *v = NULL;
    static int arr[] = { 1, 2, 3, 4, 5 };

    // note: compiler complains about NULL on Linux
    st_ignore_diagnostic("-Wnonnull", vector_push_range(v, NULL, 0));
    test_assert(!v);
    
    vector_push_range(v, arr, 0);
    test_assert(!v);
    
    vector_push_range(v, arr, 5);
    test_assert(vector_length(v) == 5);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    test_assert(v[4] == 5);

    vector_free(v);

    test_success();
}

test_case(vector_push_copy)
{
    typedef float vec3[3];
    vec3 *v = NULL;

    vec3 tmp;
    tmp[0] = 1.0f;
    tmp[1] = 2.0f;
    tmp[2] = 3.0f;

    vector_push_copy(v, tmp);
    test_assert(vector_length(v) == 1);
    test_assert(v[0][0] == 1.0f);
    test_assert(v[0][1] == 2.0f);
    test_assert(v[0][2] == 3.0f);

    vector_free(v);

    test_success();
}

test_case(vector_reserve)
{
    int *v = NULL;

    // Reserve less than default capacity
    test_assert(vector_reserve(v, 0) == 1);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == VECTOR_DEFAULT_CAPACITY);
    vector_free(v);

    // Reserve more than default capacity
    test_assert(vector_reserve(v, 100) == 1);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == 100);

    // Reserve less than current capacity
    test_assert(vector_reserve(v, 50) == 0);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == 100);
    vector_free(v);

    test_success();
}

test_case(vector_resize)
{
    int *v = NULL;

    // Resize null vector to 0
    test_assert(vector_resize(v, 0) == 0);

    // Resize null vector
    test_assert(vector_resize(v, 10) == 1);
    test_assert(vector_capacity(v) == 10);

    // Resize vector with capacity
    test_assert(vector_resize(v, 5) == 1);
    test_assert(vector_capacity(v) == 5);

    // Resize to 0
    test_assert(vector_resize(v, 0) == 1);
    test_assert(v == NULL);

    test_success();
}

test_case(vector_shrink)
{
    int *v = NULL;

    // Shrink null vector
    test_assert(vector_shrink(v) == 0);

    // Resize vector with elements
    vector_reserve(v, 10);
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_shrink(v) == 1);
    test_assert(vector_capacity(v) == 3);

    // Resize vector with equal length and capacity
    test_assert(vector_shrink(v) == 0);
    test_assert(vector_capacity(v) == 3);

    // Resize vector with no elements
    vector_clear(v);
    test_assert(vector_shrink(v) == 1);
    test_assert(v == NULL);

    test_success();
}

test_case(vector_clear)
{
    int *v = NULL;

    // Clear null vector
    test_assert(vector_clear(v) == 0);

    // Clear vector with elements
    vector_reserve(v, 10);
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_length(v) == 3);
    test_assert(vector_capacity(v) == 10);
    
    test_assert(vector_clear(v) == 1);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == 10);
    vector_free(v);

    test_success();
}

test_case(vector_free)
{
    int *v = NULL;

    // Free null vector
    test_assert(vector_free(v) == 0);

    // Free vector with elements
    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    test_assert(vector_free(v) == 1);
    test_assert(v == NULL);

    test_success();
}
