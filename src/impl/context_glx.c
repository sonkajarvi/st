#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <glad/gl.h>
#include <glad/glx.h>

#include <st/utility/print.h>
#include <st/window.h>

#include "context_glx.h"

// todo: return a status code
void glx_init(Display *display, int screen)
{
    int glx_ver = gladLoaderLoadGLX(display, screen);
    if (!glx_ver) {
        st_error("Failed to load GLX functions\n");
        return;
    }
    st_debug("GLX functions loaded for %d.%d\n", GLAD_VERSION_MAJOR(glx_ver), GLAD_VERSION_MINOR(glx_ver));
}

// todo: return a status code
void glx_create_context(struct st_window *window)
{
    int ctx_attrs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    window->x11.context = glXCreateContextAttribsARB(window->x11.display, window->x11.fbc, NULL, True, ctx_attrs);
    if (!window->x11.context) {
        st_error("Failed to create GLX context\n");
        return;
    }
    glXMakeCurrent(window->x11.display, window->x11.window, window->x11.context);

    int major, minor;
    glXQueryVersion(window->x11.display, &major, &minor);
    st_debug("GLX context created for %d.%d\n", major, minor);
    
    int gl_ver = gladLoaderLoadGL();
    if (!gl_ver) {
        st_error("Failed to load GL functions\n");
        return;
    }
    st_debug("GL functions loaded for %d.%d\n", GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver));
}

// todo: return a status code
void glx_destroy_context(struct st_window *window)
{
    glXMakeCurrent(window->x11.display, None, NULL);
    glXDestroyContext(window->x11.display, window->x11.context);

    st_debug("GLX context destroyed\n");
}

// todo: return a status code
void glx_choose_fbc(Display *display, int screen, GLXFBConfig *config)
{
    // these probably should not be set like this
    int fb_attrs[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };

    int count;
    GLXFBConfig *configs = glXChooseFBConfig(display, screen, fb_attrs, &count);
    if (!configs) {
        st_error("Failed to choose a FB config\n");
        return;
    }

    int best = -1, worst = -1, best_samples = -1, worst_samples = 999;
    for (int i = 0; i < count; i++) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, configs[i]);
        if (vi) {
            int buf, samples;
            glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLE_BUFFERS, &buf);
            glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLES, &samples);
        
            if (best < 0 || (buf && samples > best_samples)) {
                best = i;
                best_samples = samples;
            }
            if (worst < 0 || !buf || samples < worst_samples) {
                worst = i;
                worst_samples = samples;
            }
        }
        XFree(vi);
    }
    *config = configs[best];
    XFree(configs);
}

void impl_glx_swap_buffers(struct st_window *window)
{
    glXSwapBuffers(window->x11.display, window->x11.window);
}

void impl_glx_window_vsync(struct st_window *window, bool on)
{
    glXSwapIntervalEXT(window->x11.display, window->x11.window, on);
}
