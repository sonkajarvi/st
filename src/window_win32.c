#include <st/window_win32.h>

#include <assert.h>

#include <windows.h>
#include <GL/gl.h>
#include <wingdi.h>
#define WGL_WGLEXT_PROTOTYPES
#include <GL/wglext.h>

#include <st/attr.h>
#include <st/common.h>

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    int index = -1;

    if (window->win32.window) {
        switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_MOUSEMOVE:
            window->mouse.position[0] = LOWORD(lparam);
            window->mouse.position[1] = HIWORD(lparam);
            break;
        
        case WM_MOUSEWHEEL:
            window->mouse.wheel = GET_WHEEL_DELTA_WPARAM(wparam) / 120.0f;
            break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
            if (msg == WM_LBUTTONDOWN) index = ST_MOUSE_LEFT;
            else if (msg == WM_RBUTTONDOWN) index = ST_MOUSE_RIGHT;
            else if (msg == WM_MBUTTONDOWN) index = ST_MOUSE_MIDDLE;
            else if (msg == WM_XBUTTONDOWN)
                index = GET_XBUTTON_WPARAM(wparam) == XBUTTON1 ? ST_MOUSE_X1 : ST_MOUSE_X2;

            window->mouse.state[index].curr = 1;
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
            if (msg == WM_LBUTTONUP) index = ST_MOUSE_LEFT;
            else if (msg == WM_RBUTTONUP) index = ST_MOUSE_RIGHT;
            else if (msg == WM_MBUTTONUP) index = ST_MOUSE_MIDDLE;
            else if (msg == WM_XBUTTONUP)
                index = GET_XBUTTON_WPARAM(wparam) == XBUTTON1 ? ST_MOUSE_X1 : ST_MOUSE_X2;

            window->mouse.state[index].curr = 0;
            break;
        }

    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void impl_win32_window_create(StWindow *window, const char *title, int width, int height)
{
    assert(window);
    assert(title);
    assert(*title);

    HINSTANCE instance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = instance;
    wc.lpszClassName = title;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    window->win32.window = CreateWindow(
        title,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        instance,
        NULL
    );

    assert(window->win32.window);
}

void impl_win32_window_destroy(StWindow *window)
{
    assert(window);
    assert(window->win32.window);

    DestroyWindow(window->win32.window);
    window->win32.window = NULL;
}

void impl_win32_window_show(StWindow *window)
{
    assert(window);
    assert(window->win32.window);
    
    ShowWindow(window->win32.window, SW_SHOW);
    UpdateWindow(window->win32.window);
}

void impl_win32_window_get_size(StWindow *window, int *width, int *height)
{
    assert(window);
    assert(window->win32.window);
    
    RECT rect;
    GetClientRect(window->win32.window, &rect);

    if (width)
        *width = rect.right - rect.left;
    if (height)
        *height = rect.bottom - rect.top;
}

void impl_win32_window_get_pos(StWindow *window, int *x, int *y)
{
    assert(window);
    assert(window->win32.window);

    RECT rect;
    GetClientRect(window->win32.window, &rect);

    if (x)
        *x = rect.left;
    if (y)
        *y = rect.top;
}

void impl_win32_poll_events(StWindow *window)
{
    assert(window);
    assert(window->win32.window);
    
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            window->is_open = false;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void impl_wgl_swap_buffers(StWindow *window)
{
    assert(window);
    assert(window->win32.window);

    SwapBuffers(GetDC(window->win32.window));
}

void impl_wgl_context_create(StWindow *window)
{
    assert(window);
    assert(window->win32.window);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    HDC dc;
    HGLRC tmp, cx;
    int format;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    GLint attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    dc = GetDC(window->win32.window);
    format = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, format, &pfd);

    tmp = wglCreateContext(dc);
    wglMakeCurrent(dc, tmp);

    ignore_diagnostic("-Wcast-function-type", wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB"));
    cx = wglCreateContextAttribsARB(dc, 0, attribs);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tmp);
    wglMakeCurrent(dc, cx);

    ReleaseDC(window->win32.window, dc);

    ignore_diagnostic("-Wcast-function-type", wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"));
    wglSwapIntervalEXT(0);
}

void impl_wgl_context_destroy(StWindow *window)
{
    assert(window);
    assert(window->win32.window);
    
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(wglGetCurrentContext());
}
