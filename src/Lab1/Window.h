#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <functional>

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool init();
    void run();
    void close();

    // Chuyển đổi tọa độ màn hình (Góc trên-trái) sang tọa độ Đề-các (Gốc ở tâm màn hình)
    int toCartesianX(int screenX) const { return screenX - m_width / 2; }
    int toCartesianY(int screenY) const { return m_height / 2 - screenY; }

    // Chuyển ngược lại
    int toScreenX(int cartX) const { return cartX + m_width / 2; }
    int toScreenY(int cartY) const { return m_height / 2 - cartY; }

    // Callbacks sự kiện
    std::function<void()> onInit;
    std::function<void(int width, int height)> onResize;
    std::function<void(float deltaTime)> onUpdate;
    std::function<void()> onRender;
    std::function<void(int key, bool isDown)> onKey;
    std::function<void(int cartX, int cartY, int button, bool isDown)> onMouse;
    std::function<void(int cartX, int cartY)> onMouseMove;
    std::function<void(int delta)> onMouseWheel;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    HWND getHWND() const { return m_hwnd; }

private:
    std::string m_title;
    int m_width;
    int m_height;
    bool m_running;

    HWND m_hwnd;
    HDC m_hdc;
    HGLRC m_hrc;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool setupPixelFormat();
    bool createGLContext();
    void cleanup();
};

#endif // WINDOW_H
