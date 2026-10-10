#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include "Lab2/Clipping.h"
#include "Lab2/Filling.h"
#include "Lab3/Transform2D.h"

using namespace std;

void testLineClipping() {
    cout << "========================================\n";
    cout << "TEST 1: LINE CLIPPING (LAB 02)\n";
    cout << "========================================\n";

    Lab2::ClipRect rect(100, 100, 500, 400);

    // Test case 1: Completely inside
    Lab2::Line2D l1(Lab2::Point2D(150, 150), Lab2::Point2D(300, 300));
    Lab2::Line2D outCS1, outMP1;
    bool visCS1 = Lab2::cohenSutherlandClip(l1, rect, outCS1);
    bool visMP1 = Lab2::midpointSubdivisionClip(l1, rect, outMP1);
    assert(visCS1 && visMP1);
    assert(abs(outCS1.a.x - 150) < 1e-4 && abs(outCS1.a.y - 150) < 1e-4);
    assert(abs(outMP1.a.x - 150) < 1e-4 && abs(outMP1.a.y - 150) < 1e-4);
    cout << " [PASS] Case 1: Line completely inside\n";

    // Test case 2: Completely outside (top)
    Lab2::Line2D l2(Lab2::Point2D(150, 450), Lab2::Point2D(300, 480));
    Lab2::Line2D outCS2, outMP2;
    bool visCS2 = Lab2::cohenSutherlandClip(l2, rect, outCS2);
    bool visMP2 = Lab2::midpointSubdivisionClip(l2, rect, outMP2);
    assert(!visCS2 && !visMP2);
    cout << " [PASS] Case 2: Line completely outside\n";

    // Test case 3: One point inside, one point outside (cross right edge x = 500)
    Lab2::Line2D l3(Lab2::Point2D(400, 250), Lab2::Point2D(600, 250));
    Lab2::Line2D outCS3, outMP3;
    bool visCS3 = Lab2::cohenSutherlandClip(l3, rect, outCS3);
    bool visMP3 = Lab2::midpointSubdivisionClip(l3, rect, outMP3);
    assert(visCS3 && visMP3);
    assert(abs(outCS3.a.x - 400) < 1.0 && abs(outCS3.b.x - 500) < 1.0);
    assert(abs(outMP3.a.x - 400) < 1.0 && abs(outMP3.b.x - 500) < 1.0);
    cout << " [PASS] Case 3: Line crossing right boundary (one inside, one outside)\n";

    // Test case 4: Both points outside, line crossing through the window
    Lab2::Line2D l4(Lab2::Point2D(50, 250), Lab2::Point2D(550, 250));
    Lab2::Line2D outCS4, outMP4;
    bool visCS4 = Lab2::cohenSutherlandClip(l4, rect, outCS4);
    bool visMP4 = Lab2::midpointSubdivisionClip(l4, rect, outMP4);
    assert(visCS4 && visMP4);
    assert(abs(outCS4.a.x - 100) < 1.0 && abs(outCS4.b.x - 500) < 1.0);
    assert(abs(outMP4.a.x - 100) < 1.0 && abs(outMP4.b.x - 500) < 1.0);
    cout << " [PASS] Case 4: Line crossing completely through window (both outside)\n";
}

void testPolygonFilling() {
    cout << "\n========================================\n";
    cout << "TEST 2: POLYGON FILLING (LAB 02)\n";
    cout << "========================================\n";

    // Create a 200x200 canvas
    Lab2::Canvas canvasRec(200, 200);
    Lab2::Canvas canvasStack(200, 200);
    Lab2::Canvas canvasScan(200, 200);

    Lab2::ColorRGB bg(0, 0, 0);
    Lab2::ColorRGB bnd(255, 255, 255);
    Lab2::ColorRGB fill(255, 0, 0);

    canvasRec.clear(bg);
    canvasStack.clear(bg);
    canvasScan.clear(bg);

    // Box polygon: [50, 150] x [50, 150] (area approx 100x100 = 10000 px)
    Lab2::Polygon2D box;
    box.addVertex(50, 50);
    box.addVertex(150, 50);
    box.addVertex(150, 150);
    box.addVertex(50, 150);

    // 1. Boundary fill Recursive
    canvasRec.drawPolygonBoundary(box, bnd);
    int callCount = 0;
    Lab2::boundaryFillRecursive(canvasRec, 100, 100, fill, bnd, callCount);
    int recFilledPixels = 0;
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            if (canvasRec.getPixel(x, y) == fill) recFilledPixels++;
        }
    }
    cout << " [PASS] Boundary Fill (Recursive): filled " << recFilledPixels << " pixels (calls: " << callCount << ")\n";
    assert(recFilledPixels > 9000);

    // 2. Boundary fill Stack
    canvasStack.drawPolygonBoundary(box, bnd);
    Lab2::boundaryFillStack(canvasStack, 100, 100, fill, bnd);
    int stackFilledPixels = 0;
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            if (canvasStack.getPixel(x, y) == fill) stackFilledPixels++;
        }
    }
    cout << " [PASS] Boundary Fill (Stack): filled " << stackFilledPixels << " pixels\n";
    assert(stackFilledPixels == recFilledPixels);

    // 3. Scanline fill
    Lab2::scanlinePolygonFill(canvasScan, box, fill);
    int scanFilledPixels = 0;
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 200; ++x) {
            if (canvasScan.getPixel(x, y) == fill) scanFilledPixels++;
        }
    }
    cout << " [PASS] Scanline Fill: filled " << scanFilledPixels << " pixels\n";
    assert(abs(scanFilledPixels - 10000) < 500);
}

