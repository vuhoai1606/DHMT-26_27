#include <iostream>
#include <iomanip>
#include "Window.h"
#include "Camera.h"
#include "Shapes.h"

enum class DemoMode {
    LANDSCAPE_2D = 1,     // [1] Bức tranh phong cảnh 2D hoàn chỉnh
    CAR_3D       = 2,     // [2] Vật thể 3D hoàn chỉnh: Chiếc xe hơi thể thao
    HELICOPTER_3D= 3      // [3] Vật thể 3D hoàn chỉnh: Máy bay trực thăng
};

DemoMode currentMode = DemoMode::CAR_3D;
Camera camera;
float totalTime = 0.0f;
float objectTime = 0.0f;              // Thời gian chuyển động của vật thể
bool isObjectMoving = true;           // Bật/tắt chuyển động cơ cấu (bánh xe, cánh quạt...)
bool isWireframe = false;             // Bật/tắt chế độ khung dây (Wireframe/Solid)

void printMenu() {
    std::cout << "==========================================================" << std::endl;
    std::cout << "      DO HOA MAY TINH (DHMT) - VAT THE 2D & 3D HOAN CHINH " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << " [1] BUC TRANH 2D HOAN CHINH (PHONG CANH DONG QUE):" << std::endl;
    std::cout << "     - Bau troi xanh, may trang troi, mat troi toa nang xoay" << std::endl;
    std::cout << "     - Doi co xanh, con duong dat uon luon" << std::endl;
    std::cout << "     - Ngoi nha (ong khoi toa khoi, cua so kinh, cua go)" << std::endl;
    std::cout << "     - Cay tao sum sue voi qua do" << std::endl;
    std::cout << "     - Coi xay gio co 4 canh quat tu xoay dong bo" << std::endl;
    std::cout << " [2] VAT THE 3D HOAN CHINH (CHIEC XE O TO 3D THE THAO):" << std::endl;
    std::cout << "     - Than xe the thao do kim loai, can truoc/sau" << std::endl;
    std::cout << "     - Cabin kinh guong phan xa, guong chieu hau, ong xa kep" << std::endl;
    std::cout << "     - 4 banh xe cao su co mam la-zang bac 5 chau TU LAN TRON" << std::endl;
    std::cout << "     - Cap den pha vang truoc, den hau do, luoi tan nhiet" << std::endl;
    std::cout << "     - Mat duong asphalt co vach son trang" << std::endl;
    std::cout << " [3] VAT THE 3D HOAN CHINH (MAY BAY TRUC THANG 3D):" << std::endl;
    std::cout << "     - Than may bay khi dong hoc, buong lai kinh lom lon" << std::endl;
    std::cout << "     - Canh duoi dung/ngang, cang dap ong thep kep" << std::endl;
    std::cout << "     - Canh quat chinh 4 canh & Canh quat duoi QUAY TIT" << std::endl;
    std::cout << "     - Hieu ung bay lo lung (hovering) nhip nhang" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << " PHIM DIEU KHIEN:" << std::endl;
    std::cout << " - Phim [1, 2, 3]  : Chuyen che do xem" << std::endl;
    std::cout << " - Phim [W]        : Bat / Tat che do KHUNG DAY (Wireframe / Solid)" << std::endl;
    std::cout << " - Phim [T]        : Bat / Tat CHUYEN DONG (Banh xe, Canh quat)" << std::endl;
    std::cout << " - Phim [Space]    : Bat / Tat TU DONG XOAY CAMERA toan canh" << std::endl;
    std::cout << " - Phim [R]        : Reset goc nhin Camera ve mac dinh" << std::endl;
    std::cout << " - Chuot trai + keo: Xoay goc nhin 3D quanh vat the (Orbit)" << std::endl;
    std::cout << " - Chuot phai + keo: Di chuyen goc nhin (Pan)" << std::endl;
    std::cout << " - Con lan chuot   : Phong to / Thu nho (Zoom)" << std::endl;
    std::cout << " - Phim [ESC]      : Thoat chuong trinh" << std::endl;
    std::cout << "==========================================================" << std::endl;
}

