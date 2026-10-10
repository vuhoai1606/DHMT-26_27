#include "Transform2D.h"
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Lab3 {

Matrix3x3::Matrix3x3() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

Matrix3x3 Matrix3x3::identity() {
    return Matrix3x3();
}

Matrix3x3 Matrix3x3::translation(double tx, double ty) {
    Matrix3x3 res = identity();
    res.m[0][2] = tx;
    res.m[1][2] = ty;
    return res;
}

Matrix3x3 Matrix3x3::scaling(double sx, double sy, double x0, double y0) {
    Matrix3x3 res = identity();
    res.m[0][0] = sx;
    res.m[0][2] = x0 * (1.0 - sx);

    res.m[1][1] = sy;
    res.m[1][2] = y0 * (1.0 - sy);

    return res;
}

Matrix3x3 Matrix3x3::rotation(double angleDeg, double x0, double y0) {
    double rad = angleDeg * M_PI / 180.0;
    double c = std::cos(rad);
    double s = std::sin(rad);

    Matrix3x3 res = identity();
    res.m[0][0] = c;
    res.m[0][1] = -s;
    res.m[0][2] = x0 * (1.0 - c) + y0 * s;

    res.m[1][0] = s;
    res.m[1][1] = c;
    res.m[1][2] = y0 * (1.0 - c) - x0 * s;

    return res;
}

Matrix3x3 Matrix3x3::reflection(double a, double b) {
    double den = 1.0 + a * a;
    Matrix3x3 res = identity();

    res.m[0][0] = (1.0 - a * a) / den;
    res.m[0][1] = (2.0 * a) / den;
    res.m[0][2] = (-2.0 * a * b) / den;

    res.m[1][0] = (2.0 * a) / den;
    res.m[1][1] = (a * a - 1.0) / den;
    res.m[1][2] = (2.0 * b) / den;

    return res;
}

Matrix3x3 Matrix3x3::reflectionVertical(double c) {
    Matrix3x3 res = identity();
    res.m[0][0] = -1.0;
    res.m[0][2] = 2.0 * c;
    return res;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other) const {
    Matrix3x3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += m[i][k] * other.m[k][j];
            }
            res.m[i][j] = sum;
        }
    }
    return res;
}

Point2D Matrix3x3::transformPoint(const Point2D& p) const {
    double x = m[0][0] * p.x + m[0][1] * p.y + m[0][2];
    double y = m[1][0] * p.x + m[1][1] * p.y + m[1][2];
    double w = m[2][0] * p.x + m[2][1] * p.y + m[2][2];

    if (std::abs(w - 1.0) > 1e-9 && std::abs(w) > 1e-9) {
        x /= w;
        y /= w;
    }
    return Point2D(x, y);
}

void Matrix3x3::print(const std::string& title) const {
    if (!title.empty()) {
        std::cout << "\n=== " << title << " ===\n";
    }
    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < 3; ++i) {
        std::cout << "[ ";
        for (int j = 0; j < 3; ++j) {
            std::cout << std::setw(10) << m[i][j] << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "======================================\n";
}

// -----------------------------------------------------------------------------
// Polygon2D implementation
// -----------------------------------------------------------------------------
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

void Polygon2D::applyTransform(const Matrix3x3& mat) {
    for (auto& v : vertices) {
        v = mat.transformPoint(v);
    }
}

void Polygon2D::fitToWindow(double winW, double winH, double margin) {
    if (vertices.empty()) return;

    double minX, minY, maxX, maxY;
    getBoundingBox(minX, minY, maxX, maxY);

    double curW = maxX - minX;
    double curH = maxY - minY;
    if (curW < 1.0) curW = 1.0;
    if (curH < 1.0) curH = 1.0;

    double availW = winW - 2.0 * margin;
    double availH = winH - 2.0 * margin;

    double scaleX = availW / curW;
    double scaleY = availH / curH;
    double s = std::min(scaleX, scaleY);

    Point2D c = getCentroid();
    Point2D target(winW * 0.5, winH * 0.5);

    // Dịch về gốc -> Co dãn -> Dịch về tâm cửa sổ
    Matrix3x3 T1 = Matrix3x3::translation(-c.x, -c.y);
    Matrix3x3 S  = Matrix3x3::scaling(s, s, 0, 0);
    Matrix3x3 T2 = Matrix3x3::translation(target.x, target.y);

    Matrix3x3 composite = T2 * (S * T1);
    applyTransform(composite);
}

// -----------------------------------------------------------------------------
// Đọc file đa giác
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
// Tạo các hình mẫu trực quan
// -----------------------------------------------------------------------------
Polygon2D createStarPolygon(double cx, double cy, double rOuter, double rInner) {
    Polygon2D star;
    double step = M_PI / 5.0;
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

Polygon2D createHousePolygon(double cx, double cy, double size) {
    Polygon2D house;
    double half = size * 0.5;
    // Hình ngôi nhà: Đáy chữ nhật + Mái tam giác
    house.addVertex(cx - half, cy - half);
    house.addVertex(cx + half, cy - half);
    house.addVertex(cx + half, cy + half * 0.4);
    house.addVertex(cx,        cy + half); // Đỉnh mái
    house.addVertex(cx - half, cy + half * 0.4);
    return house;
}

// -----------------------------------------------------------------------------
// Xuất ảnh BMP
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

} // namespace Lab3
