#include <unistd.h>
#include <time.h>

#include <X11/keysym.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <st/instance.h>
#include <st/graphics/camera.h>
#include <st/input/keys.h>
#include <st/input/mouse.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>
#include <st/window.h>

#include "context_glx.h"
#include "window_x11.h"

static Atom wm_delete_window;
static XWindowAttributes window_attribs;

static uint64_t get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

static int keysym_to_key(int code)
{
    switch (code) {
    // alphabet
    case XK_a: return ST_KEY_A;
    case XK_b: return ST_KEY_B;
    case XK_c: return ST_KEY_C;
    case XK_d: return ST_KEY_D;
    case XK_e: return ST_KEY_E;
    case XK_f: return ST_KEY_F;
    case XK_g: return ST_KEY_G;
    case XK_h: return ST_KEY_H;
    case XK_i: return ST_KEY_I;
    case XK_j: return ST_KEY_J;
    case XK_k: return ST_KEY_K;
    case XK_l: return ST_KEY_L;
    case XK_m: return ST_KEY_M;
    case XK_n: return ST_KEY_N;
    case XK_o: return ST_KEY_O;
    case XK_p: return ST_KEY_P;
    case XK_q: return ST_KEY_Q;
    case XK_r: return ST_KEY_R;
    case XK_s: return ST_KEY_S;
    case XK_t: return ST_KEY_T;
    case XK_u: return ST_KEY_U;
    case XK_v: return ST_KEY_V;
    case XK_w: return ST_KEY_W;
    case XK_x: return ST_KEY_X;
    case XK_y: return ST_KEY_Y;
    case XK_z: return ST_KEY_Z;

    // numbers
    case XK_0: return ST_KEY_0;
    case XK_1: return ST_KEY_1;
    case XK_2: return ST_KEY_2;
    case XK_3: return ST_KEY_3;
    case XK_4: return ST_KEY_4;
    case XK_5: return ST_KEY_5;
    case XK_6: return ST_KEY_6;
    case XK_7: return ST_KEY_7;
    case XK_8: return ST_KEY_8;
    case XK_9: return ST_KEY_9;

    // arrow keys
    case XK_Up: return ST_KEY_UP;
    case XK_Down: return ST_KEY_DOWN;
    case XK_Left: return ST_KEY_LEFT;
    case XK_Right: return ST_KEY_RIGHT;

    // special keys
    case XK_Escape: return ST_KEY_ESCAPE;
    case XK_Shift_L: return ST_KEY_SHIFT;
    case XK_Control_L: return ST_KEY_CTRL;
    case XK_space: return ST_KEY_SPACE;
    case XK_Return: return ST_KEY_ENTER;

    // function keys
    case XK_F1: return ST_KEY_F1;
    case XK_F2: return ST_KEY_F2;
    case XK_F3: return ST_KEY_F3;
    case XK_F4: return ST_KEY_F4;
    case XK_F5: return ST_KEY_F5;
    case XK_F6: return ST_KEY_F6;
    case XK_F7: return ST_KEY_F7;
    case XK_F8: return ST_KEY_F8;
    case XK_F9: return ST_KEY_F9;
    case XK_F10: return ST_KEY_F10;
    case XK_F11: return ST_KEY_F11;
    case XK_F12: return ST_KEY_F12;

    default: return ST_KEY_UNKNOWN;
    }
}

// todo: return a status code
void impl_x11_window_create(struct st_window *window, const char *title, int width, int height)
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        st_error("Failed to connect to X server\n");
        return;
    }

    int screen = DefaultScreen(display);
    glx_init(display, screen);

    GLXFBConfig fbc;
    glx_choose_fbc(display, screen, &fbc);
    
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc);
    if (!vi) {
        st_error("Failed to get visual from FB config\n");
        return;
    }

    Visual *visual = vi->visual;
    int depth = vi->depth;
    XFree(vi);

    Window root = DefaultRootWindow(display);

    XSetWindowAttributes swa = { 0 };
    swa.colormap = XCreateColormap(display, root, visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    window->x11.window = XCreateWindow(
        display,
        root,
        0,
        0,
        width,
        height,
        0,
        depth,
        InputOutput,
        visual,
        CWBorderPixel | CWColormap | CWEventMask,
        &swa
    );

    XStoreName(display, window->x11.window, title);

    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window->x11.window, &wm_delete_window, 1);

    window->x11.display = display;
    window->x11.root = root;
    window->x11.fbc = fbc;
    window->x11.offset = get_time();

    glx_create_context(window);
}

