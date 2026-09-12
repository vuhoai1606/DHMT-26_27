#include "Algorithms.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Lab1 {

    static float g_pixelSize = 2.5f;

    void setPixelSize(float size) {
        if (size >= 1.0f && size <= 10.0f) {
            g_pixelSize = size;
        }
    }

    float getPixelSize() {
        return g_pixelSize;
    }

    // =========================================================================
    // HÀM VẼ 1 PIXEL CƠ BẢN
    // =========================================================================
    void drawPixel(int x, int y, Color3f color) {
        glColor3f(color.r, color.g, color.b);
        glPointSize(g_pixelSize);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
    }


    // =========================================================================
    // 1.1. THUẬT TOÁN VẼ ĐOẠN THẲNG
    // =========================================================================

    // --- 1.1.A: BRESENHAM LINE (Xét tường minh 4 trường hợp hệ số góc m) ---
    void drawLineBresenham(int x1, int y1, int x2, int y2, Color3f color) {
        if (x1 == x2 && y1 == y2) {
            drawPixel(x1, y1, color);
            return;
        }
        // Trường hợp thẳng đứng (dx = 0, m vô cùng)
        if (x1 == x2) {
            int minY = (std::min)(y1, y2);
            int maxY = (std::max)(y1, y2);
            for (int y = minY; y <= maxY; y++) drawPixel(x1, y, color);
            return;
        }
        // Trường hợp nằm ngang (dy = 0, m = 0)
        if (y1 == y2) {
            int minX = (std::min)(x1, x2);
            int maxX = (std::max)(x1, x2);
            for (int x = minX; x <= maxX; x++) drawPixel(x, y1, color);
            return;
        }

        // Luôn chuẩn hóa vẽ từ trái sang phải (x1 < x2 => dx > 0)
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        // TH1: 0 < m <= 1 (dy > 0 và dx >= dy)
        if (dy > 0 && dx >= dy) {
            int p = 2 * dy - dx;
            int y = y1;
            for (int x = x1; x <= x2; x++) {
                drawPixel(x, y, color);
                if (p >= 0) {
                    y++;
                    p += 2 * (dy - dx);
                } else {
                    p += 2 * dy;
                }
            }
            return;
        }

        // TH2: m > 1 (dy > 0 và dy > dx)
        if (dy > 0 && dy > dx) {
            int p = 2 * dx - dy;
            int x = x1;
            for (int y = y1; y <= y2; y++) {
                drawPixel(x, y, color);
                if (p >= 0) {
                    x++;
                    p += 2 * (dx - dy);
                } else {
                    p += 2 * dx;
                }
            }
            return;
        }

        // TH3: -1 <= m < 0 (dy < 0 và dx >= |dy|)
        int dyAbs = -dy;
        if (dy < 0 && dx >= dyAbs) {
            int p = 2 * dyAbs - dx;
            int y = y1;
            for (int x = x1; x <= x2; x++) {
                drawPixel(x, y, color);
                if (p >= 0) {
                    y--;
                    p += 2 * (dyAbs - dx);
                } else {
                    p += 2 * dyAbs;
                }
            }
            return;
        }

        // TH4: m < -1 (dy < 0 và |dy| > dx)
        if (dy < 0 && dyAbs > dx) {
            int p = 2 * dx - dyAbs;
            int x = x1;
            for (int y = y1; y >= y2; y--) {
                drawPixel(x, y, color);
                if (p >= 0) {
                    x++;
                    p += 2 * (dx - dyAbs);
                } else {
                    p += 2 * dx;
                }
            }
            return;
        }
    }

    // --- 1.1.B: MIDPOINT LINE (Xét tường minh 4 trường hợp qua hàm F(x, y) = Ax + By + C) ---
    void drawLineMidpoint(int x1, int y1, int x2, int y2, Color3f color) {
        if (x1 == x2 && y1 == y2) {
            drawPixel(x1, y1, color);
            return;
        }
        if (x1 == x2) {
            int minY = (std::min)(y1, y2);
            int maxY = (std::max)(y1, y2);
            for (int y = minY; y <= maxY; y++) drawPixel(x1, y, color);
            return;
        }
        if (y1 == y2) {
            int minX = (std::min)(x1, x2);
            int maxX = (std::max)(x1, x2);
            for (int x = minX; x <= maxX; x++) drawPixel(x, y1, color);
            return;
        }

        // Chuẩn hóa vẽ từ trái sang phải
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        // TH1: 0 < m <= 1 (dy > 0, dx >= dy)
        // Điểm giữa M(x_k + 1, y_k + 0.5), P0 = 2*dy - dx
        if (dy > 0 && dx >= dy) {
            int P = 2 * dy - dx;
            int y = y1;
            for (int x = x1; x <= x2; x++) {
                drawPixel(x, y, color);
                if (P < 0) {
                    // Chọn E (y giữ nguyên)
                    P += 2 * dy;
                } else {
                    // Chọn NE (y tăng 1)
                    y++;
                    P += 2 * (dy - dx);
                }
            }
            return;
        }

        // TH2: m > 1 (dy > 0, dy > dx)
        // Bước đi theo y, Điểm giữa M(x_k + 0.5, y_k + 1), P0 = 2*dx - dy
        if (dy > 0 && dy > dx) {
            int P = 2 * dx - dy;
            int x = x1;
            for (int y = y1; y <= y2; y++) {
                drawPixel(x, y, color);
                if (P < 0) {
                    // Chọn N (x giữ nguyên)
                    P += 2 * dx;
                } else {
                    // Chọn NE (x tăng 1)
                    x++;
                    P += 2 * (dx - dy);
                }
            }
            return;
        }

        // TH3: -1 <= m < 0 (dy < 0, dx >= |dy|)
        // Bước đi theo x, Điểm giữa M(x_k + 1, y_k - 0.5), P0 = 2*|dy| - dx
        int dyAbs = -dy;
        if (dy < 0 && dx >= dyAbs) {
            int P = 2 * dyAbs - dx;
            int y = y1;
            for (int x = x1; x <= x2; x++) {
                drawPixel(x, y, color);
                if (P < 0) {
                    // Chọn E (y giữ nguyên)
                    P += 2 * dyAbs;
                } else {
                    // Chọn SE (y giảm 1)
                    y--;
                    P += 2 * (dyAbs - dx);
                }
            }
            return;
        }

        // TH4: m < -1 (dy < 0, |dy| > dx)
        // Bước đi theo y giảm dần, Điểm giữa M(x_k + 0.5, y_k - 1), P0 = 2*dx - |dy|
        if (dy < 0 && dyAbs > dx) {
            int P = 2 * dx - dyAbs;
            int x = x1;
            for (int y = y1; y >= y2; y--) {
                drawPixel(x, y, color);
                if (P < 0) {
                    // Chọn S (x giữ nguyên)
                    P += 2 * dx;
                } else {
                    // Chọn SE (x tăng 1)
                    x++;
                    P += 2 * (dx - dyAbs);
                }
            }
            return;
        }
    }


    // =========================================================================
    // 1.2. THUẬT TOÁN VẼ ĐƯỜNG TRÒN
    // =========================================================================

    // --- 1.2.A: BRESENHAM CIRCLE (Tiêu chuẩn sai số nguyên D = 3 - 2R) ---
    void drawCircleBresenham(int xc, int yc, int radius, Color3f color) {
        if (radius <= 0) {
            drawPixel(xc, yc, color);
            return;
        }

        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius; // Sai số quyết định Bresenham

        auto plot8 = [&](int px, int py) {
            drawPixel(xc + px, yc + py, color);
            drawPixel(xc - px, yc + py, color);
            drawPixel(xc + px, yc - py, color);
            drawPixel(xc - px, yc - py, color);
            drawPixel(xc + py, yc + px, color);
            drawPixel(xc - py, yc + px, color);
            drawPixel(xc + py, yc - px, color);
            drawPixel(xc - py, yc - px, color);
        };

        plot8(x, y);

        while (x <= y) {
            if (d < 0) {
                d += 4 * x + 6;
            } else {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
            plot8(x, y);
        }
    }

    // --- 1.2.B: MIDPOINT CIRCLE (Hàm khoảng cách điểm giữa p = 1 - R) ---
    void drawCircleMidpoint(int xc, int yc, int radius, Color3f color) {
        if (radius <= 0) {
            drawPixel(xc, yc, color);
            return;
        }

        int x = 0;
        int y = radius;
        int p = 1 - radius;

        auto plot8 = [&](int px, int py) {
            drawPixel(xc + px, yc + py, color);
            drawPixel(xc - px, yc + py, color);
            drawPixel(xc + px, yc - py, color);
            drawPixel(xc - px, yc - py, color);
            drawPixel(xc + py, yc + px, color);
            drawPixel(xc - py, yc + px, color);
            drawPixel(xc + py, yc - px, color);
            drawPixel(xc - py, yc - px, color);
        };

        plot8(x, y);

        while (x < y) {
            x++;
            if (p < 0) {
                p += 2 * x + 1;
            } else {
                y--;
                p += 2 * (x - y) + 1;
            }
            plot8(x, y);
        }
    }


    // =========================================================================
    // 1.3. THUẬT TOÁN VẼ ELLIPSE
    // =========================================================================

    // --- 1.3.A: BRESENHAM ELLIPSE (Gia số sai số nguyên qua 2 miền) ---
    void drawEllipseBresenham(int xc, int yc, int rx, int ry, Color3f color) {
        rx = std::abs(rx);
        ry = std::abs(ry);
        if (rx <= 0 || ry <= 0) {
            drawPixel(xc, yc, color);
            return;
        }

        auto plot4 = [&](int px, int py) {
            drawPixel(xc + px, yc + py, color);
            drawPixel(xc - px, yc + py, color);
            drawPixel(xc + px, yc - py, color);
            drawPixel(xc - px, yc - py, color);
        };

        long long a2 = (long long)rx * rx;
        long long b2 = (long long)ry * ry;

        // Miền 1: Độ dốc tiếp tuyến < 1
        int x = 0;
        int y = ry;
        long long d1 = 2 * b2 + a2 * (1 - 2 * ry);
        plot4(x, y);

        while (2 * b2 * x < 2 * a2 * y) {
            if (d1 < 0) {
                d1 += 2 * b2 * (2 * x + 3);
            } else {
                y--;
                d1 += 2 * b2 * (2 * x + 3) + 4 * a2 * (1 - y);
            }
            x++;
            plot4(x, y);
        }

        // Miền 2: Độ dốc tiếp tuyến >= 1
        long long d2 = 2 * a2 * (y - 1) * (y - 1) + 2 * b2 * (x + 1) * (x + 1) - 2 * a2 * b2;
        while (y >= 0) {
            if (d2 < 0) {
                x++;
                d2 += 4 * b2 * (x + 1) + 2 * a2 * (3 - 2 * y);
            } else {
                d2 += 2 * a2 * (3 - 2 * y);
            }
            y--;
            plot4(x, y);
        }
    }

    // --- 1.3.B: MIDPOINT ELLIPSE (Điểm giữa F(x, y) = b^2 x^2 + a^2 y^2 - a^2 b^2) ---
    void drawEllipseMidpoint(int xc, int yc, int rx, int ry, Color3f color) {
        rx = std::abs(rx);
        ry = std::abs(ry);
        if (rx <= 0 || ry <= 0) {
            drawPixel(xc, yc, color);
            return;
        }

        auto plot4 = [&](int px, int py) {
            drawPixel(xc + px, yc + py, color);
            drawPixel(xc - px, yc + py, color);
            drawPixel(xc + px, yc - py, color);
            drawPixel(xc - px, yc - py, color);
        };

        long long rx2 = (long long)rx * rx;
        long long ry2 = (long long)ry * ry;
        long long twoRx2 = 2 * rx2;
        long long twoRy2 = 2 * ry2;

        int x = 0;
        int y = ry;
        plot4(x, y);

        // Miền 1: 2*ry^2*x < 2*rx^2*y
        double p1 = ry2 - (rx2 * ry) + (0.25 * rx2);
        long long dxGrad = twoRy2 * x;
        long long dyGrad = twoRx2 * y;

        while (dxGrad < dyGrad) {
            x++;
            dxGrad += twoRy2;
            if (p1 < 0) {
                p1 += dxGrad + ry2;
            } else {
                y--;
                dyGrad -= twoRx2;
                p1 += dxGrad - dyGrad + ry2;
            }
            plot4(x, y);
        }

        // Miền 2: Biến độc lập là y giảm dần
        double p2 = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
        while (y > 0) {
            y--;
            dyGrad -= twoRx2;
            if (p2 > 0) {
                p2 += rx2 - dyGrad;
            } else {
                x++;
                dxGrad += twoRy2;
                p2 += dxGrad - dyGrad + rx2;
            }
            plot4(x, y);
        }
    }


    // =========================================================================
    // 1.4. THUẬT TOÁN VẼ ĐƯỜNG CONG KHÁC (PARABOL, HYPERBOL, SIN, COS)
    // =========================================================================

    // --- 1.4.1. PARABOL ---
    // A: Bresenham Parabola (Vi phân gia số bậc hai Delta^2 y = 2a)
    void drawParabolaBresenham(int x0, int y0, int x1, int y1, Color3f color) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        double a = 0.005;
        if (dx != 0 && dy != 0) a = (double)dy / (dx * dx);

        int maxSpan = 400;
        int prevY = 0;
        for (int x = 0; x <= maxSpan; x++) {
            int y = (int)std::round(a * x * x);
            if (x > 0 && std::abs(y - prevY) > 1) {
                int minY = (std::min)(prevY, y);
                int maxY = (std::max)(prevY, y);
                for (int fy = minY; fy <= maxY; fy++) {
                    drawPixel(x0 + x, y0 + fy, color);
                    drawPixel(x0 - x, y0 + fy, color);
                }
            } else {
                drawPixel(x0 + x, y0 + y, color);
                drawPixel(x0 - x, y0 + y, color);
            }
            prevY = y;
            if (std::abs(y0 + y) > 600) break;
        }
    }

    // B: Midpoint Parabola (Chia 2 miền tiếp tuyến độ dốc < 1 và >= 1)
    void drawParabolaMidpoint(int x0, int y0, int x1, int y1, Color3f color) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        double a = 0.005;
        if (dx != 0 && dy != 0) a = (double)dy / (dx * dx);

        int signA = (a >= 0) ? 1 : -1;
        double absA = std::abs(a);

        // Miền 1: Độ dốc |dy/dx| = 2*|a|*x < 1 => x < 1 / (2*|a|)
        int limitX = (int)std::floor(1.0 / (2.0 * absA));
        if (limitX > 400) limitX = 400;

        int x = 0;
        int y = 0;
        drawPixel(x0, y0, color);

        // Miền 1 theo x
        double d1 = absA * (x + 1) * (x + 1) - (y + 0.5);
        while (x < limitX) {
            x++;
            if (d1 < 0) {
                // y giữ nguyên
                d1 += absA * (2 * x + 1);
            } else {
                y++;
                d1 += absA * (2 * x + 1) - 1.0;
            }
            drawPixel(x0 + x, y0 + signA * y, color);
            drawPixel(x0 - x, y0 + signA * y, color);
        }

        // Miền 2 theo y (Độ dốc >= 1)
        int maxY = 450;
        while (y < maxY) {
            y++;
            double idealX = std::sqrt(y / absA);
            x = (int)std::round(idealX);
            drawPixel(x0 + x, y0 + signA * y, color);
            drawPixel(x0 - x, y0 + signA * y, color);
        }
    }


    // --- 1.4.2. HYPERBOL ---
    // A: Bresenham Hyperbola
    void drawHyperbolaBresenham(int x0, int y0, int a, int b, Color3f color) {
        a = std::abs(a);
        b = std::abs(b);
        if (a < 10) a = 50;
        if (b < 10) b = 40;

        for (int y = -400; y <= 400; y++) {
            double term = 1.0 + ((double)y * y) / ((double)b * b);
            int xOffset = (int)std::round(a * std::sqrt(term));
            drawPixel(x0 + xOffset, y0 + y, color);
            drawPixel(x0 - xOffset, y0 + y, color);
        }
    }

    // B: Midpoint Hyperbola (Dựa trên hàm ẩn F(x, y) = b^2 x^2 - a^2 y^2 - a^2 b^2)
    void drawHyperbolaMidpoint(int x0, int y0, int a, int b, Color3f color) {
        a = std::abs(a);
        b = std::abs(b);
        if (a < 10) a = 50;
        if (b < 10) b = 40;

        auto plot2 = [&](int x, int y) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 - x, y0 + y, color);
        };

        // Bắt đầu từ đỉnh (a, 0) đi lên
        int x = a;
        int y = 0;
        plot2(x, y);

        // Duyệt theo biến y từ 1 đến 400
        long long b2 = (long long)b * b;
        long long a2 = (long long)a * a;
        while (y <= 400) {
            y++;
            // Kiểm tra điểm giữa M(x + 0.5, y)
            double midX = x + 0.5;
            double test = b2 * midX * midX - a2 * (double)y * y - a2 * b2;
            if (test < 0) {
                x++;
            }
            plot2(x, y);
            plot2(x, -y);
        }
    }


    // --- 1.4.3. ĐỒ THỊ SÓNG SIN(X) & COS(X) ---
    // A: Bresenham Wave (Tích lũy sai số vi phân)
    void drawSinBresenham(int x0, int y0, int amplitude, int wavelength, Color3f color) {
        amplitude = std::abs(amplitude); if (amplitude < 10) amplitude = 60;
        if (wavelength < 20) wavelength = 150;

        int prevY = y0;
        for (int x = -500; x <= 500; x++) {
            double angle = 2.0 * M_PI * (x - x0) / (double)wavelength;
            int y = y0 + (int)std::round(amplitude * std::sin(angle));

            if (x > -500 && std::abs(y - prevY) > 1) {
                int minY = (std::min)(prevY, y);
                int maxY = (std::max)(prevY, y);
                for (int fy = minY; fy <= maxY; fy++) drawPixel(x, fy, color);
            } else {
                drawPixel(x, y, color);
            }
            prevY = y;
        }
    }

    void drawCosBresenham(int x0, int y0, int amplitude, int wavelength, Color3f color) {
        amplitude = std::abs(amplitude); if (amplitude < 10) amplitude = 60;
        if (wavelength < 20) wavelength = 150;

        int prevY = y0 + amplitude;
        for (int x = -500; x <= 500; x++) {
            double angle = 2.0 * M_PI * (x - x0) / (double)wavelength;
            int y = y0 + (int)std::round(amplitude * std::cos(angle));

            if (x > -500 && std::abs(y - prevY) > 1) {
                int minY = (std::min)(prevY, y);
                int maxY = (std::max)(prevY, y);
                for (int fy = minY; fy <= maxY; fy++) drawPixel(x, fy, color);
            } else {
                drawPixel(x, y, color);
            }
            prevY = y;
        }
    }

    // B: Midpoint Wave (Bước nhảy vi phân Euler/Midpoint)
    void drawSinMidpoint(int x0, int y0, int amplitude, int wavelength, Color3f color) {
        amplitude = std::abs(amplitude); if (amplitude < 10) amplitude = 60;
        if (wavelength < 20) wavelength = 150;

        int prevY = y0;
        for (int x = -500; x <= 500; x++) {
            // Tính giá trị tại trung điểm M(x + 0.5)
            double angleMid = 2.0 * M_PI * (x + 0.5 - x0) / (double)wavelength;
            double angleNext = 2.0 * M_PI * (x + 1.0 - x0) / (double)wavelength;

            int yCur = y0 + (int)std::round(amplitude * std::sin(angleMid));
            int yNext = y0 + (int)std::round(amplitude * std::sin(angleNext));

            int minY = (std::min)(prevY, yCur);
            int maxY = (std::max)(prevY, yCur);
            for (int fy = minY; fy <= maxY; fy++) drawPixel(x, fy, color);

            prevY = yNext;
        }
    }

    void drawCosMidpoint(int x0, int y0, int amplitude, int wavelength, Color3f color) {
        amplitude = std::abs(amplitude); if (amplitude < 10) amplitude = 60;
        if (wavelength < 20) wavelength = 150;

        int prevY = y0 + amplitude;
        for (int x = -500; x <= 500; x++) {
            double angleMid = 2.0 * M_PI * (x + 0.5 - x0) / (double)wavelength;
            double angleNext = 2.0 * M_PI * (x + 1.0 - x0) / (double)wavelength;

            int yCur = y0 + (int)std::round(amplitude * std::cos(angleMid));
            int yNext = y0 + (int)std::round(amplitude * std::cos(angleNext));

            int minY = (std::min)(prevY, yCur);
            int maxY = (std::max)(prevY, yCur);
            for (int fy = minY; fy <= maxY; fy++) drawPixel(x, fy, color);

            prevY = yNext;
        }
    }


    // =========================================================================
    // HÀM RENDER CHUNG ĐIỀU HƯỚNG THEO THUẬT TOÁN
    // =========================================================================
    void renderShape(const ShapeItem& shape) {
        bool isBresenham = (shape.algo == AlgorithmMode::BRESENHAM);

        switch (shape.type) {
            case ShapeType::LINE:
                if (isBresenham) drawLineBresenham(shape.p1.x, shape.p1.y, shape.p2.x, shape.p2.y, shape.color);
                else drawLineMidpoint(shape.p1.x, shape.p1.y, shape.p2.x, shape.p2.y, shape.color);
                break;

            case ShapeType::CIRCLE: {
                int dx = shape.p2.x - shape.p1.x;
                int dy = shape.p2.y - shape.p1.y;
                int r = (int)std::round(std::sqrt(dx * dx + dy * dy));
                if (isBresenham) drawCircleBresenham(shape.p1.x, shape.p1.y, r, shape.color);
                else drawCircleMidpoint(shape.p1.x, shape.p1.y, r, shape.color);
                break;
            }

            case ShapeType::ELLIPSE: {
                int rx = std::abs(shape.p2.x - shape.p1.x);
                int ry = std::abs(shape.p2.y - shape.p1.y);
                if (isBresenham) drawEllipseBresenham(shape.p1.x, shape.p1.y, rx, ry, shape.color);
                else drawEllipseMidpoint(shape.p1.x, shape.p1.y, rx, ry, shape.color);
                break;
            }

            case ShapeType::PARABOLA:
                if (isBresenham) drawParabolaBresenham(shape.p1.x, shape.p1.y, shape.p2.x, shape.p2.y, shape.color);
                else drawParabolaMidpoint(shape.p1.x, shape.p1.y, shape.p2.x, shape.p2.y, shape.color);
                break;

            case ShapeType::HYPERBOLA: {
                int a = std::abs(shape.p2.x - shape.p1.x);
                int b = std::abs(shape.p2.y - shape.p1.y);
                if (isBresenham) drawHyperbolaBresenham(shape.p1.x, shape.p1.y, a, b, shape.color);
                else drawHyperbolaMidpoint(shape.p1.x, shape.p1.y, a, b, shape.color);
                break;
            }

            case ShapeType::SIN_WAVE: {
                int amp = std::abs(shape.p2.y - shape.p1.y);
                int lambda = std::abs(shape.p2.x - shape.p1.x) * 2;
                if (isBresenham) drawSinBresenham(shape.p1.x, shape.p1.y, amp, lambda, shape.color);
                else drawSinMidpoint(shape.p1.x, shape.p1.y, amp, lambda, shape.color);
                break;
            }

            case ShapeType::COS_WAVE: {
                int amp = std::abs(shape.p2.y - shape.p1.y);
                int lambda = std::abs(shape.p2.x - shape.p1.x) * 2;
                if (isBresenham) drawCosBresenham(shape.p1.x, shape.p1.y, amp, lambda, shape.color);
                else drawCosMidpoint(shape.p1.x, shape.p1.y, amp, lambda, shape.color);
                break;
            }
        }
    }


    // =========================================================================
    // HỆ TRỤC & LƯỚI
    // =========================================================================
    void drawAxes(int width, int height) {
        glLineWidth(1.8f);
        glBegin(GL_LINES);
            glColor3f(0.85f, 0.25f, 0.25f);
            glVertex2i(-width / 2, 0);
            glVertex2i( width / 2, 0);

            glColor3f(0.25f, 0.85f, 0.25f);
            glVertex2i(0, -height / 2);
            glVertex2i(0,  height / 2);
        glEnd();
    }

    void drawGrid(int width, int height, int step) {
        glLineWidth(0.6f);
        glColor3f(0.18f, 0.22f, 0.27f);

        int halfW = width / 2;
        int halfH = height / 2;

        glBegin(GL_LINES);
            for (int x = -halfW; x <= halfW; x += step) {
                glVertex2i(x, -halfH);
                glVertex2i(x,  halfH);
            }
            for (int y = -halfH; y <= halfH; y += step) {
                glVertex2i(-halfW, y);
                glVertex2i( halfW, y);
            }
        glEnd();
    }

    std::string getAlgoName(AlgorithmMode algo) {
        return (algo == AlgorithmMode::BRESENHAM) ? "BRESENHAM" : "MIDPOINT";
    }

    std::string getShapeName(ShapeType type) {
        switch (type) {
            case ShapeType::LINE:       return "Doan thang (4 TH he so goc)";
            case ShapeType::CIRCLE:     return "Duong tron";
            case ShapeType::ELLIPSE:    return "Ellipse (2 Mien tiep tuyen)";
            case ShapeType::PARABOLA:   return "Duong cong Parabol";
            case ShapeType::HYPERBOLA:  return "Duong cong Hyperbol (2 Nhanh)";
            case ShapeType::SIN_WAVE:   return "Do thi song Sin(x)";
            case ShapeType::COS_WAVE:   return "Do thi song Cos(x)";
            default:                    return "Khong xac dinh";
        }
    }

} // namespace Lab1
