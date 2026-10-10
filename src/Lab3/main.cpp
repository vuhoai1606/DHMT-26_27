#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Window.h"
#include "Transform2D.h"

using namespace Lab3;

// Đa giác gốc và đa giác hiện tại
Polygon2D originalPoly;
Polygon2D currentPoly;
std::vector<Polygon2D> history; // Để Undo

// Tâm biến đổi (Pivot Point)
Point2D currentPivot(0, 0);
bool customPivotSet = false;

// Thông số đường thẳng đối xứng y = a*x + b
bool hasReflectionLine = false;
double reflA = 1.0;
double reflB = 0.0;

// Các trạng thái hiển thị & tự động
bool showPivotBBox = true;
bool showGrid = true;
bool autoKeepInWindow = false;
bool autoSpin = false;
double spinSpeedDegPerSec = 30.0; // Tốc độ xoay tự động (độ/giây)
int screenshotIndex = 1;

Point2D currentMouse;

void printMenu() {
    std::cout << "========================================================================\n";
    std::cout << "   LAB 03 - CAI DAT CAC PHEP BIEN DOI 2D (HOMOGENEOUS COORDINATES 3x3)   \n";
    std::cout << "========================================================================\n";
    std::cout << " [1] PHEP TINH TIEN (Translation):\n";
    std::cout << "     - [T]       : Nhap vector tinh tien (tx ty) tu console\n";
    std::cout << "     - Phim Mui ten: Tinh tien truc tiep tren man hinh (+-15 px)\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " [2] PHEP TI LE VOI TAM TI LE (x0, y0) (Scaling):\n";
    std::cout << "     - [Y] hoac [S]: Nhap he so ti le (sx sy [x0 y0]) tu console\n";
    std::cout << "     - Phim + / -  : Ti le nhanh quanh tam (x1.1 / x0.9)\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " [3] PHEP QUAY VOI TAM QUAY (x0, y0) (Rotation):\n";
    std::cout << "     - [U] hoac [R]: Nhap goc quay va tam (angleDeg [x0 y0]) tu console\n";
    std::cout << "     - Phim [ / ]  : Quay nhanh quanh tam (+-10 do)\n";
    std::cout << "     - [Space]     : Bat / Tat tu dong quay lien tuc (Auto-spin)\n";
    std::cout << "     - Phim ; / '  : Giam / Tang toc do quay (+-10 do/giay)\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " [4] PHEP DOI XUNG QUA DUONG THANG y = a*x + b (Reflection):\n";
    std::cout << "     - [I] hoac [D]: Nhap he so a, b cua duong thang tu console\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " THAO TAC CHUOT & TIEN ICH:\n";
    std::cout << "     - Chuot trai : Dat tam bien doi (Pivot Crosshair) tai vi tri click\n";
    std::cout << "     - [V]        : Bat / Tat hien thi Tam bien doi & Hop bao quanh (Bounding Box)\n";
    std::cout << "     - [F]        : Co dan da giac vua van cua so (Auto-fit to Window)\n";
    std::cout << "     - [A]        : Bat / Tat tu dong giu hinh trong cua so (Auto keep-in-window)\n";
    std::cout << "     - [Z]        : Hoan tac phep bien doi gan nhat (Undo)\n";
    std::cout << "     - [X]        : Dat lai (Reset) ve da giac goc ban dau\n";
    std::cout << "     - [O]        : Mo file da giac (polygon.txt)\n";
    std::cout << "     - [F5]       : Tao da giac Ngoi sao 5 canh (Star)\n";
    std::cout << "     - [F6]       : Tao da giac Mui ten (Arrow)\n";
    std::cout << "     - [F7]       : Tao da giac Ngoi nha (House)\n";
    std::cout << "     - [G]        : Bat / Tat luoi toa do Ox, Oy\n";
    std::cout << "     - [F9]       : Luu anh chup man hinh BMP (screenshot_lab3_###.bmp)\n";
    std::cout << "     - [ESC]      : Thoat chuong trinh\n";
    std::cout << "========================================================================\n";
}

void updatePivotIfNeeded() {
    if (!customPivotSet) {
        currentPivot = currentPoly.getCentroid();
    }
}

