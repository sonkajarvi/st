#include <stdlib.h>
#include <string.h>

#include <st/instance.h>
#include <st/utility/print.h>

static struct st __st = {0};

int st_hello(void)
{
    if (__st.init)
        return ST_EINIT;

    st_log("Commit %.7s... (\"%s\")\n", ST_COMMIT_HASH, ST_COMMIT_SUBJECT);

    memset(&__st, 0, sizeof(__st));
    __st.init = true;

    st_debug("Library instance created\n");

    st_set_platform_callbacks(&__st);
    // Use OpenGL, as it's the only graphics backend available, for now
    st_set_graphics_callbacks(&__st, ST_GRAPHICS_OPENGL);

    return 0;
}

void st_goodbye(void)
{
    if (!__st.init)
        return;

    __st.init = false;

    st_debug("Library instance destroyed\n");
}

struct st *st_instance(void)
{
    if (!__st.init)
        return NULL;

    return &__st;
}
