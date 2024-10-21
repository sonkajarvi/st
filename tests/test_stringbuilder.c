#include <stdlib.h>
#include <string.h>

#include <st/utility/stringbuilder.h>
#include "test.h"

test_case(stringbuilder_concat)
{
    StStringBuilder sb = {0};
    st_sb_add(&sb, "hello");
    st_sb_add(&sb, ", world");
    const char *s = st_sb_concat(&sb);

    test_assert(strcmp(s, "hello, world") == 0);
    st_sb_free(&sb);
    free((void *)s);

    test_success();
}

test_case(stringbuilder_concat_empty)
{
    StStringBuilder sb = {0};
    const char *s = st_sb_concat(&sb);

    test_assert(s == NULL);
    st_sb_free(&sb);

    test_success();
}
