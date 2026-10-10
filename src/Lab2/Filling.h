#ifndef LAB2_FILLING_H
#define LAB2_FILLING_H

#include "Clipping.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>

namespace Lab2 {

struct ColorRGB {
    unsigned char r, g, b;
    ColorRGB(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0)
        : r(_r), g(_g), b(_b) {}

    bool operator==(const ColorRGB& o) const { return r == o.r && g == o.g && b == o.b; }
    bool operator!=(const ColorRGB& o) const { return !(*this == o); }
};

struct Polygon2D {
    std::vector<Point2D> vertices;

    void clear() { vertices.clear(); }
    void addVertex(double x, double y) { vertices.push_back(Point2D(x, y)); }
    Point2D getCentroid() const;
    void getBoundingBox(double& minX, double& minY, double& maxX, double& maxY) const;
};

// Canvas điểm ảnh quản lý mảng màu CPU
class Canvas {
public:
    int width;
    int height;
    std::vector<unsigned char> pixels; // size: width * height * 3 (RGB)

    Canvas(int w = 900, int h = 700);
    void resize(int w, int h);
    void clear(ColorRGB bgColor);

    void setPixel(int x, int y, ColorRGB c);
    ColorRGB getPixel(int x, int y) const;

    // Vẽ đường thẳng raster bằng thuật toán Bresenham
    void drawLineBresenham(int x1, int y1, int x2, int y2, ColorRGB c);

    // Vẽ khung biên đa giác
    void drawPolygonBoundary(const Polygon2D& poly, ColorRGB boundaryColor);

    // Đẩy framebuffer lên màn hình OpenGL
    void renderGL() const;
};

// 1. Tô màu theo đường biên dùng đệ quy (Recursive Boundary Fill)
void boundaryFillRecursive(Canvas& canvas, int x, int y, ColorRGB fillColor, ColorRGB boundaryColor, int& callCount, int maxDepth = 250000);

// 2. Tô màu theo đường biên KHÔNG đệ quy (Stack Boundary Fill)
void boundaryFillStack(Canvas& canvas, int startX, int startY, ColorRGB fillColor, ColorRGB boundaryColor);

// 3. Tô màu đa giác theo dòng quét (Scanline Polygon Fill)
void scanlinePolygonFill(Canvas& canvas, const Polygon2D& poly, ColorRGB fillColor);

// Đọc đa giác từ file văn bản (.txt)
bool loadPolygonFromFile(const std::string& filename, Polygon2D& poly);

// Tạo các đa giác mẫu trực quan
Polygon2D createStarPolygon(double cx, double cy, double rOuter, double rInner);
Polygon2D createArrowPolygon(double cx, double cy, double length, double width);
Polygon2D createConcavePolygon(double cx, double cy, double size);

// Chụp màn hình và lưu thành file ảnh 24-bit BMP
bool saveScreenshotBMP(const std::string& filename, int width, int height);

} // namespace Lab2

#endif // LAB2_FILLING_H
