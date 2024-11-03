#include <stdio.h> // IWYU pragma: keep

#include "test.h"

int main()
{
    // Vector
    test_declare(st_vector_empty);
    test_declare(st_vector_length);
    test_declare(st_vector_capacity);
    test_declare(st_vector_front);
    test_declare(st_vector_back);
    test_declare(st_vector_at);
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

    // String builder
    test_declare(st_strbuilder_free);
    test_declare(st_strbuilder_append);
    test_declare(st_strbuilder_insert);
    test_declare(st_strbuilder_empty);
}
