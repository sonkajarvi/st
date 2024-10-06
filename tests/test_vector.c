#include "test.h"

#include <st/utils/compiler.h>
#include <st/utils/vector.h>

test_case(vector_is_empty)
{
    int *v = NULL;
    test_assert(vector_is_empty(v) == 1);

    vector_push(v, 1);
    test_assert(vector_is_empty(v) == 0);

    vector_free(v);
    test_assert(vector_is_empty(v) == 1);

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

test_case(vector_front)
{
    int *v = NULL;
    test_assert(vector_front(v) == NULL);

    vector_push(v, 1);
    test_assert(vector_front(v) == &v[0]);

    vector_free(v);
    test_assert(vector_front(v) == NULL);

    test_success();
}

test_case(vector_back)
{
    int *v = NULL;
    test_assert(vector_back(v) == NULL);

    vector_push(v, 1);
    test_assert(vector_back(v) == &v[0]);

    vector_free(v);
    test_assert(vector_back(v) == NULL);

    test_success();
}

test_case(vector_at)
{
    int *v = NULL;
    test_assert(vector_at(v, 0) == NULL);

    vector_push(v, 1);
    test_assert(*vector_at(v, 0) == 1);
    test_assert(vector_at(v, 1) == NULL);

    vector_free(v);

    test_success();
}

test_case(vector_for)
{
    int *v = NULL;

    vector_for(v, int, i) {
        test_assert(0);
    }

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    int count = 0;
    vector_for(v, int, i) {
        test_assert(v[count++] == *i);
    }
    test_assert(count == 3);

    vector_free(v);

    test_success();
}

test_case(vector_rof)
{
    int *v = NULL;

    vector_for(v, int, i) {
        test_assert(0);
    }

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);

    int count = 3;
    vector_rof(v, int, i) {
        test_assert(v[(count--) - 1] == *i);
    }
    test_assert(count == 0);

    vector_free(v);

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

test_case(vector_push_1000000)
{
    int *v = NULL;

    for (int i = 0; i < 1000000; i++) {
        vector_push(v, i);
    }

    test_assert(vector_length(v) == 1000000);
    int count = 0;
    vector_for(v, int, i) {
        test_assert(v[count++] == *i);
    }

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
