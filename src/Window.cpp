#include "Window.h"
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height), m_running(false),
      m_hwnd(NULL), m_hdc(NULL), m_hrc(NULL) {}

Window::~Window() {
    cleanup();
}

bool Window::setupPixelFormat() {
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
        24, // 24-bit depth buffer
        8,  // 8-bit stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (!pixelFormat) {
        std::cerr << "Failed to find suitable PixelFormat." << std::endl;
        return false;
    }

    if (!SetPixelFormat(m_hdc, pixelFormat, &pfd)) {
        std::cerr << "Failed to set PixelFormat." << std::endl;
        return false;
    }

    return true;
}

bool Window::createGLContext() {
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc) {
        std::cerr << "Failed to get Device Context." << std::endl;
        return false;
    }

    if (!setupPixelFormat()) {
        return false;
    }

    m_hrc = wglCreateContext(m_hdc);
    if (!m_hrc) {
        std::cerr << "Failed to create OpenGL Rendering Context." << std::endl;
        return false;
    }

    if (!wglMakeCurrent(m_hdc, m_hrc)) {
        std::cerr << "Failed to activate OpenGL Rendering Context." << std::endl;
        return false;
    }

    return true;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* pWindow = NULL;
    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    } else {
        pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pWindow) {
        return pWindow->handleMessage(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_SIZE: {
        m_width = LOWORD(lParam);
        m_height = HIWORD(lParam);
        if (onResize && m_height > 0) {
            onResize(m_width, m_height);
        }
        return 0;
    }
    case WM_KEYDOWN:
        if (onKey) onKey(static_cast<int>(wParam), true);
        return 0;
    case WM_KEYUP:
        if (onKey) onKey(static_cast<int>(wParam), false);
        return 0;
    case WM_LBUTTONDOWN:
        if (onMouse) onMouse(LOWORD(lParam), HIWORD(lParam), 0, true);
        return 0;
    case WM_LBUTTONUP:
        if (onMouse) onMouse(LOWORD(lParam), HIWORD(lParam), 0, false);
        return 0;
    case WM_RBUTTONDOWN:
        if (onMouse) onMouse(LOWORD(lParam), HIWORD(lParam), 1, true);
        return 0;
    case WM_RBUTTONUP:
        if (onMouse) onMouse(LOWORD(lParam), HIWORD(lParam), 1, false);
        return 0;
    case WM_MOUSEMOVE:
        if (onMouseMove) onMouseMove(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_MOUSEWHEEL:
        if (onMouseWheel) onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    case WM_CLOSE:
        m_running = false;
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}

bool Window::init() {
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.lpszClassName = "DHMT_OpenGL_Class";

    if (!RegisterClassExA(&wc)) {
        std::cerr << "Failed to register window class." << std::endl;
        return false;
    }

    RECT rect = { 0, 0, m_width, m_height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindowExA(
        0,
        wc.lpszClassName,
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL, NULL, hInstance, this
    );

    if (!m_hwnd) {
        std::cerr << "Failed to create window." << std::endl;
        return false;
    }

    if (!createGLContext()) {
        return false;
    }

    if (onInit) {
        onInit();
    }

    if (onResize) {
        onResize(m_width, m_height);
    }

    return true;
}

void Window::run() {
    m_running = true;
    MSG msg = {};

    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (m_running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                m_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!m_running) break;

        QueryPerformanceCounter(&currentTime);
        float deltaTime = static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        lastTime = currentTime;

        if (onUpdate) {
            onUpdate(deltaTime);
        }

        if (onRender) {
            onRender();
            SwapBuffers(m_hdc);
        }

        Sleep(1); // Yield CPU to avoid 100% spin
    }
}

void Window::close() {
    m_running = false;
}

void Window::cleanup() {
    if (m_hrc) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hrc);
        m_hrc = NULL;
    }
    if (m_hdc && m_hwnd) {
        ReleaseDC(m_hwnd, m_hdc);
        m_hdc = NULL;
    }
}
