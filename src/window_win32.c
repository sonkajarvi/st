#include <st/window_win32.h>

#include <assert.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#define WGL_WGLEXT_PROTOTYPES
#include <GL/wglext.h>

#include <st/attr.h>
#include <st/common.h>
#include <st/keys.h>

static int vk_to_key(int vk)
{
    switch (vk) {
    // alphabet
    case 'A': return ST_KEY_A;
    case 'B': return ST_KEY_B;
    case 'C': return ST_KEY_C;
    case 'D': return ST_KEY_D;
    case 'E': return ST_KEY_E;
    case 'F': return ST_KEY_F;
    case 'G': return ST_KEY_G;
    case 'H': return ST_KEY_H;
    case 'I': return ST_KEY_I;
    case 'J': return ST_KEY_J;
    case 'K': return ST_KEY_K;
    case 'L': return ST_KEY_L;
    case 'M': return ST_KEY_M;
    case 'N': return ST_KEY_N;
    case 'O': return ST_KEY_O;
    case 'P': return ST_KEY_P;
    case 'Q': return ST_KEY_Q;
    case 'R': return ST_KEY_R;
    case 'S': return ST_KEY_S;
    case 'T': return ST_KEY_T;
    case 'U': return ST_KEY_U;
    case 'V': return ST_KEY_V;
    case 'W': return ST_KEY_W;
    case 'X': return ST_KEY_X;
    case 'Y': return ST_KEY_Y;
    case 'Z': return ST_KEY_Z;

    // numbers
    case '0': return ST_KEY_0;
    case '1': return ST_KEY_1;
    case '2': return ST_KEY_2;
    case '3': return ST_KEY_3;
    case '4': return ST_KEY_4;
    case '5': return ST_KEY_5;
    case '6': return ST_KEY_6;
    case '7': return ST_KEY_7;
    case '8': return ST_KEY_8;
    case '9': return ST_KEY_9;

    // arrow keys
    case VK_UP: return ST_KEY_UP;
    case VK_DOWN: return ST_KEY_DOWN;
    case VK_LEFT: return ST_KEY_LEFT;
    case VK_RIGHT: return ST_KEY_RIGHT;
    
    // special keys
    case VK_ESCAPE: return ST_KEY_ESCAPE;
    case VK_SHIFT: return ST_KEY_SHIFT;
    case VK_CONTROL: return ST_KEY_CTRL;
    case VK_SPACE: return ST_KEY_SPACE;
    case ST_KEY_ENTER: return ST_KEY_ENTER;

    // function keys
    case VK_F1: return ST_KEY_F1;
    case VK_F2: return ST_KEY_F2;
    case VK_F3: return ST_KEY_F3;
    case VK_F4: return ST_KEY_F4;
    case VK_F5: return ST_KEY_F5;
    case VK_F6: return ST_KEY_F6;
    case VK_F7: return ST_KEY_F7;
    case VK_F8: return ST_KEY_F8;
    case VK_F9: return ST_KEY_F9;
    case VK_F10: return ST_KEY_F10;
    case VK_F11: return ST_KEY_F11;
    case VK_F12: return ST_KEY_F12;

    default: return ST_KEY_UNKNOWN;
    }
}

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    int index = -1;
    int vk, key, c;

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
        
        case WM_KEYUP:
        case WM_KEYDOWN:
            key = vk_to_key((vk = (int)wparam));
            if (key == ST_KEY_UNKNOWN) {
                c = MapVirtualKey(vk, MAPVK_VK_TO_CHAR);
                printf("unknown key: (0x%x) %c\n", vk, c);
                break;
            }

            window->keyboard.state[key].curr = msg == WM_KEYDOWN;
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

    assert(QueryPerformanceCounter((LARGE_INTEGER *)&window->win32.offset));
    assert(QueryPerformanceFrequency((LARGE_INTEGER *)&window->win32.frequency));
}

double impl_win32_engine_time(StWindow *window)
{
    assert(window);
    assert(window->win32.window);

    UINT64 counter;
    QueryPerformanceCounter((LARGE_INTEGER *)&counter);
    return (double)(counter - window->win32.offset) / window->win32.frequency;
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
