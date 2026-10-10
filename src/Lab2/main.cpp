#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include "Window.h"
#include "Clipping.h"
#include "Filling.h"

using namespace Lab2;

enum class AppMode {
    CLIPPING = 1,
    FILLING  = 2
};

enum class ClipAlgo {
    COHEN_SUTHERLAND,
    MIDPOINT_BINARY
};

enum class FillAlgo {
    BOUNDARY_REC,
    BOUNDARY_STACK,
    SCANLINE
};

// Trạng thái chương trình Lab 2
AppMode currentMode = AppMode::CLIPPING;
ClipAlgo currentClipAlgo = ClipAlgo::COHEN_SUTHERLAND;
FillAlgo currentFillAlgo = FillAlgo::BOUNDARY_STACK;

// Dữ liệu xén hình (Clipping)
ClipRect clipWindow(150, 120, 650, 500);
std::vector<Line2D> linesHistory;
bool isDrawingLine = false;
Point2D tempStart;
Point2D currentMouse;

// Dữ liệu tô màu (Filling)
Polygon2D currentPoly;
bool polyLoaded = false;
Point2D seedPoint(-1, -1);
bool isFilled = false;
Canvas fillCanvas(900, 700);
ColorRGB colBackground(18, 22, 28);
ColorRGB colBoundary(255, 255, 255);
ColorRGB colFill(230, 45, 45); // Màu đỏ rực rỡ

bool showGrid = true;
int screenshotIndex = 1;

void printMenu() {
    std::cout << "========================================================================\n";
    std::cout << "   LAB 02 - THUAT TOAN XEN HINH (LINE CLIPPING) & TO MAU (POLYGON FILL)  \n";
    std::cout << "========================================================================\n";
    std::cout << " [1] Che do XEN HINH (Line Clipping)\n";
    std::cout << "     - [C] Thuat toan Cohen-Sutherland\n";
    std::cout << "     - [M] Thuat toan Chia nhi phan (Midpoint Subdivision)\n";
    std::cout << "     - Chuot trai lan 1: Chon diem dau | Di chuot: Xem truoc (Preview)\n";
    std::cout << "     - Chuot trai lan 2: Chot doan thang (Tu dong xen theo thuat toan)\n";
    std::cout << "     - Phim Mui ten    : Di chuyen cua so xen (Clipping Window)\n";
    std::cout << "     - Phim [ / ]      : Thu nho / Mo rong chieu rong cua so\n";
    std::cout << "     - Phim , / .      : Thu nho / Mo rong chieu cao cua so\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " [2] Che do TO MAU DA GIAC (Polygon Filling)\n";
    std::cout << "     - [R] To theo duong bien DE QUY (Boundary Fill - Recursive)\n";
    std::cout << "     - [K] To theo duong bien KHU DE QUY (Boundary Fill - Stack)\n";
    std::cout << "     - [S] To theo DONG QUET (Scanline Polygon Fill)\n";
    std::cout << "     - Chuot trai : Chon diem hat giong (Seed Point) de to duong bien\n";
    std::cout << "     - Chuot phai : Kich hoat to mau (Trigger Fill Pipeline)\n";
    std::cout << "     - [O]        : Mo file da giac (polygon.txt)\n";
    std::cout << "     - [F5]       : Tao da giac Ngoi sao 5 canh (Star)\n";
    std::cout << "     - [F6]       : Tao da giac Mui ten (Arrow)\n";
    std::cout << "     - [F7]       : Tao da giac Lom (Concave)\n";
    std::cout << "------------------------------------------------------------------------\n";
    std::cout << " TIEN ICH CHUNG:\n";
    std::cout << "     - [G]        : Bat / Tat luoi toa do\n";
    std::cout << "     - [X]        : Xoa doan thang / Xoa ket qua to mau\n";
    std::cout << "     - [F9]       : Luu anh chup man hinh BMP (screenshot_lab2_###.bmp)\n";
    std::cout << "     - [ESC]      : Thoat chuong trinh\n";
    std::cout << "========================================================================\n";
}

