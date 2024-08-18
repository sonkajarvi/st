#include "glad_loader.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>

void glad_init(void)
{
    static bool glad_initialized = false;
    assert(!glad_initialized);

    assert(gladLoadGL());
    glad_initialized = true;
    printf("glad: %d.%d\n", GLVersion.major, GLVersion.minor);
}
