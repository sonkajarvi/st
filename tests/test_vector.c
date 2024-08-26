#include "test.h"

#include <vector.h>

test_case(vector_is_empty)
{
    int *v = NULL;
    test_assert(vector_is_empty(v));

    vector_push(v, 1);
    test_assert(!vector_is_empty(v));

    vector_free(v);
    test_assert(vector_is_empty(v));

    test_success();
}

test_case(vector_length)
{
    int *v = NULL;
    test_assert(vector_length(v) == 0);

    vector_push(v, 1);
    test_assert(vector_length(v) == 1);

    vector_free(v);
    test_assert(vector_length(v) == 0);

    test_success();
}

test_case(vector_capacity)
{
    int *v = NULL;
    test_assert(vector_capacity(v) == 0);

    vector_push(v, 1);
    test_assert(vector_capacity(v) == VECTOR_DEFAULT_CAPACITY);

    vector_free(v);
    test_assert(vector_capacity(v) == 0);

    test_success();
}

test_case(vector_front_ref)
{
    int *v = NULL;
    test_assert(vector_front_ref(v) == NULL);

    vector_push(v, 1);
    test_assert(vector_front_ref(v) == &v[0]);

    vector_free(v);
    test_assert(vector_front_ref(v) == NULL);

    test_success();
}

test_case(vector_back_ref)
{
    int *v = NULL;
    test_assert(vector_back_ref(v) == NULL);

    vector_push(v, 1);
    test_assert(vector_back_ref(v) == &v[0]);

    vector_free(v);
    test_assert(vector_back_ref(v) == NULL);

    test_success();
}

test_case(vector_push)
{
    int *v = NULL;

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    vector_push(v, 4);
    vector_push(v, 5);
    
    test_assert(vector_length(v) == 5);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);
    test_assert(v[3] == 4);
    test_assert(v[4] == 5);

    vector_free(v);

    test_success();
}

test_case(vector_pop)
{
    int *v = NULL;

    vector_push(v, 1);
    test_assert(vector_length(v) == 1);

    vector_pop(v);
    test_assert(vector_length(v) == 0);
    
    vector_pop(v);
    test_assert(vector_length(v) == 0);

    vector_free(v);

    test_success();
}

test_case(vector_insert)
{
    int *v = NULL;

    vector_insert(v, 0, 1);
    test_assert(vector_length(v) == 1);
    vector_free(v);

    vector_push(v, 2);
    vector_push(v, 3);
    vector_insert(v, 0, 1);
    
    test_assert(vector_length(v) == 3);
    test_assert(v[0] == 1);
    test_assert(v[1] == 2);
    test_assert(v[2] == 3);

    vector_insert(v, 3, 4);
    test_assert(vector_length(v) == 4);
    test_assert(v[3] == 4);

    vector_insert(v, 5, 6);
    test_assert(vector_length(v) == 4);

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

test_case(vector_push_range)
{
    int *v = NULL;
    static int arr[] = { 1, 2, 3, 4, 5 };

    vector_push_range(v, NULL, 0);
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

test_case(vector_reserve)
{
    int *v = NULL;

    vector_reserve(v, 100);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == 100);
    
    vector_reserve(v, 50);
    test_assert(vector_length(v) == 0);
    test_assert(vector_capacity(v) == 100);

    vector_free(v);

    test_success();
}

test_case(vector_clear)
{
    int *v = NULL;
    vector_clear(v);

    vector_push(v, 1);
    test_assert(vector_length(v) == 1);
    test_assert(vector_capacity(v) == VECTOR_DEFAULT_CAPACITY);
    
    vector_clear(v);
    test_assert(vector_capacity(v) == VECTOR_DEFAULT_CAPACITY);

    vector_free(v);

    test_success();
}

test_case(vector_free)
{
    int *v = NULL;
    vector_free(v);

    vector_push(v, 1);
    test_assert(v);

    vector_free(v);
    test_assert(!v);

    test_success();
}

test_case(vector_push_struct)
{
    typedef struct {
        int a;
        float b;
    } S;

    S *v = NULL;

    S tmp = { 0xdeadbeef, 3.14f };
    vector_push(v, tmp);
    test_assert(v[0].a == 0xdeadbeef);
    test_assert(v[0].b == 3.14f);

    vector_free(v);

    test_success();
}