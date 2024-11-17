#include <stdlib.h>
#include <string.h>

#include <st/instance.h>
#include <st/utility/print.h>

static St __st = { 0 };

void st_hello(void)
{
    if (__st.initialized) {
        st_warn("Failed to create instance. Instance already exists\n");
        return;
    }

    st_log("Commit %.7s... ('%s')\n", ST_COMMIT_HASH, ST_COMMIT_SUBJECT);

    memset(&__st, 0, sizeof(__st));
    __st.initialized = true;
    st_debug("Instance created\n");
    
    st_set_platform_callbacks(&__st);

    // use opengl, as it's the only graphics backend available, for now
    st_set_graphics_callbacks(&__st, ST_GRAPHICS_OPENGL);
}

void st_goodbye(void)
{
    if (!__st.initialized) {
        st_error("Failed to destroy instance. No instance found\n");
        return;
    }

    memset(&__st, 0, sizeof(__st));

    st_debug("Instance destroyed\n");
}

St *st_instance(void)
{
    if (!__st.initialized)
        return NULL;

    return &__st;
}
