#include "test.h"

int main()
{
    // vector
    test_declare(vector_is_empty);
    test_declare(vector_length);
    test_declare(vector_capacity);
    test_declare(vector_front);
    test_declare(vector_back);
    test_declare(vector_at);
    test_declare(vector_for);
    test_declare(vector_rof);

    test_declare(vector_push);
    test_declare(vector_push_1000000);
    test_declare(vector_pop);
    test_declare(vector_insert);
    test_declare(vector_remove);
    test_declare(vector_remove_swap);
    test_declare(vector_push_range);
    test_declare(vector_push_copy);
    test_declare(vector_reserve);
    test_declare(vector_clear);
    test_declare(vector_free);
}
