#ifndef LAB3_TRANSFORM2D_H
#define LAB3_TRANSFORM2D_H

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace Lab3 {

struct Point2D {
    double x, y;
    Point2D(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

// Lớp Ma trận thuần nhất 3x3 cho các phép biến đổi 2D (Homogeneous Coordinates)
class Matrix3x3 {
public:
    double m[3][3];

    Matrix3x3(); // Khởi tạo ma trận đơn vị

    static Matrix3x3 identity();

    // 1. Ma trận tịnh tiến (Translation): T(tx, ty)
    static Matrix3x3 translation(double tx, double ty);

    // 2. Ma trận tỉ lệ với tâm tỉ lệ (x0, y0): S(sx, sy, x0, y0)
    static Matrix3x3 scaling(double sx, double sy, double x0, double y0);

    // 3. Ma trận quay với tâm quay (x0, y0): R(angleDeg, x0, y0)
    static Matrix3x3 rotation(double angleDeg, double x0, double y0);

    // 4. Ma trận đối xứng qua đường thẳng y = a*x + b
    static Matrix3x3 reflection(double a, double b);

    // Ma trận đối xứng qua đường thẳng đứng x = c
    static Matrix3x3 reflectionVertical(double c);

    // Nhân hai ma trận 3x3
    Matrix3x3 operator*(const Matrix3x3& other) const;

    // Biến đổi một điểm 2D
    Point2D transformPoint(const Point2D& p) const;

    // In ma trận ra console
    void print(const std::string& title = "") const;
};

// Cấu trúc đa giác 2D
struct Polygon2D {
    std::vector<Point2D> vertices;

    void clear() { vertices.clear(); }
    void addVertex(double x, double y) { vertices.push_back(Point2D(x, y)); }
    Point2D getCentroid() const;
    void getBoundingBox(double& minX, double& minY, double& maxX, double& maxY) const;

    // Áp dụng ma trận biến đổi lên tất cả các đỉnh
    void applyTransform(const Matrix3x3& mat);

    // Căn chỉnh đa giác vừa vặn trong cửa sổ hiển thị
    void fitToWindow(double winW, double winH, double margin = 80.0);
};

// Đọc đa giác từ file văn bản (.txt)
bool loadPolygonFromFile(const std::string& filename, Polygon2D& poly);

// Tạo các đa giác mẫu trực quan
Polygon2D createStarPolygon(double cx, double cy, double rOuter, double rInner);
Polygon2D createArrowPolygon(double cx, double cy, double length, double width);
Polygon2D createHousePolygon(double cx, double cy, double size);

// Chụp ảnh màn hình BMP
bool saveScreenshotBMP(const std::string& filename, int width, int height);

} // namespace Lab3

#endif // LAB3_TRANSFORM2D_H
