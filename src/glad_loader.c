#include <st/glad_loader.h>

#include <assert.h>
#include <stdbool.h>

#include <glad/glad.h>

#include <st/print.h>

void glad_init(void)
{
    static bool glad_initialized = false;
    assert(!glad_initialized);

    assert(gladLoadGL());
    glad_initialized = true;
    st_log("glad: %d.%d\n", GLVersion.major, GLVersion.minor);
}
