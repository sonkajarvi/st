#include "window_x11.h"

#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <glad/glad_glx.h>

#include "common.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

static Atom wm_delete_window;
static XWindowAttributes window_attribs;

void impl_x11_window_create(struct st_window *window, const char *title, int width, int height)
{
    window->x11.display = XOpenDisplay(NULL);
    assert(window->x11.display);

    assert(gladLoadGLX(window->x11.display, DefaultScreen(window->x11.display)));

    int vi_attribs[] = {
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

    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(window->x11.display, DefaultScreen(window->x11.display), vi_attribs, &fbcount);
    assert(fbc);

    int best = -1, worst = -1, best_samples = -1, worst_samples = 999;
    for (int i = 0; i < fbcount; i++) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(window->x11.display, fbc[i]);
        if (vi) {
            int buf, samples;
            glXGetFBConfigAttrib(window->x11.display, fbc[i], GLX_SAMPLE_BUFFERS, &buf);
            glXGetFBConfigAttrib(window->x11.display, fbc[i], GLX_SAMPLES, &samples);
        
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

    window->x11.fbc = fbc[best];
    assert(window->x11.fbc);
    XFree(fbc);

    XVisualInfo *vi = glXGetVisualFromFBConfig(window->x11.display, window->x11.fbc);
    assert(vi);
    
    window->x11.root = DefaultRootWindow(window->x11.display);
    assert(window->x11.root);

    XSetWindowAttributes swa = { 0 };
    swa.colormap = XCreateColormap(window->x11.display, window->x11.root, vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    window->x11.window = XCreateWindow(
        window->x11.display,
        window->x11.root,
        0, 0,
        width, height,
        0,
        vi->depth,
        InputOutput,
        vi->visual,
        CWBorderPixel | CWColormap | CWEventMask,
        &swa
    );
    assert(window->x11.window);
    XFree(vi);

    XStoreName(window->x11.display, window->x11.window, title);

    wm_delete_window = XInternAtom(window->x11.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->x11.display, window->x11.window, &wm_delete_window, 1);
}

void impl_x11_window_destroy(struct st_window *window)
{
    XDestroyWindow(window->x11.display, window->x11.window);
    XCloseDisplay(window->x11.display);
}

void impl_x11_window_show(struct st_window *window)
{
    assert(window);

    XMapWindow(window->x11.display, window->x11.window);
}

void impl_x11_window_get_size(struct st_window *window, int *width, int *height)
{
    XGetWindowAttributes(window->x11.display, window->x11.window, &window_attribs);

    if (width)
        *width = window_attribs.width;
    if (height)
        *height = window_attribs.height;
}

void impl_x11_window_get_pos(struct st_window *window, int *x, int *y)
{
    XGetWindowAttributes(window->x11.display, window->x11.window, &window_attribs);

    if (x)
        *x = window_attribs.x;
    if (y)
        *y = window_attribs.y;
}

void impl_x11_poll_events(struct st_window *window)
{
    XEvent event;
    while (QLength(window->x11.display)) {
        XNextEvent(window->x11.display, &event);

        switch (event.type) {
        case ClientMessage:
            if (event.xclient.data.l[0] == (long)wm_delete_window) {
                window->is_open = false;
                return;
            }
            break;
        }
    }

    XFlush(window->x11.display);
}

void impl_glx_swap_buffers(struct st_window *window)
{
    glXSwapBuffers(window->x11.display, window->x11.window);
}

void impl_glx_context_create(struct st_window *window)
{
    glXCreateContextAttribsARBProc glxCreateContextAttribsABR =
        (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
    
    int ctx_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        None
    };

    window->x11.context = glxCreateContextAttribsABR(window->x11.display, window->x11.fbc, NULL, True, ctx_attribs);
    assert(window->x11.context);

    glXMakeCurrent(window->x11.display, window->x11.window, window->x11.context);

    int major, minor;
    glXQueryVersion(window->x11.display, &major, &minor);
    printf("GLX version: %d.%d\n", major, minor);
}

void impl_glx_context_destroy(struct st_window *window)
{
    glXMakeCurrent(window->x11.display, 0, 0);
    glXDestroyContext(window->x11.display, window->x11.context);
}