void updateTitle(Window* win) {
    std::string title = "LAB 02 | ";
    if (currentMode == AppMode::CLIPPING) {
        title += "[CHE DO XEN HINH] ";
        title += (currentClipAlgo == ClipAlgo::COHEN_SUTHERLAND) ? "(Cohen-Sutherland)" : "(Chia nhi phan - Midpoint)";
        title += " | So doan: " + std::to_string(linesHistory.size());
        if (isDrawingLine) title += " [DANG VE... Click 2 de chot]";
    } else {
        title += "[CHE DO TO MAU] ";
        if (currentFillAlgo == FillAlgo::BOUNDARY_REC) title += "(Bien De quy [R])";
        else if (currentFillAlgo == FillAlgo::BOUNDARY_STACK) title += "(Bien Stack [K])";
        else title += "(Dong quet [S])";

        title += " | Dinh: " + std::to_string(currentPoly.vertices.size());
        if (seedPoint.x >= 0) {
            title += " | Seed: (" + std::to_string((int)seedPoint.x) + "," + std::to_string((int)seedPoint.y) + ")";
        } else {
            title += " | Seed: [Centroid]";
        }
        title += " | Chuot phai: To mau";
    }
    title += " | Chuot: (" + std::to_string((int)currentMouse.x) + ", " + std::to_string((int)currentMouse.y) + ")";
    SetWindowTextA(win->getHWND(), title.c_str());
}

