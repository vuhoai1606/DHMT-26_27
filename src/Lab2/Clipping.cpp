#include "Clipping.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace Lab2 {

int computeOutCode(const Point2D& p, const ClipRect& r) {
    int code = INSIDE;
    if (p.x < r.xmin)
        code |= LEFT;
    else if (p.x > r.xmax)
        code |= RIGHT;

    if (p.y < r.ymin)
        code |= BOTTOM;
    else if (p.y > r.ymax)
        code |= TOP;

    return code;
}

// -----------------------------------------------------------------------------
// 1. Thuật toán Cohen-Sutherland
// -----------------------------------------------------------------------------
bool cohenSutherlandClip(Line2D inLine, const ClipRect& r, Line2D& outLine) {
    double x0 = inLine.a.x;
    double y0 = inLine.a.y;
    double x1 = inLine.b.x;
    double y1 = inLine.b.y;

    int code0 = computeOutCode(Point2D(x0, y0), r);
    int code1 = computeOutCode(Point2D(x1, y1), r);

    bool accept = false;

    while (true) {
        if ((code0 | code1) == 0) {
            // Trivial accept: Cả 2 điểm đều nằm bên trong cửa sổ xén
            accept = true;
            break;
        } else if ((code0 & code1) != 0) {
            // Trivial reject: Cả 2 điểm cùng nằm về một phía bên ngoài cửa sổ
            break;
        } else {
            // Cần xén: Chọn một điểm nằm bên ngoài
            int codeOut = (code0 != 0) ? code0 : code1;
            double x = 0.0, y = 0.0;

            // Tìm giao điểm với đường biên tương ứng
            if (codeOut & TOP) {
                // Điểm nằm phía trên cửa sổ (y = ymax)
                x = x0 + (x1 - x0) * (r.ymax - y0) / (y1 - y0);
                y = r.ymax;
            } else if (codeOut & BOTTOM) {
                // Điểm nằm phía dưới cửa sổ (y = ymin)
                x = x0 + (x1 - x0) * (r.ymin - y0) / (y1 - y0);
                y = r.ymin;
            } else if (codeOut & RIGHT) {
                // Điểm nằm bên phải cửa sổ (x = xmax)
                y = y0 + (y1 - y0) * (r.xmax - x0) / (x1 - x0);
                x = r.xmax;
            } else if (codeOut & LEFT) {
                // Điểm nằm bên trái cửa sổ (x = xmin)
                y = y0 + (y1 - y0) * (r.xmin - x0) / (x1 - x0);
                x = r.xmin;
            }

            // Thay thế điểm nằm ngoài bằng giao điểm vừa tìm được và cập nhật OutCode
            if (codeOut == code0) {
                x0 = x;
                y0 = y;
                code0 = computeOutCode(Point2D(x0, y0), r);
            } else {
                x1 = x;
                y1 = y;
                code1 = computeOutCode(Point2D(x1, y1), r);
            }
        }
    }

    if (accept) {
        outLine.a = Point2D(x0, y0);
        outLine.b = Point2D(x1, y1);
        outLine.isClipped = true;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// 2. Thuật toán xén chia nhị phân (Midpoint Subdivision / Binary Clipping)
// -----------------------------------------------------------------------------
// Hàm tìm điểm biên giữa điểm P (bên trong) và điểm Q (bên ngoài)
static Point2D binaryFindIntersection(Point2D P, Point2D Q, const ClipRect& r, double eps = 0.5) {
    // P có Code == 0 (trong), Q có Code != 0 (ngoài)
    while ((std::abs(P.x - Q.x) + std::abs(P.y - Q.y)) > eps) {
        Point2D M((P.x + Q.x) * 0.5, (P.y + Q.y) * 0.5);
        if (computeOutCode(M, r) == INSIDE) {
            P = M; // M nằm trong, thu hẹp về phía M
        } else {
            Q = M; // M nằm ngoài, thu hẹp về phía M
        }
    }
    return P;
}

bool midpointSubdivisionClip(Line2D inLine, const ClipRect& r, Line2D& outLine) {
    Point2D A = inLine.a;
    Point2D B = inLine.b;

    int codeA = computeOutCode(A, r);
    int codeB = computeOutCode(B, r);

    // TH 1: Cả hai điểm đều bên trong
    if ((codeA | codeB) == 0) {
        outLine.a = A;
        outLine.b = B;
        outLine.isClipped = true;
        return true;
    }

    // TH 2: Cả hai điểm cùng nằm ngoài cùng 1 phía
    if ((codeA & codeB) != 0) {
        return false;
    }

    // TH 3: Một điểm trong, một điểm ngoài
    if (codeA == INSIDE && codeB != INSIDE) {
        Point2D P = binaryFindIntersection(A, B, r);
        outLine.a = A;
        outLine.b = P;
        outLine.isClipped = true;
        return true;
    }
    if (codeA != INSIDE && codeB == INSIDE) {
        Point2D P = binaryFindIntersection(B, A, r);
        outLine.a = P;
        outLine.b = B;
        outLine.isClipped = true;
        return true;
    }

    // TH 4: Cả 2 điểm đều ở ngoài, nhưng có thể đoạn thẳng cắt xuyên qua cửa sổ
    // Sử dụng thuật toán nhị phân tìm xem có điểm M nào trên đoạn thẳng nằm trong cửa sổ không
    Point2D P = A;
    Point2D Q = B;
    Point2D M((P.x + Q.x) * 0.5, (P.y + Q.y) * 0.5);

    int maxIter = 100;
    while (computeOutCode(M, r) != INSIDE && ((std::abs(P.x - Q.x) + std::abs(P.y - Q.y)) > 0.5) && maxIter-- > 0) {
        if ((computeOutCode(P, r) & computeOutCode(M, r)) != 0) {
            P = M;
        } else {
            Q = M;
        }
        M = Point2D((P.x + Q.x) * 0.5, (P.y + Q.y) * 0.5);
    }

    // Nếu tìm thấy một điểm M nằm bên trong cửa sổ
    if (computeOutCode(M, r) == INSIDE) {
        // Giao điểm 1: giữa M (trong) và A (ngoài)
        Point2D P1 = binaryFindIntersection(M, A, r);
        // Giao điểm 2: giữa M (trong) và B (ngoài)
        Point2D P2 = binaryFindIntersection(M, B, r);

        outLine.a = P1;
        outLine.b = P2;
        outLine.isClipped = true;
        return true;
    }

    // Không có điểm nào nằm trong cửa sổ
    return false;
}

} // namespace Lab2
