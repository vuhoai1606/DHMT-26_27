#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "Window.h"
#include "Algorithms.h"

using namespace Lab1;

// Trạng thái chương trình
AlgorithmMode currentAlgo = AlgorithmMode::BRESENHAM; // Mặc định là Bresenham
ShapeType currentMode = ShapeType::LINE;              // Mặc định là vẽ đoạn thẳng
int clickStep = 0;                                    // 0: Chờ click 1, 1: Đã click 1, đang xem trước (preview)
Point2D startPoint;
Point2D currentMouse;

bool showGrid = true;
std::vector<ShapeItem> shapesHistory;

// Bảng màu phân biệt giữa 2 thuật toán:
// Bresenham: Gam màu Lạnh / Cam (Cyan, Lam, Cam đỏ, Hồng tím)
const Color3f paletteBresenham[] = {
    Color3f(0.20f, 0.85f, 1.00f), // Cyan
    Color3f(1.00f, 0.45f, 0.20f), // Cam đỏ
    Color3f(0.35f, 0.65f, 1.00f), // Xanh dương
    Color3f(1.00f, 0.30f, 0.60f)  // Hồng sen
};
// Midpoint: Gam màu Vàng / Xanh lá (Lục, Vàng neon, Xanh ngọc, Tím nhạt)
const Color3f paletteMidpoint[] = {
    Color3f(0.25f, 1.00f, 0.40f), // Xanh lá mạ
    Color3f(1.00f, 0.90f, 0.20f), // Vàng tươi
    Color3f(0.20f, 1.00f, 0.85f), // Xanh ngọc
    Color3f(0.85f, 0.40f, 1.00f)  // Tím sáng
};
int bresCount = 0;
int midCount = 0;

void printMenu() {
    std::cout << "=================================================================" << std::endl;
    std::cout << "    LAB 01 - SO SANH THUAT TOAN BRESENHAM VA MIDPOINT (OPENGL)   " << std::endl;
    std::cout << "=================================================================" << std::endl;
    std::cout << " [*] PHIM [M] HOAC [TAB]: Chuyen doi thuat toan BRESENHAM <-> MIDPOINT" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << " CHON DOI TUONG VE (Phim 1 -> 7):" << std::endl;
    std::cout << " [1] Doan thang (Xet day du 4 truong hop he so goc m)" << std::endl;
    std::cout << " [2] Duong tron (Doi xung 8 phan)" << std::endl;
    std::cout << " [3] Ellipse    (Doi xung 4 phan, chia 2 mien tiep tuyen)" << std::endl;
    std::cout << " [4] Parabol    (Dinh va Diem di qua)" << std::endl;
    std::cout << " [5] Hyperbol   (Tam va 2 Ban truc a, b)" << std::endl;
    std::cout << " [6] Song Sin(x)(Goc song, Bien do va Buoc song)" << std::endl;
    std::cout << " [7] Song Cos(x)(Goc song, Bien do va Buoc song)" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << " THAO TAC CHUOT & PHIM TIEN ICH:" << std::endl;
    std::cout << " - Click chuot trai lan 1: Chon diem bat dau / Tam / Dinh        " << std::endl;
    std::cout << " - Di chuot             : Xem truoc duong net (Live Preview)    " << std::endl;
    std::cout << " - Click chuot trai lan 2: Chot hinh ve                         " << std::endl;
    std::cout << " - Phim [Z]             : Hoan tac hinh vua ve (Undo)           " << std::endl;
    std::cout << " - Phim [C]             : Xoa sach Canvas (Clear all)           " << std::endl;
    std::cout << " - Phim [G]             : Bat / Tat luoi toa do Pixel           " << std::endl;
    std::cout << " - Phim [+] / [-]       : Tang / Giam kich thuoc Pixel          " << std::endl;
    std::cout << " - Phim [ESC]           : Thoat chuong trinh                    " << std::endl;
    std::cout << "=================================================================" << std::endl;
}

void updateWindowTitle(Window* pWin) {
    std::string title = "LAB 01 | [Thuat toan: " + getAlgoName(currentAlgo) + "]";
    title += " | [" + getShapeName(currentMode) + "]";
    title += " | Chuot: (" + std::to_string(currentMouse.x) + ", " + std::to_string(currentMouse.y) + ")";
    title += " | Pixel: " + std::to_string((int)getPixelSize()) + "px";
    if (clickStep == 1) {
        title += " [DANG VE... Click lan 2 de chot]";
    } else {
        title += " [Nhan M de doi sang " + (currentAlgo == AlgorithmMode::BRESENHAM ? std::string("MIDPOINT") : std::string("BRESENHAM")) + "]";
    }
    SetWindowTextA(pWin->getHWND(), title.c_str());
}