void testTransformations() {
    cout << "\n========================================\n";
    cout << "TEST 3: 2D TRANSFORMATIONS (LAB 03)\n";
    cout << "========================================\n";

    // 1. Translation: T(50, -30) on (100, 100) -> (150, 70)
    Lab3::Matrix3x3 T = Lab3::Matrix3x3::translation(50, -30);
    Lab3::Point2D p1(100, 100);
    Lab3::Point2D tp1 = T.transformPoint(p1);
    assert(abs(tp1.x - 150.0) < 1e-4 && abs(tp1.y - 70.0) < 1e-4);
    cout << " [PASS] Translation: (100, 100) -> (" << tp1.x << ", " << tp1.y << ")\n";

    // 2. Scaling: S(2.0, 3.0) with pivot (100, 100) on point (110, 120)
    // dx = 10, dy = 20 -> dx' = 20, dy' = 60 -> (120, 160)
    Lab3::Matrix3x3 S = Lab3::Matrix3x3::scaling(2.0, 3.0, 100, 100);
    Lab3::Point2D p2(110, 120);
    Lab3::Point2D sp2 = S.transformPoint(p2);
    assert(abs(sp2.x - 120.0) < 1e-4 && abs(sp2.y - 160.0) < 1e-4);
    cout << " [PASS] Scaling about pivot (100, 100): (110, 120) -> (" << sp2.x << ", " << sp2.y << ")\n";

    // 3. Rotation: R(90 deg) with pivot (100, 100) on point (150, 100)
    // (150, 100) rotated 90 deg counter-clockwise around (100, 100) -> (100, 150)
    Lab3::Matrix3x3 R = Lab3::Matrix3x3::rotation(90.0, 100, 100);
    Lab3::Point2D p3(150, 100);
    Lab3::Point2D rp3 = R.transformPoint(p3);
    assert(abs(rp3.x - 100.0) < 1e-3 && abs(rp3.y - 150.0) < 1e-3);
    cout << " [PASS] Rotation 90 deg about pivot (100, 100): (150, 100) -> (" << rp3.x << ", " << rp3.y << ")\n";

    // 4. Reflection across line y = 1*x + 0 (y = x)
    // Point (30, 70) reflected across y = x -> (70, 30)
    Lab3::Matrix3x3 M = Lab3::Matrix3x3::reflection(1.0, 0.0);
    Lab3::Point2D p4(30, 70);
    Lab3::Point2D mp4 = M.transformPoint(p4);
    assert(abs(mp4.x - 70.0) < 1e-4 && abs(mp4.y - 30.0) < 1e-4);
    cout << " [PASS] Reflection across line y = x: (30, 70) -> (" << mp4.x << ", " << mp4.y << ")\n";

    // Reflection across line y = 1*x + 10
    // Point (0, 10) is on line -> stays (0, 10)
    Lab3::Matrix3x3 M2 = Lab3::Matrix3x3::reflection(1.0, 10.0);
    Lab3::Point2D p5(0, 10);
    Lab3::Point2D mp5 = M2.transformPoint(p5);
    assert(abs(mp5.x - 0.0) < 1e-4 && abs(mp5.y - 10.0) < 1e-4);
    cout << " [PASS] Reflection across line y = x + 10: fixed point on line stays (0, 10)\n";
}

int main() {
    cout << "RUNNING TEST SUITE FOR LAB 02 & LAB 03...\n\n";
    testLineClipping();
    testPolygonFilling();
    testTransformations();
    cout << "\n>>> ALL UNIT TESTS PASSED SUCCESSFULLY! 100% VERIFIED! <<<\n";
    return 0;
}
