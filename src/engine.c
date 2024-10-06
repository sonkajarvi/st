#include <stdlib.h>
#include <string.h>

#include <st/engine.h>
#include <st/utils/print.h>

#include "callbacks.h"

static StEngine *ST_INTERNAL_engine_context = NULL;

void st_engine_init(void)
{
    if (ST_INTERNAL_engine_context) {
        st_warn("Failed to initialize engine context. Engine already initialized\n");
        return;
    }

    ST_INTERNAL_engine_context = malloc(sizeof(*ST_INTERNAL_engine_context));
    if (!ST_INTERNAL_engine_context) {
        st_error("Failed to allocate memory for engine struct\n");
        return;
    }

    memset(ST_INTERNAL_engine_context, 0, sizeof(*ST_INTERNAL_engine_context));
    set_platform_callbacks(ST_INTERNAL_engine_context);
    
    // use opengl, as it's the only graphics backend available, for now
    set_graphics_callbacks(ST_INTERNAL_engine_context, ST_GRAPHICS_OPENGL);

    st_debug("Engine context initialized\n");
}

void st_engine_destroy(void)
{
    if (!ST_INTERNAL_engine_context) {
        st_error("Failed to destroy engine. No context found\n");
        return;
    }

    free(ST_INTERNAL_engine_context);
    ST_INTERNAL_engine_context = NULL;

    st_debug("Engine context destroyed\n");
}

StEngine *st_engine_context(void)
{
    return ST_INTERNAL_engine_context;
}
