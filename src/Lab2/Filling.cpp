#include "Filling.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Lab2 {

Point2D Polygon2D::getCentroid() const {
    if (vertices.empty()) return Point2D(0, 0);
    double sumX = 0, sumY = 0;
    for (const auto& v : vertices) {
        sumX += v.x;
        sumY += v.y;
    }
    return Point2D(sumX / vertices.size(), sumY / vertices.size());
}

void Polygon2D::getBoundingBox(double& minX, double& minY, double& maxX, double& maxY) const {
    if (vertices.empty()) {
        minX = minY = maxX = maxY = 0;
        return;
    }
    minX = maxX = vertices[0].x;
    minY = maxY = vertices[0].y;
    for (const auto& v : vertices) {
        if (v.x < minX) minX = v.x;
        if (v.x > maxX) maxX = v.x;
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }
}

// -----------------------------------------------------------------------------
// Canvas implementation
// -----------------------------------------------------------------------------
Canvas::Canvas(int w, int h) : width(w), height(h) {
    pixels.resize(width * height * 3, 0);
}

void Canvas::resize(int w, int h) {
    width = w;
    height = h;
    pixels.assign(width * height * 3, 0);
}

void Canvas::clear(ColorRGB bgColor) {
    for (int i = 0; i < width * height; ++i) {
        pixels[i * 3 + 0] = bgColor.r;
        pixels[i * 3 + 1] = bgColor.g;
        pixels[i * 3 + 2] = bgColor.b;
    }
}

void Canvas::setPixel(int x, int y, ColorRGB c) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = (y * width + x) * 3;
        pixels[idx + 0] = c.r;
        pixels[idx + 1] = c.g;
        pixels[idx + 2] = c.b;
    }
}

ColorRGB Canvas::getPixel(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = (y * width + x) * 3;
        return ColorRGB(pixels[idx + 0], pixels[idx + 1], pixels[idx + 2]);
    }
    // Ra ngoài biên thì coi như màu đường biên để chặn tràn loang
    return ColorRGB(255, 255, 255);
}

void Canvas::drawLineBresenham(int x1, int y1, int x2, int y2, ColorRGB c) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(x1, y1, c);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

void Canvas::drawPolygonBoundary(const Polygon2D& poly, ColorRGB boundaryColor) {
    int n = static_cast<int>(poly.vertices.size());
    if (n < 2) return;
    for (int i = 0; i < n; ++i) {
        const Point2D& p1 = poly.vertices[i];
        const Point2D& p2 = poly.vertices[(i + 1) % n];
        drawLineBresenham(static_cast<int>(std::round(p1.x)), static_cast<int>(std::round(p1.y)),
                          static_cast<int>(std::round(p2.x)), static_cast<int>(std::round(p2.y)),
                          boundaryColor);
    }
}

void Canvas::renderGL() const {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glRasterPos2i(0, 0);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
}

// -----------------------------------------------------------------------------
// 1. Thuật toán tô màu theo đường biên - Đệ quy (Recursive Boundary Fill)
// -----------------------------------------------------------------------------
void boundaryFillRecursive(Canvas& canvas, int x, int y, ColorRGB fillColor, ColorRGB boundaryColor, int& callCount, int maxDepth) {
    if (x < 0 || x >= canvas.width || y < 0 || y >= canvas.height)
        return;

    ColorRGB current = canvas.getPixel(x, y);
    if (current != boundaryColor && current != fillColor) {
        canvas.setPixel(x, y, fillColor);
        callCount++;
        if (callCount >= maxDepth) return; // Chống tràn stack quá sâu

        // 4 lân cận
        boundaryFillRecursive(canvas, x - 1, y,     fillColor, boundaryColor, callCount, maxDepth); // Trái
        boundaryFillRecursive(canvas, x + 1, y,     fillColor, boundaryColor, callCount, maxDepth); // Phải
        boundaryFillRecursive(canvas, x,     y + 1, fillColor, boundaryColor, callCount, maxDepth); // Trên
        boundaryFillRecursive(canvas, x,     y - 1, fillColor, boundaryColor, callCount, maxDepth); // Dưới
    }
}