void updateTitle(Window* win) {
    std::string title = "LAB 03 - Bien Doi 2D | Tam: (" + 
                        std::to_string((int)currentPivot.x) + ", " + 
                        std::to_string((int)currentPivot.y) + ")";
    if (autoSpin) {
        title += " [AUTO-SPIN: " + std::to_string((int)spinSpeedDegPerSec) + " deg/s]";
    }
    if (autoKeepInWindow) {
        title += " [KEEP-IN-WINDOW: ON]";
    }
    title += " | Chuot: (" + std::to_string((int)currentMouse.x) + ", " + std::to_string((int)currentMouse.y) + ")";
    SetWindowTextA(win->getHWND(), title.c_str());
}

void drawGrid(int w, int h, int step = 40) {
    glColor3f(0.12f, 0.16f, 0.22f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int x = 0; x < w; x += step) {
        glVertex2i(x, 0); glVertex2i(x, h);
    }
    for (int y = 0; y < h; y += step) {
        glVertex2i(0, y); glVertex2i(w, y);
    }
    glEnd();

    // Hệ trục toạ độ chính
    glColor3f(0.25f, 0.35f, 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2i(0, 0); glVertex2i(w, 0);
    glVertex2i(0, 0); glVertex2i(0, h);
    glEnd();
}

void drawReflectionLine(double a, double b, int w, int h) {
    glColor3f(1.0f, 0.55f, 0.10f); // Màu cam nổi bật
    glLineWidth(2.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA); // Nét đứt

    // Tìm 2 giao điểm của đường thẳng y = a*x + b với biên màn hình [0, w] x [0, h]
    std::vector<Point2D> pts;
    // Cắt với x = 0
    double y_at_x0 = b;
    if (y_at_x0 >= 0 && y_at_x0 <= h) pts.push_back(Point2D(0, y_at_x0));

    // Cắt với x = w
    double y_at_xw = a * w + b;
    if (y_at_xw >= 0 && y_at_xw <= h) pts.push_back(Point2D(w, y_at_xw));

    // Cắt với y = 0
    if (std::abs(a) > 1e-6) {
        double x_at_y0 = -b / a;
        if (x_at_y0 >= 0 && x_at_y0 <= w) pts.push_back(Point2D(x_at_y0, 0));
        // Cắt với y = h
        double x_at_yh = (h - b) / a;
        if (x_at_yh >= 0 && x_at_yh <= w) pts.push_back(Point2D(x_at_yh, h));
    }

    if (pts.size() >= 2) {
        glBegin(GL_LINES);
        glVertex2d(pts[0].x, pts[0].y);
        glVertex2d(pts[1].x, pts[1].y);
        glEnd();
    } else {
        // Vẽ bao quát đường thẳng từ x = -100 đến x = w + 100
        glBegin(GL_LINES);
        glVertex2d(-100.0, a * (-100.0) + b);
        glVertex2d(w + 100.0, a * (w + 100.0) + b);
        glEnd();
    }

    glDisable(GL_LINE_STIPPLE);
}

void applyTransformationWithLog(const Matrix3x3& mat, const std::string& transformName) {
    history.push_back(currentPoly);
    mat.print("MA TRAN BIEN DOI 3x3: " + transformName);
    currentPoly.applyTransform(mat);
    updatePivotIfNeeded();

    if (autoKeepInWindow) {
        double minX, minY, maxX, maxY;
        currentPoly.getBoundingBox(minX, minY, maxX, maxY);
        if (minX < 20 || minY < 20 || maxX > 880 || maxY > 680) {
            std::cout << "[Auto Keep-in-Window] Da tu dong can chinh da giac vao trong man hinh.\n";
            currentPoly.fitToWindow(900, 700, 80);
            updatePivotIfNeeded();
        }
    }
}

int main() {
    printMenu();

    Window window("LAB 03 - Bien Doi 2D (OpenGL C++)", 900, 700);

    // Tự động nạp polygon.txt hoặc tạo hình mặc định
    if (loadPolygonFromFile("polygon.txt", currentPoly) ||
        loadPolygonFromFile("../polygon.txt", currentPoly) ||
        loadPolygonFromFile("../../polygon.txt", currentPoly)) {
        std::cout << "[Thanh cong] Da tu dong doc file 'polygon.txt' (" 
                  << currentPoly.vertices.size() << " dinh).\n";
        // Căn chỉnh vị trí thích hợp ban đầu
        currentPoly.fitToWindow(900, 700, 150);
    } else {
        currentPoly = createStarPolygon(450, 350, 200, 85);
        std::cout << "[Thong tin] Chua tim thay polygon.txt, tao da giac Ngoi sao mau.\n";
    }

    originalPoly = currentPoly;
    currentPivot = currentPoly.getCentroid();

    window.onInit = [&]() {
        glClearColor(0.07f, 0.09f, 0.11f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        updateTitle(&window);
    };

    window.onResize = [&](int width, int height) {
        if (height == 0) height = 1;
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, width, 0.0, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    };

    window.onUpdate = [&](float deltaTime) {
        if (autoSpin) {
            double angle = spinSpeedDegPerSec * deltaTime;
            updatePivotIfNeeded();
            Matrix3x3 R = Matrix3x3::rotation(angle, currentPivot.x, currentPivot.y);
            currentPoly.applyTransform(R);
        }
    };

    window.onRender = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        if (showGrid) {
            drawGrid(window.getWidth(), window.getHeight(), 30);
        }

        // 1. Vẽ đường thẳng đối xứng nếu có
        if (hasReflectionLine) {
            drawReflectionLine(reflA, reflB, window.getWidth(), window.getHeight());
        }

        // 2. Vẽ đa giác gốc ban đầu (nét đứt / màu xám nhạt để đối chiếu trước - sau biến đổi)
        if (!originalPoly.vertices.empty()) {
            glColor3f(0.35f, 0.40f, 0.48f);
            glLineWidth(1.5f);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0xAAAA);
            glBegin(GL_LINE_LOOP);
            for (const auto& v : originalPoly.vertices) {
                glVertex2d(v.x, v.y);
            }
            glEnd();
            glDisable(GL_LINE_STIPPLE);
        }

        // 3. Vẽ đa giác sau biến đổi (màu xanh ngọc bích rực rỡ)
        if (!currentPoly.vertices.empty()) {
            glColor3f(0.15f, 0.90f, 0.95f);
            glLineWidth(2.5f);
            glBegin(GL_LINE_LOOP);
            for (const auto& v : currentPoly.vertices) {
                glVertex2d(v.x, v.y);
            }
            glEnd();

            // Đánh dấu các đỉnh của đa giác
            glPointSize(7.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_POINTS);
            for (const auto& v : currentPoly.vertices) {
                glVertex2d(v.x, v.y);
            }
            glEnd();
        }

        // 4. Vẽ Hộp bao quanh (Bounding Box) và Tâm biến đổi (Pivot Point)
        if (showPivotBBox && !currentPoly.vertices.empty()) {
            double minX, minY, maxX, maxY;
            currentPoly.getBoundingBox(minX, minY, maxX, maxY);

            // Bounding box màu vàng nhạt nét đứt
            glColor3f(0.95f, 0.95f, 0.40f);
            glLineWidth(1.2f);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0xCCCC);
            glBegin(GL_LINE_LOOP);
            glVertex2d(minX, minY);
            glVertex2d(maxX, minY);
            glVertex2d(maxX, maxY);
            glVertex2d(minX, maxY);
            glEnd();
            glDisable(GL_LINE_STIPPLE);

            // Dấu chữ thập tâm biến đổi (Pivot Crosshair) màu vàng tươi
            updatePivotIfNeeded();
            glColor3f(1.0f, 0.95f, 0.10f);
            glLineWidth(2.5f);
            glBegin(GL_LINES);
            glVertex2d(currentPivot.x - 12.0, currentPivot.y);
            glVertex2d(currentPivot.x + 12.0, currentPivot.y);
            glVertex2d(currentPivot.x, currentPivot.y - 12.0);
            glVertex2d(currentPivot.x, currentPivot.y + 12.0);
            glEnd();

            glPointSize(5.0f);
            glBegin(GL_POINTS);
            glVertex2d(currentPivot.x, currentPivot.y);
            glEnd();
        }

        // 5. Đánh dấu con trỏ chuột
        glPointSize(5.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        glVertex2d(currentMouse.x, currentMouse.y);
        glEnd();
    };

    window.onMouseMove = [&](int worldX, int worldY) {
        currentMouse.x = worldX;
        currentMouse.y = worldY;
        updateTitle(&window);
    };

    window.onMouse = [&](int worldX, int worldY, int button, bool isDown) {
        if (!isDown) return;

        // Chuột trái: Đặt tâm biến đổi (Pivot Point) tại vị trí click
        if (button == 0) {
            currentPivot = Point2D(worldX, worldY);
            customPivotSet = true;
            std::cout << "[Pivot] Da dat tam bien doi (x0, y0) tai: (" 
                      << worldX << ", " << worldY << ")\n";
            updateTitle(&window);
        }
    };

    window.onKey = [&](int key, bool isDown) {
        if (!isDown) return;

        // ---------------------------------------------------------------------
        // 1. Phép Tịnh tiến (Translation)
        // ---------------------------------------------------------------------
        if (key == 'T' || key == 't') {
            std::cout << "\n=== PHEP TINH TIEN (TRANSLATION) ===\n";
            std::cout << "Nhap vector tinh tien (tx ty) [Vi du: 50 30]: ";
            std::string line;
            std::getline(std::cin, line);
            double tx = 50.0, ty = 30.0;
            if (!line.empty()) {
                std::stringstream ss(line);
                ss >> tx >> ty;
            }
            std::cout << "-> Thuc hien tinh tien: tx = " << tx << ", ty = " << ty << "\n";
            Matrix3x3 T = Matrix3x3::translation(tx, ty);
            applyTransformationWithLog(T, "Tinh Tien (tx=" + std::to_string((int)tx) + ", ty=" + std::to_string((int)ty) + ")");
            updateTitle(&window);
        } else if (key == VK_LEFT) {
            Matrix3x3 T = Matrix3x3::translation(-15.0, 0.0);
            applyTransformationWithLog(T, "Tinh Tien Sang Trai (-15 px)");
            updateTitle(&window);
        } else if (key == VK_RIGHT) {
            Matrix3x3 T = Matrix3x3::translation(15.0, 0.0);
            applyTransformationWithLog(T, "Tinh Tien Sang Phai (+15 px)");
            updateTitle(&window);
        } else if (key == VK_UP) {
            Matrix3x3 T = Matrix3x3::translation(0.0, 15.0);
            applyTransformationWithLog(T, "Tinh Tien Len Tren (+15 px)");
            updateTitle(&window);
        } else if (key == VK_DOWN) {
            Matrix3x3 T = Matrix3x3::translation(0.0, -15.0);
            applyTransformationWithLog(T, "Tinh Tien Xuong Duoi (-15 px)");
            updateTitle(&window);
        }

        // ---------------------------------------------------------------------
        // 2. Phép Tỉ lệ với tâm tỉ lệ (x0, y0) (Scaling)
        // ---------------------------------------------------------------------
        else if (key == 'Y' || key == 'y' || key == 'S' || key == 's') {
            updatePivotIfNeeded();
            std::cout << "\n=== PHEP TI LE VOI TAM (SCALING) ===\n";
            std::cout << "Tam hien tai: (" << currentPivot.x << ", " << currentPivot.y << ")\n";
            std::cout << "Nhap (sx sy [x0 y0]) [Vi du: 1.2 0.8]: ";
            std::string line;
            std::getline(std::cin, line);
            double sx = 1.2, sy = 1.2;
            double cx = currentPivot.x, cy = currentPivot.y;
            if (!line.empty()) {
                std::stringstream ss(line);
                ss >> sx >> sy;
                if (ss >> cx >> cy) {
                    currentPivot = Point2D(cx, cy);
                    customPivotSet = true;
                }
            }
            std::cout << "-> Thuc hien ti le: sx=" << sx << ", sy=" << sy 
                      << " quanh tam (" << cx << ", " << cy << ")\n";
            Matrix3x3 S = Matrix3x3::scaling(sx, sy, cx, cy);
            applyTransformationWithLog(S, "Ti Le (sx=" + std::to_string(sx) + ", sy=" + std::to_string(sy) + ")");
            updateTitle(&window);
        } else if (key == VK_OEM_PLUS || key == 187 || key == VK_ADD) {
            updatePivotIfNeeded();
            Matrix3x3 S = Matrix3x3::scaling(1.1, 1.1, currentPivot.x, currentPivot.y);
            applyTransformationWithLog(S, "Phong to x1.1 quanh tam");
            updateTitle(&window);
        } else if (key == VK_OEM_MINUS || key == 189 || key == VK_SUBTRACT) {
            updatePivotIfNeeded();
            Matrix3x3 S = Matrix3x3::scaling(0.9, 0.9, currentPivot.x, currentPivot.y);
            applyTransformationWithLog(S, "Thu nho x0.9 quanh tam");
            updateTitle(&window);
        }

        // ---------------------------------------------------------------------
        // 3. Phép Quay với tâm quay (x0, y0) (Rotation)
        // ---------------------------------------------------------------------
        else if (key == 'U' || key == 'u' || key == 'R' || key == 'r') {
            updatePivotIfNeeded();
            std::cout << "\n=== PHEP QUAY VOI TAM (ROTATION) ===\n";
            std::cout << "Tam quay hien tai: (" << currentPivot.x << ", " << currentPivot.y << ")\n";
            std::cout << "Nhap (goc_do [x0 y0]) [Vi du: 30 hoac 45 450 350]: ";
            std::string line;
            std::getline(std::cin, line);
            double angleDeg = 30.0;
            double cx = currentPivot.x, cy = currentPivot.y;
            if (!line.empty()) {
                std::stringstream ss(line);
                ss >> angleDeg;
                if (ss >> cx >> cy) {
                    currentPivot = Point2D(cx, cy);
                    customPivotSet = true;
                }
            }
            std::cout << "-> Thuc hien quay goc: " << angleDeg 
                      << " do quanh tam (" << cx << ", " << cy << ")\n";
            Matrix3x3 R = Matrix3x3::rotation(angleDeg, cx, cy);
            applyTransformationWithLog(R, "Quay " + std::to_string((int)angleDeg) + " do");
            updateTitle(&window);
        } else if (key == VK_OEM_4 || key == '[') {
            updatePivotIfNeeded();
            Matrix3x3 R = Matrix3x3::rotation(10.0, currentPivot.x, currentPivot.y);
            applyTransformationWithLog(R, "Quay +10 do");
            updateTitle(&window);
        } else if (key == VK_OEM_6 || key == ']') {
            updatePivotIfNeeded();
            Matrix3x3 R = Matrix3x3::rotation(-10.0, currentPivot.x, currentPivot.y);
            applyTransformationWithLog(R, "Quay -10 do");
            updateTitle(&window);
        }

        // ---------------------------------------------------------------------
        // 4. Phép Đối xứng qua đường thẳng y = a*x + b (Reflection)
        // ---------------------------------------------------------------------
        else if (key == 'I' || key == 'i' || key == 'D' || key == 'd') {
            std::cout << "\n=== PHEP DOI XUNG QUA DUONG THANG y = a*x + b ===\n";
            std::cout << "Nhap he so a va b [Vi du: 1 50]: ";
            std::string line;
            std::getline(std::cin, line);
            double a = 1.0, b = 0.0;
            if (!line.empty()) {
                std::stringstream ss(line);
                ss >> a >> b;
            }
            reflA = a;
            reflB = b;
            hasReflectionLine = true;
            std::cout << "-> Thuc hien doi xung qua duong thang: y = " << a << "*x + " << b << "\n";
            Matrix3x3 M = Matrix3x3::reflection(a, b);
            applyTransformationWithLog(M, "Doi Xung qua y=" + std::to_string(a) + "*x + " + std::to_string(b));
            updateTitle(&window);
        }

        // ---------------------------------------------------------------------
        // Tiện ích & Tự động
        // ---------------------------------------------------------------------
        else if (key == VK_SPACE) {
            autoSpin = !autoSpin;
            std::cout << "--> Auto-spin: " << (autoSpin ? "BAT" : "TAT") 
                      << " (Toc do: " << spinSpeedDegPerSec << " deg/s)\n";
            updateTitle(&window);
        } else if (key == VK_OEM_1 || key == ';') {
            spinSpeedDegPerSec = std::max(10.0, spinSpeedDegPerSec - 10.0);
            std::cout << "--> Toc do Auto-spin giam xuong: " << spinSpeedDegPerSec << " deg/s\n";
            updateTitle(&window);
        } else if (key == VK_OEM_7 || key == '\'') {
            spinSpeedDegPerSec += 10.0;
            std::cout << "--> Toc do Auto-spin tang len: " << spinSpeedDegPerSec << " deg/s\n";
            updateTitle(&window);
        } else if (key == 'V' || key == 'v') {
            showPivotBBox = !showPivotBBox;
            std::cout << "--> Hien thi Tam Pivot & Bounding Box: " << (showPivotBBox ? "BAT" : "TAT") << "\n";
        } else if (key == 'A' || key == 'a') {
            autoKeepInWindow = !autoKeepInWindow;
            std::cout << "--> Auto Keep-in-Window: " << (autoKeepInWindow ? "BAT" : "TAT") << "\n";
            updateTitle(&window);
        } else if (key == 'F' || key == 'f') {
            currentPoly.fitToWindow(window.getWidth(), window.getHeight(), 80);
            updatePivotIfNeeded();
            std::cout << "--> Da can chinh da giac vua van cua so (Auto-fit).\n";
            updateTitle(&window);
        } else if (key == 'Z' || key == 'z') {
            if (!history.empty()) {
                currentPoly = history.back();
                history.pop_back();
                updatePivotIfNeeded();
                std::cout << "--> Da hoan tac (Undo). Con lai: " << history.size() << " buoc trong lich su.\n";
                updateTitle(&window);
            } else {
                std::cout << "[Thong bao] Lich su rong, khong the Undo!\n";
            }
        } else if (key == 'X' || key == 'x') {
            history.push_back(currentPoly);
            currentPoly = originalPoly;
            customPivotSet = false;
            hasReflectionLine = false;
            updatePivotIfNeeded();
            std::cout << "--> Da dat lai (Reset) ve da giac ban dau!\n";
            updateTitle(&window);
        } else if (key == 'O' || key == 'o') {
            std::cout << "Nhap duong dan file da giac (.txt) [Mac dinh: polygon.txt]: ";
            std::string filename;
            std::getline(std::cin, filename);
            if (filename.empty()) filename = "polygon.txt";
            if (loadPolygonFromFile(filename, currentPoly) ||
                loadPolygonFromFile("../" + filename, currentPoly) ||
                loadPolygonFromFile("../../" + filename, currentPoly)) {
                std::cout << "[Thanh cong] Da mo file " << filename 
                          << " (" << currentPoly.vertices.size() << " dinh)\n";
                currentPoly.fitToWindow(window.getWidth(), window.getHeight(), 120);
                originalPoly = currentPoly;
                customPivotSet = false;
                hasReflectionLine = false;
                history.clear();
                updatePivotIfNeeded();
            } else {
                std::cout << "[Loi] Khong the mo file: " << filename << "\n";
            }
            updateTitle(&window);
        } else if (key == VK_F5) {
            history.push_back(currentPoly);
            currentPoly = createStarPolygon(450, 350, 200, 85);
            originalPoly = currentPoly;
            customPivotSet = false;
            hasReflectionLine = false;
            updatePivotIfNeeded();
            std::cout << "--> Da tao da giac Ngoi sao 5 canh (F5)\n";
            updateTitle(&window);
        } else if (key == VK_F6) {
            history.push_back(currentPoly);
            currentPoly = createArrowPolygon(450, 350, 360, 130);
            originalPoly = currentPoly;
            customPivotSet = false;
            hasReflectionLine = false;
            updatePivotIfNeeded();
            std::cout << "--> Da tao da giac Mui ten (F6)\n";
            updateTitle(&window);
        } else if (key == VK_F7) {
            history.push_back(currentPoly);
            currentPoly = createHousePolygon(450, 350, 240);
            originalPoly = currentPoly;
            customPivotSet = false;
            hasReflectionLine = false;
            updatePivotIfNeeded();
            std::cout << "--> Da tao da giac Ngoi nha (F7)\n";
            updateTitle(&window);
        } else if (key == 'G' || key == 'g') {
            showGrid = !showGrid;
            std::cout << "--> Luoi toa do: " << (showGrid ? "BAT" : "TAT") << "\n";
        } else if (key == VK_F9) {
            char fname[64];
            snprintf(fname, sizeof(fname), "screenshot_lab3_%03d.bmp", screenshotIndex++);
            if (saveScreenshotBMP(fname, window.getWidth(), window.getHeight())) {
                std::cout << "[F9] Da xuat anh chup man hinh: " << fname << "\n";
            } else {
                std::cout << "[Loi] Khong the ghi file anh: " << fname << "\n";
            }
        } else if (key == VK_ESCAPE) {
            window.close();
        }
    };

    if (!window.init()) {
        std::cerr << "Loi khoi tao Window / OpenGL Context cho Lab 3!\n";
        return -1;
    }

    window.run();
    return 0;
}
