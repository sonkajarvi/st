#include <stdio.h> // IWYU pragma: keep

#include "test.h"

int main()
{
    // Vector
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

    // Args parser
    test_declare(args_short_option);
    test_declare(args_short_option_combined);
    test_declare(args_short_option_args);
    test_declare(args_short_option_args_combined);
    test_declare(args_long_option);
    test_declare(args_long_option_args);
    test_declare(args_fixed);

    test_declare(args_short_option_args_array);
    test_declare(args_short_option_args_array_combined);
    test_declare(args_short_option_args_array_next);
    test_declare(args_short_option_args_array_trailing);
    test_declare(args_long_option_args_array);
    test_declare(args_long_option_args_array_next);

    test_declare(args_array_escaped_separator);
    test_declare(args_array_escaped_separators);
    test_declare(args_array_escaped_separator_end);
    test_declare(args_empty);
    test_declare(args_short_option_no_id);
    test_declare(args_long_option_no_id);
    test_declare(args_short_option_args_no_args);
    test_declare(args_long_option_args_no_args);
    test_declare(args_long_option_args_no_args2);
}