int main() {
    printMenu();

    Window window("LAB 01 - Thuat Toan Bresenham & Midpoint", 980, 700);

    window.onInit = [&]() {
        // Màu nền tối dịu mắt chuyên dùng quan sát điểm pixel
        glClearColor(0.07f, 0.09f, 0.12f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        updateWindowTitle(&window);
    };

    window.onResize = [&](int width, int height) {
        if (height == 0) height = 1;
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    };

    window.onRender = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Vẽ lưới toạ độ pixel
        if (showGrid) {
            drawGrid(window.getWidth(), window.getHeight(), 30);
        }

        // 2. Vẽ hệ trục toạ độ Ox, Oy
        drawAxes(window.getWidth(), window.getHeight());

        // 3. Vẽ toàn bộ các hình đã chốt trong lịch sử (giữ nguyên thuật toán của từng hình)
        for (const auto& item : shapesHistory) {
            renderShape(item);
        }

        // 4. Vẽ hình đang xem trước (Preview) theo con trỏ chuột
        if (clickStep == 1) {
            ShapeItem previewItem;
            previewItem.type = currentMode;
            previewItem.algo = currentAlgo;
            previewItem.p1 = startPoint;
            previewItem.p2 = currentMouse;
            previewItem.color = (currentAlgo == AlgorithmMode::BRESENHAM)
                                    ? Color3f(1.0f, 0.6f, 0.2f)  // Màu cam nếu là Bresenham
                                    : Color3f(1.0f, 1.0f, 0.2f); // Màu vàng nếu là Midpoint
            renderShape(previewItem);

            // Điểm mốc đỏ đánh dấu vị trí click 1
            drawPixel(startPoint.x, startPoint.y, Color3f(1.0f, 0.2f, 0.2f));
        }

        // 5. Đánh dấu vị trí con trỏ chuột hiện tại
        drawPixel(currentMouse.x, currentMouse.y, Color3f(1.0f, 1.0f, 1.0f));
    };

    window.onMouseMove = [&](int cartX, int cartY) {
        currentMouse.x = cartX;
        currentMouse.y = cartY;
        updateWindowTitle(&window);
    };

    window.onMouse = [&](int cartX, int cartY, int button, bool isDown) {
        // Chuột trái (button 0): Vẽ hình
        if (button == 0 && isDown) {
            if (clickStep == 0) {
                // Click 1: Đặt điểm đầu
                startPoint.x = cartX;
                startPoint.y = cartY;
                clickStep = 1;
                std::cout << "-> Click 1 tai: (" << cartX << ", " << cartY << ")" << std::endl;
            } else if (clickStep == 1) {
                // Click 2: Chốt hình vẽ
                ShapeItem newItem;
                newItem.type = currentMode;
                newItem.algo = currentAlgo; // Lưu thuật toán đang chọn
                newItem.p1 = startPoint;
                newItem.p2 = Point2D(cartX, cartY);

                if (currentAlgo == AlgorithmMode::BRESENHAM) {
                    newItem.color = paletteBresenham[bresCount % 4];
                    bresCount++;
                } else {
                    newItem.color = paletteMidpoint[midCount % 4];
                    midCount++;
                }

                shapesHistory.push_back(newItem);
                clickStep = 0;

                std::cout << "-> Da ve [" << getShapeName(currentMode)
                          << "] bang thuat toan [" << getAlgoName(currentAlgo)
                          << "] tu (" << startPoint.x << ", " << startPoint.y
                          << ") den (" << cartX << ", " << cartY << ")" << std::endl;
            }
            updateWindowTitle(&window);
        } else if (button == 1 && isDown) {
            // Chuột phải (button 1): Hủy nét vẽ dở
            if (clickStep == 1) {
                clickStep = 0;
                std::cout << "-> Da huy thao tac ve dở." << std::endl;
                updateWindowTitle(&window);
            }
        }
    };

    window.onKey = [&](int key, bool isDown) {
        if (!isDown) return;

        // Phím M hoặc TAB: Đổi thuật toán Bresenham <-> Midpoint
        if (key == 'M' || key == 'm' || key == VK_TAB) {
            currentAlgo = (currentAlgo == AlgorithmMode::BRESENHAM)
                              ? AlgorithmMode::MIDPOINT
                              : AlgorithmMode::BRESENHAM;
            updateWindowTitle(&window);
            std::cout << "=========================================================" << std::endl;
            std::cout << "-> DANG SU DUNG THUAT TOAN: [" << getAlgoName(currentAlgo) << "]" << std::endl;
            std::cout << "=========================================================" << std::endl;
        } else if (key >= '1' && key <= '7') {
            currentMode = static_cast<ShapeType>(key - '0');
            clickStep = 0; // Reset bước click
            updateWindowTitle(&window);
            std::cout << "-> Chon doi tuong: [" << getShapeName(currentMode)
                      << "] | Thuat toan: [" << getAlgoName(currentAlgo) << "]" << std::endl;
        } else if (key == 'Z' || key == 'z') {
            // Hoàn tác
            if (!shapesHistory.empty()) {
                shapesHistory.pop_back();
                std::cout << "-> Da hoan tac. Con lai: " << shapesHistory.size() << " hinh." << std::endl;
            }
        } else if (key == 'C' || key == 'c') {
            // Xóa canvas
            shapesHistory.clear();
            clickStep = 0;
            std::cout << "-> Da xoa sach Canvas!" << std::endl;
        } else if (key == 'G' || key == 'g') {
            showGrid = !showGrid;
            std::cout << "-> Luoi toa do: " << (showGrid ? "BAT" : "TAT") << std::endl;
        } else if (key == VK_OEM_PLUS || key == 187 || key == VK_ADD) {
            setPixelSize(getPixelSize() + 1.0f);
            updateWindowTitle(&window);
            std::cout << "-> Kich thuoc pixel: " << getPixelSize() << "px" << std::endl;
        } else if (key == VK_OEM_MINUS || key == 189 || key == VK_SUBTRACT) {
            setPixelSize(getPixelSize() - 1.0f);
            updateWindowTitle(&window);
            std::cout << "-> Kich thuoc pixel: " << getPixelSize() << "px" << std::endl;
        } else if (key == VK_ESCAPE) {
            window.close();
        }
    };

    if (!window.init()) {
        std::cerr << "Loi khoi tao Window / OpenGL Context cho Lab 1!" << std::endl;
        return -1;
    }

    window.run();
    return 0;
}
