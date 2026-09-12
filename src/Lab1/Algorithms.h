#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

namespace Lab1 {

    struct Color3f {
        float r, g, b;
        Color3f(float red = 1.0f, float green = 1.0f, float blue = 1.0f)
            : r(red), g(green), b(blue) {}
    };

    struct Point2D {
        int x, y;
        Point2D(int px = 0, int py = 0) : x(px), y(py) {}
    };

    // Chế độ thuật toán lựa chọn: Bresenham hoặc Midpoint
    enum class AlgorithmMode {
        BRESENHAM = 1,
        MIDPOINT = 2
    };

    // Loại hình cần vẽ
    enum class ShapeType {
        LINE = 1,       // 1.1 Đoạn thẳng (xét 4 trường hợp hệ số góc m)
        CIRCLE = 2,     // 1.2 Đường tròn
        ELLIPSE = 3,    // 1.3 Ellipse
        PARABOLA = 4,   // 1.4 Đường cong Parabol
        HYPERBOLA = 5,  // 1.4 Đường cong Hyperbol
        SIN_WAVE = 6,   // 1.4 Đồ thị sóng Sin(x)
        COS_WAVE = 7    // 1.4 Đồ thị sóng Cos(x)
    };

    struct ShapeItem {
        ShapeType type;
        AlgorithmMode algo; // Thuật toán được dùng để vẽ hình này
        Point2D p1;         // Điểm bắt đầu / Tâm / Đỉnh
        Point2D p2;         // Điểm kết thúc / Bán kính / Biên độ
        Color3f color;
    };

    // Quản lý kích thước hiển thị pixel
    void setPixelSize(float size);
    float getPixelSize();

    // Vẽ 1 pixel cơ bản
    void drawPixel(int x, int y, Color3f color);

    // =========================================================================
    // 1.1. THUẬT TOÁN VẼ ĐOẠN THẲNG (XÉT ĐẦY ĐỦ 4 TRƯỜNG HỢP HỆ SỐ GÓC)
    // =========================================================================
    void drawLineBresenham(int x1, int y1, int x2, int y2, Color3f color);
    void drawLineMidpoint(int x1, int y1, int x2, int y2, Color3f color);

    // =========================================================================
    // 1.2. THUẬT TOÁN VẼ ĐƯỜNG TRÒN
    // =========================================================================
    void drawCircleBresenham(int xc, int yc, int radius, Color3f color);
    void drawCircleMidpoint(int xc, int yc, int radius, Color3f color);

    // =========================================================================
    // 1.3. THUẬT TOÁN VẼ ELLIPSE
    // =========================================================================
    void drawEllipseBresenham(int xc, int yc, int rx, int ry, Color3f color);
    void drawEllipseMidpoint(int xc, int yc, int rx, int ry, Color3f color);

    // =========================================================================
    // 1.4. THUẬT TOÁN VẼ CÁC ĐƯỜNG CONG KHÁC (PARABOL, HYPERBOL, SIN, COS)
    // =========================================================================
    // Parabol đỉnh (x0, y0) đi qua (x1, y1)
    void drawParabolaBresenham(int x0, int y0, int x1, int y1, Color3f color);
    void drawParabolaMidpoint(int x0, int y0, int x1, int y1, Color3f color);

    // Hyperbol tâm (x0, y0) với bán trục a, b
    void drawHyperbolaBresenham(int x0, int y0, int a, int b, Color3f color);
    void drawHyperbolaMidpoint(int x0, int y0, int a, int b, Color3f color);

    // Đồ thị sóng Sin(x)
    void drawSinBresenham(int x0, int y0, int amplitude, int wavelength, Color3f color);
    void drawSinMidpoint(int x0, int y0, int amplitude, int wavelength, Color3f color);

    // Đồ thị sóng Cos(x)
    void drawCosBresenham(int x0, int y0, int amplitude, int wavelength, Color3f color);
    void drawCosMidpoint(int x0, int y0, int amplitude, int wavelength, Color3f color);

    // Hàm vẽ chung tự động định tuyến theo thuật toán
    void renderShape(const ShapeItem& shape);

    // Hệ trục toạ độ & Lưới
    void drawAxes(int width, int height);
    void drawGrid(int width, int height, int step = 25);

    // Tên hiển thị
    std::string getAlgoName(AlgorithmMode algo);
    std::string getShapeName(ShapeType type);

} // namespace Lab1

#endif // ALGORITHMS_H
