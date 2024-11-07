#include <stdio.h> // IWYU pragma: keep

#include "test.h"

int main()
{
    // Vector
    test_declare(st_vector_empty);
    test_declare(st_vector_length);
    test_declare(st_vector_capacity);
    test_declare(st_vector_at);

    test_declare(st_vector_begin);
    test_declare(st_vector_end);
    test_declare(st_vector_for);
    test_declare(st_vector_rof);

    test_declare(st_vector_push);
    test_declare(st_vector_push_range);
    test_declare(st_vector_insert);
    test_declare(st_vector_insert_swap);
    test_declare(st_vector_pop);
    test_declare(st_vector_remove);
    test_declare(st_vector_remove_swap);
    test_declare(st_vector_reserve);
    test_declare(st_vector_resize);
    test_declare(st_vector_shrink);
    test_declare(st_vector_clear);
    test_declare(st_vector_free);
}
