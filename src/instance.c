#include <stdlib.h>
#include <string.h>

#include <st/event.h>
#include <st/instance.h>
#include <st/utility/print.h>

#include "callbacks.h"

static St __st = { 0 };

void st_hello(void)
{
    if (__st.initialized) {
        st_warn("Failed to create instance. Instance already exists\n");
        return;
    }

    st_log("Commit %s..\n", ST_COMMIT);

    memset(&__st, 0, sizeof(__st));
    __st.initialized = true;
    st_debug("Instance created\n");
    
    set_platform_callbacks(&__st);

    // use opengl, as it's the only graphics backend available, for now
    set_graphics_callbacks(&__st, ST_GRAPHICS_OPENGL);
}

void st_goodbye(void)
{
    if (!__st.initialized) {
        st_error("Failed to destroy instance. No instance found\n");
        return;
    }

    // Clear event listeners
    for (int i = 0; i < ST_EVENT_LENGTH; i++)
        st_event_clear(i);

    memset(&__st, 0, sizeof(__st));

    st_debug("Instance destroyed\n");
}

St *st_instance(void)
{
    if (!__st.initialized)
        return NULL;

    return &__st;
}
