#ifndef LAB3_WINDOW_H
#define LAB3_WINDOW_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <functional>

namespace Lab3 {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool init();
    void run();
    void close();

    // Chuyển đổi tọa độ màn hình sang tọa độ OpenGL (Gốc góc dưới-trái)
    int toWorldX(int screenX) const { return screenX; }
    int toWorldY(int screenY) const { return m_height - screenY; }

    int toScreenX(int worldX) const { return worldX; }
    int toScreenY(int worldY) const { return m_height - worldY; }

    // Callbacks sự kiện
    std::function<void()> onInit;
    std::function<void(int width, int height)> onResize;
    std::function<void(float deltaTime)> onUpdate;
    std::function<void()> onRender;
    std::function<void(int key, bool isDown)> onKey;
    std::function<void(int worldX, int worldY, int button, bool isDown)> onMouse;
    std::function<void(int worldX, int worldY)> onMouseMove;
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

} // namespace Lab3

#endif // LAB3_WINDOW_H
