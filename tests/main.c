#include "test.h"

int main()
{
    // vector
    test_declare(vector_is_empty);
    test_declare(vector_length);
    test_declare(vector_capacity);
    test_declare(vector_front_ref);
    test_declare(vector_back_ref);
    test_declare(vector_at);

    test_declare(vector_push);
    test_declare(vector_pop);
    test_declare(vector_insert);
    test_declare(vector_remove);
    test_declare(vector_push_range);
    test_declare(vector_push_copy);
    test_declare(vector_reserve);
    test_declare(vector_clear);
    test_declare(vector_free);
}