void impl_x11_window_destroy(struct st_window *window)
{
    glx_destroy_context(window);
    
    XDestroyWindow(window->x11.display, window->x11.window);
    XCloseDisplay(window->x11.display);
}

void impl_x11_window_show(struct st_window *window)
{
    st_assert(window);
    st_assert(window->x11.display);
    st_assert(window->x11.window);

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

double impl_x11_window_time(struct st_window *window)
{
    uint64_t now = get_time();
    return (double)(now - window->x11.offset) / 1000000000.0;
}

void impl_x11_poll_events(struct st_window *window)
{
    XEvent event;
    int keycode, keysym, key;

    while (QLength(window->x11.display)) {
        XNextEvent(window->x11.display, &event);

        switch (event.type) {
        case ClientMessage:
            if (event.xclient.data.l[0] == (long)wm_delete_window) {
                window->visible = false;
                return;
            }
            break;

        case ConfigureNotify:
            st_camera_recalculate_projection(&window->camera);
            break;

        case KeyPress:
        case KeyRelease:
            keycode = event.xkey.keycode;
            keysym = XkbKeycodeToKeysym(window->x11.display, keycode, 0, 0);
            key = keysym_to_key(keysym);

            if (key == ST_KEY_UNKNOWN) {
                st_warn("Unknown key\n");
                st_warn("... code: %d\n", keycode);
                st_warn("... sym: %d\n", keysym);
                break;
            }

            window->keyboard.state[key].current = (event.type == KeyPress);
            break;

        case ButtonPress:
        case ButtonRelease:
            switch (event.xbutton.button) {
            case Button1:
                window->mouse.states[ST_MOUSE_LEFT].curr = (event.type == ButtonPress);
                break;
            case Button2:
                window->mouse.states[ST_MOUSE_MIDDLE].curr = (event.type == ButtonPress);
                break;
            case Button3:
                window->mouse.states[ST_MOUSE_RIGHT].curr = (event.type == ButtonPress);
                break;
            // todo: X1 and X2
            }
            break;
        }
    }

    XFlush(window->x11.display);
}

// void impl_glx_swap_buffers(struct st_window *window)
// {
//     glXSwapBuffers(window->x11.display, window->x11.window);
// }

// void impl_glx_context_create(struct st_window *window)
// {
//     glXCreateContextAttribsARBProc glxCreateContextAttribsABR =
//         (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
    
//     int ctx_attribs[] = {
//         GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
//         GLX_CONTEXT_MINOR_VERSION_ARB, 6,
//         None
//     };

//     window->x11.context = glxCreateContextAttribsABR(window->x11.display, window->x11.fbc, NULL, True, ctx_attribs);
//     st_assert(window->x11.context);

//     glXMakeCurrent(window->x11.display, window->x11.window, window->x11.context);

//     int major, minor;
//     glXQueryVersion(window->x11.display, &major, &minor);
//     st_log("GLX version: %d.%d\n", major, minor);
// }

// void impl_glx_context_destroy(struct st_window *window)
// {
//     glXMakeCurrent(window->x11.display, 0, 0);
//     glXDestroyContext(window->x11.display, window->x11.context);
// }

// void impl_glx_window_vsync(struct st_window *window, bool value)
// {
//     glXSwapIntervalEXTProc glXSwapIntervalEXT =
//         (glXSwapIntervalEXTProc)glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");

//     glXSwapIntervalEXT(window->x11.display, glXGetCurrentDrawable(), value);
// }