// -----------------------------------------------------------------------------
// 2. Thuật toán tô màu theo đường biên - Khử đệ quy bằng Stack (Stack Boundary Fill)
// -----------------------------------------------------------------------------
void boundaryFillStack(Canvas& canvas, int startX, int startY, ColorRGB fillColor, ColorRGB boundaryColor) {
    if (startX < 0 || startX >= canvas.width || startY < 0 || startY >= canvas.height)
        return;

    ColorRGB initial = canvas.getPixel(startX, startY);
    if (initial == boundaryColor || initial == fillColor)
        return;

    std::vector<std::pair<int, int>> stack;
    stack.reserve(canvas.width * canvas.height / 4);

    canvas.setPixel(startX, startY, fillColor);
    stack.push_back({startX, startY});

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!stack.empty()) {
        std::pair<int, int> top = stack.back();
        stack.pop_back();

        int cx = top.first;
        int cy = top.second;

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < canvas.width && ny >= 0 && ny < canvas.height) {
                ColorRGB c = canvas.getPixel(nx, ny);
                if (c != boundaryColor && c != fillColor) {
                    canvas.setPixel(nx, ny, fillColor);
                    stack.push_back({nx, ny});
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// 3. Thuật toán tô màu đa giác theo dòng quét (Scanline Polygon Fill)
// -----------------------------------------------------------------------------
struct ScanlineEdge {
    int yMin;
    int yMax;
    double xCurrent; // hoành độ giao điểm tại yMin
    double invSlope; // 1/m = (x2 - x1) / (y2 - y1)
};

void scanlinePolygonFill(Canvas& canvas, const Polygon2D& poly, ColorRGB fillColor) {
    int n = static_cast<int>(poly.vertices.size());
    if (n < 3) return;

    // Tìm yMin và yMax của toàn bộ đa giác
    int yBottom = static_cast<int>(std::floor(poly.vertices[0].y));
    int yTop    = static_cast<int>(std::ceil(poly.vertices[0].y));

    for (int i = 1; i < n; ++i) {
        int vy = static_cast<int>(std::round(poly.vertices[i].y));
        if (vy < yBottom) yBottom = vy;
        if (vy > yTop)    yTop    = vy;
    }

    // Giới hạn trong kích thước canvas
    yBottom = std::max(0, yBottom);
    yTop    = std::min(canvas.height - 1, yTop);

    // Khởi tạo bảng các cạnh
    std::vector<ScanlineEdge> edges;
    for (int i = 0; i < n; ++i) {
        Point2D p1 = poly.vertices[i];
        Point2D p2 = poly.vertices[(i + 1) % n];

        int y1 = static_cast<int>(std::round(p1.y));
        int y2 = static_cast<int>(std::round(p2.y));

        // Cạnh song song dòng quét bỏ qua
        if (y1 == y2) continue;

        ScanlineEdge edge;
        if (y1 < y2) {
            edge.yMin = y1;
            edge.yMax = y2;
            edge.xCurrent = p1.x;
        } else {
            edge.yMin = y2;
            edge.yMax = y1;
            edge.xCurrent = p2.x;
        }
        edge.invSlope = (p2.x - p1.x) / static_cast<double>(p2.y - p1.y);
        edges.push_back(edge);
    }

    // Quét từng dòng từ yBottom đến yTop
    for (int y = yBottom; y <= yTop; ++y) {
        std::vector<double> intersections;

        // Tính giao điểm của dòng quét y với các cạnh kích hoạt (yMin <= y < yMax)
        for (const auto& e : edges) {
            if (y >= e.yMin && y < e.yMax) {
                double xIntersect = e.xCurrent + (y - e.yMin) * e.invSlope;
                intersections.push_back(xIntersect);
            }
        }

        std::sort(intersections.begin(), intersections.end());

        // Tô các đoạn giữa các cặp giao điểm (x0, x1), (x2, x3), ...
        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int xStart = static_cast<int>(std::ceil(intersections[i]));
            int xEnd   = static_cast<int>(std::floor(intersections[i + 1]));

            xStart = std::max(0, xStart);
            xEnd   = std::min(canvas.width - 1, xEnd);

            for (int x = xStart; x <= xEnd; ++x) {
                canvas.setPixel(x, y, fillColor);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Đọc đa giác từ file
// -----------------------------------------------------------------------------
bool loadPolygonFromFile(const std::string& filename, Polygon2D& poly) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int n;
    if (!(file >> n) || n < 3) return false;

    poly.clear();
    for (int i = 0; i < n; ++i) {
        double x, y;
        if (file >> x >> y) {
            poly.addVertex(x, y);
        } else {
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Sinh các đa giác mẫu trực quan
// -----------------------------------------------------------------------------
Polygon2D createStarPolygon(double cx, double cy, double rOuter, double rInner) {
    Polygon2D star;
    double step = M_PI / 5.0; // 10 đỉnh (5 cánh ngoài, 5 góc trong)
    for (int i = 0; i < 10; ++i) {
        double angle = i * step - M_PI / 2.0;
        double r = (i % 2 == 0) ? rOuter : rInner;
        star.addVertex(cx + r * std::cos(angle), cy - r * std::sin(angle));
    }
    return star;
}

Polygon2D createArrowPolygon(double cx, double cy, double length, double width) {
    Polygon2D arrow;
    double halfW = width * 0.5;
    double headW = width * 1.0;
    double bodyL = length * 0.6;
    double headL = length * 0.4;

    double leftX = cx - length * 0.5;
    double rightX = cx + length * 0.5;

    arrow.addVertex(leftX, cy - halfW);
    arrow.addVertex(leftX + bodyL, cy - halfW);
    arrow.addVertex(leftX + bodyL, cy - headW);
    arrow.addVertex(rightX, cy);
    arrow.addVertex(leftX + bodyL, cy + headW);
    arrow.addVertex(leftX + bodyL, cy + halfW);
    arrow.addVertex(leftX, cy + halfW);

    return arrow;
}

Polygon2D createConcavePolygon(double cx, double cy, double size) {
    Polygon2D poly;
    poly.addVertex(cx - size * 0.8, cy - size * 0.8);
    poly.addVertex(cx + size * 0.8, cy - size * 0.8);
    poly.addVertex(cx + size * 0.6, cy + size * 0.8);
    poly.addVertex(cx,              cy + size * 0.1); // Đỉnh lõm
    poly.addVertex(cx - size * 0.6, cy + size * 0.8);
    return poly;
}

// -----------------------------------------------------------------------------
// Chụp ảnh màn hình BMP
// -----------------------------------------------------------------------------
bool saveScreenshotBMP(const std::string& filename, int width, int height) {
    int rowPadded = (width * 3 + 3) & (~3);
    int dataSize = rowPadded * height;

    unsigned char fileHeader[14] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0, 0, 0,
        54, 0, 0, 0
    };
    int fileSize = 54 + dataSize;
    fileHeader[2] = static_cast<unsigned char>(fileSize);
    fileHeader[3] = static_cast<unsigned char>(fileSize >> 8);
    fileHeader[4] = static_cast<unsigned char>(fileSize >> 16);
    fileHeader[5] = static_cast<unsigned char>(fileSize >> 24);

    unsigned char infoHeader[40] = {
        40, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 0,
        24, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    infoHeader[4]  = static_cast<unsigned char>(width);
    infoHeader[5]  = static_cast<unsigned char>(width >> 8);
    infoHeader[6]  = static_cast<unsigned char>(width >> 16);
    infoHeader[7]  = static_cast<unsigned char>(width >> 24);

    infoHeader[8]  = static_cast<unsigned char>(height);
    infoHeader[9]  = static_cast<unsigned char>(height >> 8);
    infoHeader[10] = static_cast<unsigned char>(height >> 16);
    infoHeader[11] = static_cast<unsigned char>(height >> 24);

    infoHeader[20] = static_cast<unsigned char>(dataSize);
    infoHeader[21] = static_cast<unsigned char>(dataSize >> 8);
    infoHeader[22] = static_cast<unsigned char>(dataSize >> 16);
    infoHeader[23] = static_cast<unsigned char>(dataSize >> 24);

    std::vector<unsigned char> pixels(width * height * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Đổi định dạng RGB sang BGR cho định dạng chuẩn Windows BMP
    std::vector<unsigned char> rowBuffer(rowPadded, 0);
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) return false;

    fwrite(fileHeader, 1, 14, f);
    fwrite(infoHeader, 1, 40, f);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIdx = (y * width + x) * 3;
            rowBuffer[x * 3 + 0] = pixels[srcIdx + 2]; // B
            rowBuffer[x * 3 + 1] = pixels[srcIdx + 1]; // G
            rowBuffer[x * 3 + 2] = pixels[srcIdx + 0]; // R
        }
        fwrite(rowBuffer.data(), 1, rowPadded, f);
    }
    fclose(f);
    return true;
}

} // namespace Lab2