void drawGrid(int w, int h, int step = 40) {
    glColor3f(0.12f, 0.16f, 0.22f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int x = 0; x < w; x += step) {
        glVertex2i(x, 0);
        glVertex2i(x, h);
    }
    for (int y = 0; y < h; y += step) {
        glVertex2i(0, y);
        glVertex2i(w, y);
    }
    glEnd();

    // Hai trục chính
    glColor3f(0.25f, 0.35f, 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2i(0, 0); glVertex2i(w, 0);
    glVertex2i(0, 0); glVertex2i(0, h);
    glEnd();
}

void triggerFilling() {
    if (currentPoly.vertices.size() < 3) {
        std::cout << "[Canh bao] Da giac chua du 3 dinh de to mau!\n";
        return;
    }

    fillCanvas.clear(colBackground);
    fillCanvas.drawPolygonBoundary(currentPoly, colBoundary);

    Point2D actualSeed = seedPoint;
    if (actualSeed.x < 0 || actualSeed.y < 0) {
        actualSeed = currentPoly.getCentroid();
    }

    std::cout << "\n--> Dang tien hanh to mau da giac...\n";
    if (currentFillAlgo == FillAlgo::BOUNDARY_REC) {
        std::cout << "   [Thuat toan] To theo duong bien DE QUY tai diem Seed: (" 
                  << (int)actualSeed.x << ", " << (int)actualSeed.y << ")\n";
        int callCount = 0;
        boundaryFillRecursive(fillCanvas, (int)std::round(actualSeed.x), (int)std::round(actualSeed.y), 
                              colFill, colBoundary, callCount);
        std::cout << "   [Hoan tat] So buoc goi de quy: " << callCount << "\n";
    } else if (currentFillAlgo == FillAlgo::BOUNDARY_STACK) {
        std::cout << "   [Thuat toan] To theo duong bien KHU DE QUY (STACK) tai diem Seed: (" 
                  << (int)actualSeed.x << ", " << (int)actualSeed.y << ")\n";
        boundaryFillStack(fillCanvas, (int)std::round(actualSeed.x), (int)std::round(actualSeed.y), 
                          colFill, colBoundary);
        std::cout << "   [Hoan tat] Da to mau thanh cong bang Stack!\n";
    } else if (currentFillAlgo == FillAlgo::SCANLINE) {
        std::cout << "   [Thuat toan] To theo DONG QUET (Scanline Polygon Fill). Khong can Seed Point.\n";
        scanlinePolygonFill(fillCanvas, currentPoly, colFill);
        std::cout << "   [Hoan tat] Da to mau thanh cong bang Dong quet!\n";
    }

    // Vẽ lại viền cho sắc nét
    fillCanvas.drawPolygonBoundary(currentPoly, colBoundary);
    isFilled = true;
}

int main() {
    printMenu();

    Window window("LAB 02 - Xen Hinh & To Mau (OpenGL)", 900, 700);

    // Tự động tìm nạp polygon.txt nếu có
    if (loadPolygonFromFile("polygon.txt", currentPoly) ||
        loadPolygonFromFile("../polygon.txt", currentPoly) ||
        loadPolygonFromFile("../../polygon.txt", currentPoly)) {
        polyLoaded = true;
        std::cout << "[Thanh cong] Da tu dong doc file 'polygon.txt' voi " 
                  << currentPoly.vertices.size() << " dinh.\n";
    } else {
        // Sinh đa giác ngôi sao mặc định
        currentPoly = createStarPolygon(450, 350, 220, 90);
        polyLoaded = true;
        std::cout << "[Thong tin] Chua tim thay polygon.txt, tao da giac Ngoi sao mau (450, 350).\n";
    }

    window.onInit = [&]() {
        glClearColor(0.07f, 0.09f, 0.11f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        fillCanvas.resize(window.getWidth(), window.getHeight());
        fillCanvas.clear(colBackground);
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

        fillCanvas.resize(width, height);
        if (isFilled) {
            triggerFilling();
        }
    };

    window.onRender = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        if (showGrid) {
            drawGrid(window.getWidth(), window.getHeight(), 30);
        }

        // =====================================================================
        // RENDER CHẾ ĐỘ 1: XÉN ĐOẠN THẲNG (CLIPPING)
        // =====================================================================
        if (currentMode == AppMode::CLIPPING) {
            // 1. Vẽ khung cửa sổ xén (Màu vàng rực rỡ)
            glColor3f(1.0f, 0.95f, 0.20f);
            glLineWidth(2.5f);
            glBegin(GL_LINE_LOOP);
            glVertex2d(clipWindow.xmin, clipWindow.ymin);
            glVertex2d(clipWindow.xmax, clipWindow.ymin);
            glVertex2d(clipWindow.xmax, clipWindow.ymax);
            glVertex2d(clipWindow.xmin, clipWindow.ymax);
            glEnd();

            // Tô mờ nhẹ bên trong khung xén để dễ nhận biết vùng hiển thị
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 0.95f, 0.20f, 0.04f);
            glBegin(GL_QUADS);
            glVertex2d(clipWindow.xmin, clipWindow.ymin);
            glVertex2d(clipWindow.xmax, clipWindow.ymin);
            glVertex2d(clipWindow.xmax, clipWindow.ymax);
            glVertex2d(clipWindow.xmin, clipWindow.ymax);
            glEnd();
            glDisable(GL_BLEND);

            // 2. Vẽ tất cả các đoạn thẳng đã chốt
            for (const auto& line : linesHistory) {
                // Vẽ đoạn thẳng gốc nguyên bản (màu xám / trắng mờ)
                glColor3f(0.40f, 0.45f, 0.50f);
                glLineWidth(1.5f);
                glBegin(GL_LINES);
                glVertex2d(line.a.x, line.a.y);
                glVertex2d(line.b.x, line.b.y);
                glEnd();

                // Thực hiện xén theo thuật toán hiện tại
                Line2D clipped;
                bool visible = false;
                if (currentClipAlgo == ClipAlgo::COHEN_SUTHERLAND) {
                    visible = cohenSutherlandClip(line, clipWindow, clipped);
                } else {
                    visible = midpointSubdivisionClip(line, clipWindow, clipped);
                }

                // Nếu có phần nằm trong cửa sổ, vẽ phần xén nổi bật màu đỏ rực rỡ
                if (visible) {
                    glColor3f(1.0f, 0.20f, 0.20f);
                    glLineWidth(3.0f);
                    glBegin(GL_LINES);
                    glVertex2d(clipped.a.x, clipped.a.y);
                    glVertex2d(clipped.b.x, clipped.b.y);
                    glEnd();

                    // Đánh dấu 2 điểm đầu mút xén
                    glPointSize(6.0f);
                    glColor3f(0.20f, 1.0f, 0.50f);
                    glBegin(GL_POINTS);
                    glVertex2d(clipped.a.x, clipped.a.y);
                    glVertex2d(clipped.b.x, clipped.b.y);
                    glEnd();
                }
            }

            // 3. Vẽ đoạn thẳng đang xem trước (Preview) theo con trỏ chuột
            if (isDrawingLine) {
                // Đường preview màu xanh lục sáng
                glColor3f(0.20f, 1.0f, 0.40f);
                glLineWidth(1.8f);
                glBegin(GL_LINES);
                glVertex2d(tempStart.x, tempStart.y);
                glVertex2d(currentMouse.x, currentMouse.y);
                glEnd();

                // Thử xén ngay trực tiếp trong khi preview!
                Line2D testLine(tempStart, currentMouse);
                Line2D previewClipped;
                bool testVis = (currentClipAlgo == ClipAlgo::COHEN_SUTHERLAND)
                                   ? cohenSutherlandClip(testLine, clipWindow, previewClipped)
                                   : midpointSubdivisionClip(testLine, clipWindow, previewClipped);
                if (testVis) {
                    glColor3f(1.0f, 0.3f, 0.3f);
                    glLineWidth(3.5f);
                    glBegin(GL_LINES);
                    glVertex2d(previewClipped.a.x, previewClipped.a.y);
                    glVertex2d(previewClipped.b.x, previewClipped.b.y);
                    glEnd();
                }

                // Điểm mốc click 1
                glPointSize(8.0f);
                glColor3f(1.0f, 0.3f, 0.3f);
                glBegin(GL_POINTS);
                glVertex2d(tempStart.x, tempStart.y);
                glEnd();
            }
        }
        // =====================================================================
        // RENDER CHẾ ĐỘ 2: TÔ MÀU ĐA GIÁC (FILLING)
        // =====================================================================
        else {
            if (isFilled) {
                // Hiển thị kết quả raster từ CPU canvas
                fillCanvas.renderGL();
            } else {
                // Nếu chưa nhấn tô, vẽ khung dây viền đa giác bằng OpenGL
                glColor3f(0.30f, 0.85f, 1.00f);
                glLineWidth(2.5f);
                glBegin(GL_LINE_LOOP);
                for (const auto& v : currentPoly.vertices) {
                    glVertex2d(v.x, v.y);
                }
                glEnd();

                // Đánh dấu các đỉnh của đa giác
                glPointSize(6.0f);
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_POINTS);
                for (const auto& v : currentPoly.vertices) {
                    glVertex2d(v.x, v.y);
                }
                glEnd();
            }

            // Hiển thị điểm hạt giống (Seed Point)
            Point2D pt = (seedPoint.x >= 0) ? seedPoint : currentPoly.getCentroid();
            glPointSize(10.0f);
            glColor3f(1.0f, 0.90f, 0.10f); // Điểm hạt giống màu vàng
            glBegin(GL_POINTS);
            glVertex2d(pt.x, pt.y);
            glEnd();

            // Vẽ dấu chữ thập nhỏ tại Seed Point
            glColor3f(1.0f, 0.90f, 0.10f);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            glVertex2d(pt.x - 7, pt.y); glVertex2d(pt.x + 7, pt.y);
            glVertex2d(pt.x, pt.y - 7); glVertex2d(pt.x, pt.y + 7);
            glEnd();
        }

        // Vẽ con trỏ chuột
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

        // Chuột trái (button 0)
        if (button == 0) {
            if (currentMode == AppMode::CLIPPING) {
                if (!isDrawingLine) {
                    tempStart.x = worldX;
                    tempStart.y = worldY;
                    isDrawingLine = true;
                    std::cout << "[Line] Click 1: Bat dau tai (" << worldX << ", " << worldY << ")\n";
                } else {
                    Line2D newLine(tempStart, Point2D(worldX, worldY));
                    linesHistory.push_back(newLine);
                    isDrawingLine = false;
                    std::cout << "[Line] Click 2: Chot doan (" << tempStart.x << ", " << tempStart.y 
                              << ") -> (" << worldX << ", " << worldY << ")\n";
                }
            } else {
                // Polygon Filling: Đặt điểm hạt giống Seed Point
                seedPoint = Point2D(worldX, worldY);
                std::cout << "[Seed] Da chon Seed Point tai: (" << worldX << ", " << worldY << ")\n";
                if (isFilled) {
                    // Nếu đang hiển thị kết quả cũ, tô lại với seed point mới
                    triggerFilling();
                }
            }
            updateTitle(&window);
        }
        // Chuột phải (button 1)
        else if (button == 1) {
            if (currentMode == AppMode::CLIPPING) {
                if (isDrawingLine) {
                    isDrawingLine = false;
                    std::cout << "[Line] Da huy thao tac ve doan thang.\n";
                    updateTitle(&window);
                }
            } else {
                // Polygon Filling: Kích hoạt tô màu
                triggerFilling();
                updateTitle(&window);
            }
        }
    };

    window.onKey = [&](int key, bool isDown) {
        if (!isDown) return;

        // Chuyển chế độ [1] và [2]
        if (key == '1') {
            currentMode = AppMode::CLIPPING;
            std::cout << ">>> Chuyen sang [CHE DO 1: XEN DOAN THANG]\n";
            updateTitle(&window);
        } else if (key == '2') {
            currentMode = AppMode::FILLING;
            std::cout << ">>> Chuyen sang [CHE DO 2: TO MAU DA GIAC]\n";
            updateTitle(&window);
        }
        // Chọn thuật toán xén: C (Cohen) hoặc M (Midpoint)
        else if (key == 'C' || key == 'c') {
            currentClipAlgo = ClipAlgo::COHEN_SUTHERLAND;
            std::cout << "--> Thuat toan xen: [COHEN-SUTHERLAND]\n";
            updateTitle(&window);
        } else if (key == 'M' || key == 'm') {
            currentClipAlgo = ClipAlgo::MIDPOINT_BINARY;
            std::cout << "--> Thuat toan xen: [CHIA NHI PHAN - MIDPOINT SUBDIVISION]\n";
            updateTitle(&window);
        }
        // Chọn thuật toán tô màu: R, K, S
        else if (key == 'R' || key == 'r') {
            currentFillAlgo = FillAlgo::BOUNDARY_REC;
            std::cout << "--> Thuat toan to: [DUONG BIEN DE QUY (BOUNDARY RECURSIVE)]\n";
            updateTitle(&window);
            if (currentMode == AppMode::FILLING && isFilled) triggerFilling();
        } else if (key == 'K' || key == 'k') {
            currentFillAlgo = FillAlgo::BOUNDARY_STACK;
            std::cout << "--> Thuat toan to: [DUONG BIEN KHU DE QUY (BOUNDARY STACK)]\n";
            updateTitle(&window);
            if (currentMode == AppMode::FILLING && isFilled) triggerFilling();
        } else if (key == 'S' || key == 's') {
            currentFillAlgo = FillAlgo::SCANLINE;
            std::cout << "--> Thuat toan to: [DONG QUET (SCANLINE FILL)]\n";
            updateTitle(&window);
            if (currentMode == AppMode::FILLING && isFilled) triggerFilling();
        }
        // Di chuyển và thay đổi kích thước cửa sổ xén
        else if (key == VK_LEFT) {
            clipWindow.move(-20, 0);
        } else if (key == VK_RIGHT) {
            clipWindow.move(20, 0);
        } else if (key == VK_UP) {
            clipWindow.move(0, 20);
        } else if (key == VK_DOWN) {
            clipWindow.move(0, -20);
        } else if (key == VK_OEM_4 || key == '[') { // Phím '['
            clipWindow.expandWidth(-20);
        } else if (key == VK_OEM_6 || key == ']') { // Phím ']'
            clipWindow.expandWidth(20);
        } else if (key == VK_OEM_COMMA || key == ',') {
            clipWindow.expandHeight(-20);
        } else if (key == VK_OEM_PERIOD || key == '.') {
            clipWindow.expandHeight(20);
        }
        // Nạp và sinh đa giác
        else if (key == 'O' || key == 'o') {
            std::cout << "Nhap duong dan file da giac (.txt) [Mac dinh: polygon.txt]: ";
            std::string filename;
            std::getline(std::cin, filename);
            if (filename.empty()) filename = "polygon.txt";
            if (loadPolygonFromFile(filename, currentPoly) ||
                loadPolygonFromFile("../" + filename, currentPoly) ||
                loadPolygonFromFile("../../" + filename, currentPoly)) {
                std::cout << "[Thanh cong] Da doc file " << filename << " (" << currentPoly.vertices.size() << " dinh)\n";
                isFilled = false;
                seedPoint = Point2D(-1, -1);
            } else {
                std::cout << "[Loi] Khong mo duoc file: " << filename << "\n";
            }
            updateTitle(&window);
        } else if (key == VK_F5) {
            currentPoly = createStarPolygon(450, 350, 220, 90);
            isFilled = false;
            seedPoint = Point2D(-1, -1);
            std::cout << "--> Da tao da giac Ngoi sao 5 canh (F5)\n";
            updateTitle(&window);
        } else if (key == VK_F6) {
            currentPoly = createArrowPolygon(450, 350, 380, 140);
            isFilled = false;
            seedPoint = Point2D(-1, -1);
            std::cout << "--> Da tao da giac Mui ten (F6)\n";
            updateTitle(&window);
        } else if (key == VK_F7) {
            currentPoly = createConcavePolygon(450, 350, 220);
            isFilled = false;
            seedPoint = Point2D(-1, -1);
            std::cout << "--> Da tao da giac Lom (F7)\n";
            updateTitle(&window);
        }
        // Xoá / Reset
        else if (key == 'X' || key == 'x') {
            if (currentMode == AppMode::CLIPPING) {
                linesHistory.clear();
                isDrawingLine = false;
                std::cout << "--> Da xoa sach danh sach doan thang!\n";
            } else {
                isFilled = false;
                fillCanvas.clear(colBackground);
                std::cout << "--> Da xoa ket qua to mau, tro ve khung vien!\n";
            }
            updateTitle(&window);
        } else if (key == 'G' || key == 'g') {
            showGrid = !showGrid;
            std::cout << "--> Luoi toa do: " << (showGrid ? "BAT" : "TAT") << "\n";
        }
        // Chụp ảnh màn hình BMP (F9)
        else if (key == VK_F9) {
            char fname[64];
            snprintf(fname, sizeof(fname), "screenshot_lab2_%03d.bmp", screenshotIndex++);
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
        std::cerr << "Loi khoi tao Window / OpenGL Context cho Lab 2!\n";
        return -1;
    }

    window.run();
    return 0;
}