void setWindowModeTitle(Window* pWin) {
    std::string title = "DHMT - OpenGL [";
    switch (currentMode) {
        case DemoMode::LANDSCAPE_2D:  title += "1. Buc tranh 2D Phong canh Dong que"; break;
        case DemoMode::CAR_3D:        title += "2. Vat the 3D: Chiec Xe Hoi The Thao"; break;
        case DemoMode::HELICOPTER_3D: title += "3. Vat the 3D: May Bay Truc Thang"; break;
    }
    title += isWireframe ? " | WIREFRAME]" : " | SOLID]";
    SetWindowTextA(pWin->getHWND(), title.c_str());
}

int main() {
    printMenu();

    Window window("DHMT - Vat The 2D & 3D Hoan Chinh", 1024, 700);

    window.onInit = [&]() {
        // Màu nền xám đậm hiện đại làm nổi bật mô hình
        glClearColor(0.12f, 0.13f, 0.16f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Khử răng cưa cho đường nét mượt mà
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setWindowModeTitle(&window);
    };

    window.onResize = [&](int width, int height) {
        if (height == 0) height = 1;
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    };

    window.onUpdate = [&](float deltaTime) {
        totalTime += deltaTime;
        if (isObjectMoving) {
            objectTime += deltaTime;
        }
        camera.update(deltaTime);
    };

    window.onRender = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Áp dụng chế độ Khung dây (Wireframe) hoặc Mặt đặc (Solid)
        if (isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        bool is3D = (currentMode != DemoMode::LANDSCAPE_2D);

        if (is3D) {
            glEnable(GL_DEPTH_TEST);
            window.onResize(window.getWidth(), window.getHeight());
            camera.apply3D();

            // Trục tọa độ 3D tham chiếu
            Shapes::drawAxes3D(2.5f);

            switch (currentMode) {
                case DemoMode::CAR_3D:
                    Shapes::render3DCar(objectTime);
                    break;
                case DemoMode::HELICOPTER_3D:
                    Shapes::render3DHelicopter(objectTime);
                    break;
                default:
                    break;
            }
        } else {
            // Chế độ 2D: Tắt Depth test & Ánh sáng, dùng phép chiếu trực giao
            glDisable(GL_DEPTH_TEST);
            Shapes::disableLighting();
            camera.apply2D(window.getWidth(), window.getHeight());

            // Vẽ bức tranh 2D phong cảnh hoàn chỉnh
            Shapes::render2DLandscape(objectTime);
        }
    };

    window.onKey = [&](int key, bool isDown) {
        if (!isDown) return;

        if (key >= '1' && key <= '3') {
            currentMode = static_cast<DemoMode>(key - '0');
            setWindowModeTitle(&window);
            std::cout << "-> Chuyen sang Che do [" << (key - '0') << "]" << std::endl;
        } else if (key == 'W' || key == 'w') {
            isWireframe = !isWireframe;
            setWindowModeTitle(&window);
            std::cout << "-> Che do Khung day (Wireframe): " << (isWireframe ? "BAT" : "TAT") << std::endl;
        } else if (key == 'T' || key == 't') {
            isObjectMoving = !isObjectMoving;
            std::cout << "-> Chuyen dong bo phan (Banh xe / Canh quat): " << (isObjectMoving ? "BAT" : "TAT") << std::endl;
        } else if (key == VK_SPACE) {
            camera.autoRotate = !camera.autoRotate;
            std::cout << "-> Tu dong xoay CAMERA toan canh: " << (camera.autoRotate ? "BAT" : "TAT") << std::endl;
        } else if (key == 'R' || key == 'r') {
            camera.reset();
            std::cout << "-> Camera da reset ve vi tri mac dinh!" << std::endl;
        } else if (key == VK_ESCAPE) {
            window.close();
        }
    };

    window.onMouse = [&](int x, int y, int button, bool isDown) {
        if (isDown) {
            camera.onMouseDown(x, y, button);
        } else {
            camera.onMouseUp(button);
        }
    };

    window.onMouseMove = [&](int x, int y) {
        camera.onMouseMove(x, y);
    };

    window.onMouseWheel = [&](int delta) {
        camera.onMouseWheel(delta);
    };

    if (!window.init()) {
        std::cerr << "Loi khoi tao Window / OpenGL Context!" << std::endl;
        return -1;
    }

    window.run();
    return 0;
}
