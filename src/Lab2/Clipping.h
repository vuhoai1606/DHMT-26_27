#ifndef LAB2_CLIPPING_H
#define LAB2_CLIPPING_H

#include <vector>
#include <string>

namespace Lab2 {

struct Point2D {
    double x, y;
    Point2D(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

struct Line2D {
    Point2D a, b;
    bool isClipped;
    Line2D() : isClipped(false) {}
    Line2D(Point2D _a, Point2D _b) : a(_a), b(_b), isClipped(false) {}
};

struct ClipRect {
    double xmin, ymin, xmax, ymax;
    ClipRect(double x1 = 150, double y1 = 120, double x2 = 650, double y2 = 500)
        : xmin(x1), ymin(y1), xmax(x2), ymax(y2) {}

    void move(double dx, double dy) {
        xmin += dx; xmax += dx;
        ymin += dy; ymax += dy;
    }

    void expandWidth(double delta) {
        if (xmax - xmin + delta * 2 >= 40) {
            xmin -= delta;
            xmax += delta;
        }
    }

    void expandHeight(double delta) {
        if (ymax - ymin + delta * 2 >= 40) {
            ymin -= delta;
            ymax += delta;
        }
    }
};

// 4-bit region codes
enum OutCode {
    INSIDE = 0, // 0000
    LEFT   = 1, // 0001
    RIGHT  = 2, // 0010
    BOTTOM = 4, // 0100
    TOP    = 8  // 1000
};

// Hàm tính OutCode cho 1 điểm so với cửa sổ xén
int computeOutCode(const Point2D& p, const ClipRect& r);

// Thuật toán xén đoạn thẳng Cohen-Sutherland
// Trả về true nếu một phần hoặc toàn bộ đoạn thẳng nằm trong cửa sổ, kết quả lưu vào outLine
bool cohenSutherlandClip(Line2D inLine, const ClipRect& r, Line2D& outLine);

// Thuật toán xén đoạn thẳng chia nhị phân (Midpoint Subdivision / Binary Clipping)
// Trả về true nếu một phần hoặc toàn bộ đoạn thẳng nằm trong cửa sổ, kết quả lưu vào outLine
bool midpointSubdivisionClip(Line2D inLine, const ClipRect& r, Line2D& outLine);

} // namespace Lab2

#endif // LAB2_CLIPPING_H
