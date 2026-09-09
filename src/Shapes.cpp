#include "Shapes.h"
#include <cmath>
#include <initializer_list>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Shapes {

    // =========================================================================
    // HỆ THỐNG CHIẾU SÁNG & TRỤC TỌA ĐỘ
    // =========================================================================
    void setupLighting() {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat lightPos[]      = { 6.0f, 9.0f, 7.0f, 1.0f };
        GLfloat lightAmbient[]  = { 0.35f, 0.35f, 0.38f, 1.0f };
        GLfloat lightDiffuse[]  = { 0.90f, 0.90f, 0.90f, 1.0f };
        GLfloat lightSpecular[] = { 1.00f, 1.00f, 1.00f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);
    }

    void disableLighting() {
        glDisable(GL_LIGHTING);
    }

    void drawAxes3D(float length) {
        glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT);
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);

        glBegin(GL_LINES);
            // Trục X - Đỏ
            glColor3f(1.0f, 0.2f, 0.2f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(length, 0.0f, 0.0f);

            // Trục Y - Xanh lá
            glColor3f(0.2f, 1.0f, 0.2f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, length, 0.0f);

            // Trục Z - Xanh dương
            glColor3f(0.3f, 0.6f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, length);
        glEnd();

        glPopAttrib();
    }


    // =========================================================================
    // CÁC HÀM HÌNH HỌC CƠ BẢN DÙNG ĐỂ LẮP RÁP VẬT THỂ
    // =========================================================================
    static void drawBox(float width, float height, float length) {
        float w = width / 2.0f;
        float h = height / 2.0f;
        float l = length / 2.0f;

        glBegin(GL_QUADS);
            // Mặt trước (+Z)
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-w, -h,  l); glVertex3f( w, -h,  l);
            glVertex3f( w,  h,  l); glVertex3f(-w,  h,  l);

            // Mặt sau (-Z)
            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f(-w, -h, -l); glVertex3f(-w,  h, -l);
            glVertex3f( w,  h, -l); glVertex3f( w, -h, -l);

            // Mặt trên (+Y)
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-w,  h, -l); glVertex3f(-w,  h,  l);
            glVertex3f( w,  h,  l); glVertex3f( w,  h, -l);

            // Mặt dưới (-Y)
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(-w, -h, -l); glVertex3f( w, -h, -l);
            glVertex3f( w, -h,  l); glVertex3f(-w, -h,  l);

            // Mặt phải (+X)
            glNormal3f(1.0f, 0.0f, 0.0f);
            glVertex3f( w, -h, -l); glVertex3f( w,  h, -l);
            glVertex3f( w,  h,  l); glVertex3f( w, -h,  l);

            // Mặt trái (-X)
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glVertex3f(-w, -h, -l); glVertex3f(-w, -h,  l);
            glVertex3f(-w,  h,  l); glVertex3f(-w,  h, -l);
        glEnd();
    }

    static void drawCylinderZ(float radius, float length, int slices = 20) {
        float halfL = length / 2.0f;

        // Thân hình trụ quay dọc trục Z
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            float angle = i * 2.0f * (float)M_PI / slices;
            float c = cos(angle);
            float s = sin(angle);
            glNormal3f(c, s, 0.0f);
            glVertex3f(radius * c, radius * s, -halfL);
            glVertex3f(radius * c, radius * s,  halfL);
        }
        glEnd();

        // Nắp trước (+Z)
        glBegin(GL_TRIANGLE_FAN);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, halfL);
            for (int i = 0; i <= slices; i++) {
                float angle = i * 2.0f * (float)M_PI / slices;
                glVertex3f(radius * cos(angle), radius * sin(angle), halfL);
            }
        glEnd();

        // Nắp sau (-Z)
        glBegin(GL_TRIANGLE_FAN);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f(0.0f, 0.0f, -halfL);
            for (int i = slices; i >= 0; i--) {
                float angle = i * 2.0f * (float)M_PI / slices;
                glVertex3f(radius * cos(angle), radius * sin(angle), -halfL);
            }
        glEnd();
    }

    static void drawSphere3D(float radius, int slices = 20, int stacks = 20) {
        for (int i = 0; i <= stacks; ++i) {
            float lat0 = (float)M_PI * (-0.5f + (float)(i - 1) / stacks);
            float z0  = radius * sin(lat0);
            float zr0 = radius * cos(lat0);

            float lat1 = (float)M_PI * (-0.5f + (float)i / stacks);
            float z1  = radius * sin(lat1);
            float zr1 = radius * cos(lat1);

            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; ++j) {
                float lng = 2.0f * (float)M_PI * (float)j / slices;
                float x = cos(lng);
                float y = sin(lng);

                glNormal3f(x * cos(lat0), y * cos(lat0), sin(lat0));
                glVertex3f(x * zr0, y * zr0, z0);

                glNormal3f(x * cos(lat1), y * cos(lat1), sin(lat1));
                glVertex3f(x * zr1, y * zr1, z1);
            }
            glEnd();
        }
    }


    // =========================================================================
    // 1. BỨC TRANH 2D HOÀN CHỈNH: PHONG CẢNH ĐỒNG QUÊ & CỐI XAY GIÓ CHUYỂN ĐỘNG
    // =========================================================================
    void render2DLandscape(float time) {
        glDisable(GL_LIGHTING);

        // 1.1 BẦU TRỜI XANH (Nội suy màu từ xanh đậm xuống xanh dịu)
        glBegin(GL_QUADS);
            glColor3f(0.35f, 0.65f, 0.95f); // Trên cao
            glVertex2f(-500.0f,  350.0f);
            glVertex2f( 500.0f,  350.0f);
            glColor3f(0.70f, 0.88f, 0.98f); // Chân trời
            glVertex2f( 500.0f, -40.0f);
            glVertex2f(-500.0f, -40.0f);
        glEnd();

        // 1.2 MẶT TRỜI VÀNG TỎA NẮNG
        glPushMatrix();
            glTranslatef(-320.0f, 220.0f, 0.0f);

            // Quầng sáng / Vầng mặt trời tròn
            glColor3f(1.0f, 0.85f, 0.15f);
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.0f, 0.0f);
                for (int i = 0; i <= 36; i++) {
                    float angle = i * 2.0f * (float)M_PI / 36.0f;
                    glVertex2f(45.0f * cos(angle), 45.0f * sin(angle));
                }
            glEnd();

            // Các tia nắng xoay chậm quanh mặt trời
            glRotatef(time * 25.0f, 0.0f, 0.0f, 1.0f);
            glLineWidth(2.5f);
            glColor3f(1.0f, 0.75f, 0.1f);
            glBegin(GL_LINES);
                for (int i = 0; i < 12; i++) {
                    float angle = i * (2.0f * (float)M_PI / 12.0f);
                    glVertex2f(55.0f * cos(angle), 55.0f * sin(angle));
                    glVertex2f(75.0f * cos(angle), 75.0f * sin(angle));
                }
            glEnd();
        glPopMatrix();

        // 1.3 CÁC ĐÁM MÂY TRẮNG BỒNG BỀNH
        auto drawCloud = [](float cx, float cy, float scale) {
            glColor3f(1.0f, 1.0f, 1.0f);
            float circles[][3] = {
                { 0.0f,  0.0f, 32.0f },
                { 25.0f, 10.0f, 28.0f },
                { 50.0f,  0.0f, 26.0f },
                {-25.0f,  5.0f, 26.0f }
            };
            for (auto& c : circles) {
                glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(cx + c[0] * scale, cy + c[1] * scale);
                    for (int i = 0; i <= 24; i++) {
                        float angle = i * 2.0f * (float)M_PI / 24.0f;
                        glVertex2f(cx + (c[0] + c[2] * cos(angle)) * scale,
                                   cy + (c[1] + c[2] * sin(angle)) * scale);
                    }
                glEnd();
            }
        };

        // Đám mây trôi nhẹ theo thời gian
        drawCloud(100.0f + 20.0f * sin(time * 0.3f), 240.0f, 1.0f);
        drawCloud(-100.0f + 15.0f * cos(time * 0.25f), 260.0f, 0.75f);
        drawCloud(320.0f + 18.0f * sin(time * 0.2f), 210.0f, 0.85f);

        // 1.4 ĐỒI CỎ VÀ MẶT ĐẤT XANH MƯỢT
        glBegin(GL_QUADS);
            // Đồi thoai thoải phía sau
            glColor3f(0.28f, 0.65f, 0.28f);
            glVertex2f(-500.0f, -40.0f);
            glVertex2f( 500.0f, -40.0f);
            // Bãi cỏ tươi phía trước
            glColor3f(0.18f, 0.52f, 0.18f);
            glVertex2f( 500.0f, -350.0f);
            glVertex2f(-500.0f, -350.0f);
        glEnd();

        // 1.5 CON ĐƯỜNG ĐẤT UỐN LƯỢN DẪN VÀO NHÀ
        glColor3f(0.82f, 0.68f, 0.45f);
        glBegin(GL_QUAD_STRIP);
            glVertex2f(-160.0f, -140.0f); glVertex2f(-120.0f, -140.0f);
            glVertex2f(-140.0f, -190.0f); glVertex2f( -90.0f, -190.0f);
            glVertex2f(-100.0f, -250.0f); glVertex2f( -40.0f, -250.0f);
            glVertex2f( -60.0f, -350.0f); glVertex2f(  20.0f, -350.0f);
        glEnd();

        // 1.6 NGÔI NHÀ 2D HOÀN CHỈNH
        // Thân nhà (Hình chữ nhật màu kem)
        glColor3f(0.98f, 0.92f, 0.75f);
        glBegin(GL_QUADS);
            glVertex2f(-240.0f, -140.0f);
            glVertex2f( -80.0f, -140.0f);
            glVertex2f( -80.0f,   10.0f);
            glVertex2f(-240.0f,   10.0f);
        glEnd();

        // Viền thân nhà
        glColor3f(0.70f, 0.60f, 0.45f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(-240.0f, -140.0f);
            glVertex2f( -80.0f, -140.0f);
            glVertex2f( -80.0f,   10.0f);
            glVertex2f(-240.0f,   10.0f);
        glEnd();

        // Ống khói trên mái nhà
        glColor3f(0.75f, 0.25f, 0.20f);
        glBegin(GL_QUADS);
            glVertex2f(-215.0f,  30.0f);
            glVertex2f(-190.0f,  30.0f);
            glVertex2f(-190.0f,  95.0f);
            glVertex2f(-215.0f,  95.0f);
        glEnd();

        // Làn khói bay từ ống khói
        glColor3f(0.85f, 0.88f, 0.90f);
        for (int i = 0; i < 4; i++) {
            float sy = 105.0f + i * 16.0f;
            float sx = -202.0f + sin(time * 2.0f + i) * 8.0f;
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(sx, sy);
                for (int j = 0; j <= 16; j++) {
                    float a = j * 2.0f * (float)M_PI / 16.0f;
                    glVertex2f(sx + (6.0f + i * 2.0f) * cos(a), sy + (6.0f + i * 2.0f) * sin(a));
                }
            glEnd();
        }

        // Mái nhà (Tam giác ngói đỏ)
        glColor3f(0.88f, 0.22f, 0.18f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-260.0f,  10.0f);
            glVertex2f( -60.0f,  10.0f);
            glVertex2f(-160.0f, 105.0f);
        glEnd();

        // Cửa chính (Gỗ nâu) & Tay nắm cửa
        glColor3f(0.48f, 0.24f, 0.10f);
        glBegin(GL_QUADS);
            glVertex2f(-180.0f, -140.0f);
            glVertex2f(-135.0f, -140.0f);
            glVertex2f(-135.0f,  -55.0f);
            glVertex2f(-180.0f,  -55.0f);
        glEnd();
        // Tay nắm cửa mạ vàng
        glColor3f(1.0f, 0.85f, 0.2f);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
            glVertex2f(-142.0f, -98.0f);
        glEnd();

        // Cửa sổ (Kính xanh nhạt có khung chia 4 ô)
        glColor3f(0.35f, 0.75f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(-225.0f, -65.0f);
            glVertex2f(-195.0f, -65.0f);
            glVertex2f(-195.0f, -30.0f);
            glVertex2f(-225.0f, -30.0f);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f(-210.0f, -65.0f); glVertex2f(-210.0f, -30.0f); // Thanh dọc
            glVertex2f(-225.0f, -47.5f); glVertex2f(-195.0f, -47.5f); // Thanh ngang
        glEnd();

        // 1.7 CÂY TÁO SUM SUÊ
        // Thân cây (Nâu)
        glColor3f(0.50f, 0.28f, 0.10f);
        glBegin(GL_QUADS);
            glVertex2f(-340.0f, -140.0f);
            glVertex2f(-310.0f, -140.0f);
            glVertex2f(-315.0f,  -10.0f);
            glVertex2f(-335.0f,  -10.0f);
        glEnd();

        // Tán lá xanh tròn
        auto drawFoliage = [](float cx, float cy, float r) {
            glColor3f(0.15f, 0.65f, 0.22f);
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(cx, cy);
                for (int i = 0; i <= 24; i++) {
                    float a = i * 2.0f * (float)M_PI / 24.0f;
                    glVertex2f(cx + r * cos(a), cy + r * sin(a));
                }
            glEnd();
        };
        drawFoliage(-325.0f,  30.0f, 50.0f);
        drawFoliage(-355.0f,  -5.0f, 40.0f);
        drawFoliage(-295.0f,  -5.0f, 40.0f);

        // Vài quả táo đỏ trên cây
        glColor3f(0.95f, 0.15f, 0.15f);
        float apples[][2] = { {-320, 40}, {-345, 10}, {-305, 15}, {-330, -10}, {-325, 15} };
        for (auto& ap : apples) {
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(ap[0], ap[1]);
                for (int i = 0; i <= 12; i++) {
                    float a = i * 2.0f * (float)M_PI / 12.0f;
                    glVertex2f(ap[0] + 5.0f * cos(a), ap[1] + 5.0f * sin(a));
                }
            glEnd();
        }

        // 1.8 CHIẾC CỐI XAY GIÓ CÓ CÁNH QUẠT XOAY (WINDMILL)
        // Tháp cối xay gió hình thang
        glColor3f(0.85f, 0.85f, 0.88f);
        glBegin(GL_QUADS);
            glVertex2f( 130.0f, -140.0f);
            glVertex2f( 230.0f, -140.0f);
            glVertex2f( 205.0f,   40.0f);
            glVertex2f( 155.0f,   40.0f);
        glEnd();
        // Mái vòm cối xay gió
        glColor3f(0.35f, 0.40f, 0.50f);
        glBegin(GL_TRIANGLES);
            glVertex2f( 150.0f, 40.0f);
            glVertex2f( 210.0f, 40.0f);
            glVertex2f( 180.0f, 85.0f);
        glEnd();
        // Cửa cối xay gió
        glColor3f(0.40f, 0.20f, 0.10f);
        glBegin(GL_QUADS);
            glVertex2f( 165.0f, -140.0f);
            glVertex2f( 195.0f, -140.0f);
            glVertex2f( 195.0f,  -85.0f);
            glVertex2f( 165.0f,  -85.0f);
        glEnd();

        // Trục xoay và 4 cánh quạt cối xay gió (TỰ XOAY THEO THỜI GIAN)
        glPushMatrix();
            glTranslatef(180.0f, 45.0f, 0.0f); // Tâm trục quay
            glRotatef(time * 65.0f, 0.0f, 0.0f, 1.0f); // Tự quay 65 độ/giây

            // Vẽ 4 cánh quạt cách nhau 90 độ
            for (int k = 0; k < 4; k++) {
                glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

                // Thanh chính của cánh quạt
                glColor3f(0.25f, 0.25f, 0.28f);
                glLineWidth(3.0f);
                glBegin(GL_LINES);
                    glVertex2f(0.0f, 0.0f);
                    glVertex2f(0.0f, 105.0f);
                glEnd();

                // Lưới cánh quạt màu cam nhạt
                glColor3f(0.92f, 0.60f, 0.30f);
                glBegin(GL_QUADS);
                    glVertex2f(  2.0f,  30.0f);
                    glVertex2f( 18.0f,  30.0f);
                    glVertex2f( 22.0f, 100.0f);
                    glVertex2f(  2.0f, 100.0f);
                glEnd();
            }

            // Nút trục tròn trung tâm màu đen
            glColor3f(0.15f, 0.15f, 0.18f);
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.0f, 0.0f);
                for (int i = 0; i <= 16; i++) {
                    float a = i * 2.0f * (float)M_PI / 16.0f;
                    glVertex2f(8.0f * cos(a), 8.0f * sin(a));
                }
            glEnd();
        glPopMatrix();

        // 1.9 BỤI HOA NHỎ TRÊN BÃI CỎ
        for (int i = -4; i <= 4; i++) {
            float fx = i * 75.0f + 25.0f;
            float fy = -220.0f + (i % 2) * 40.0f;
            glColor3f(1.0f, 0.3f, 0.6f);
            glPointSize(7.0f);
            glBegin(GL_POINTS);
                glVertex2f(fx, fy);
            glEnd();
        }
    }


    // =========================================================================
    // 2. VẬT THỂ 3D HOÀN CHỈNH 1: CHIẾC XE Ô TÔ 3D (3D CAR MODEL)
    // =========================================================================
    static void drawCarWheel(float radius, float width, float spinAngle) {
        glPushMatrix();
            // Xoay bánh xe quanh trục của nó
            glRotatef(spinAngle, 0.0f, 0.0f, 1.0f);

            // Lốp cao su màu đen xám
            glColor3f(0.14f, 0.14f, 0.15f);
            drawCylinderZ(radius, width, 24);

            // Mâm đúc La-zăng màu bạc kim loại sáng ở 2 mặt
            glColor3f(0.82f, 0.85f, 0.90f);
            float rimRadius = radius * 0.62f;
            float sides[2] = { -width / 2.0f - 0.005f, width / 2.0f + 0.005f };
            for (float side : sides) {
                glBegin(GL_TRIANGLE_FAN);
                    glNormal3f(0.0f, 0.0f, side > 0 ? 1.0f : -1.0f);
                    glVertex3f(0.0f, 0.0f, side);
                    for (int i = 0; i <= 20; i++) {
                        float a = i * 2.0f * (float)M_PI / 20.0f;
                        glVertex3f(rimRadius * cos(a), rimRadius * sin(a), side);
                    }
                glEnd();

                // Các chấu nan hoa mâm xe (5 chấu)
                glColor3f(0.3f, 0.3f, 0.35f);
                glLineWidth(3.0f);
                glBegin(GL_LINES);
                for (int i = 0; i < 5; i++) {
                    float a = i * 2.0f * (float)M_PI / 5.0f;
                    glVertex3f(0.0f, 0.0f, side + 0.002f);
                    glVertex3f(rimRadius * cos(a), rimRadius * sin(a), side + 0.002f);
                }
                glEnd();
                glColor3f(0.82f, 0.85f, 0.90f);
            }
        glPopMatrix();
    }

    void render3DCar(float time) {
        setupLighting();

        glPushMatrix();
            // Nâng xe lên mặt đường
            glTranslatef(0.0f, -0.2f, 0.0f);

            // 2.1 MẶT ĐƯỜNG ASPHALT & VẠCH KẺ ĐƯỜNG
            glColor3f(0.20f, 0.22f, 0.24f);
            glPushMatrix();
                glTranslatef(0.0f, -0.02f, 0.0f);
                drawBox(6.5f, 0.04f, 4.0f);
            glPopMatrix();

            // Vạch sơn trắng giữa đường
            glDisable(GL_LIGHTING);
            glColor3f(0.95f, 0.95f, 0.95f);
            glBegin(GL_QUADS);
                glVertex3f(-2.5f, 0.005f, -0.08f); glVertex3f(-1.0f, 0.005f, -0.08f);
                glVertex3f(-1.0f, 0.005f,  0.08f); glVertex3f(-2.5f, 0.005f,  0.08f);

                glVertex3f(-0.5f, 0.005f, -0.08f); glVertex3f( 1.0f, 0.005f, -0.08f);
                glVertex3f( 1.0f, 0.005f,  0.08f); glVertex3f(-0.5f, 0.005f,  0.08f);

                glVertex3f( 1.5f, 0.005f, -0.08f); glVertex3f( 3.0f, 0.005f, -0.08f);
                glVertex3f( 3.0f, 0.005f,  0.08f); glVertex3f( 1.5f, 0.005f,  0.08f);
            glEnd();
            setupLighting();

            // 2.2 THÂN XE CHÍNH (CHASSIS) - Đỏ thể thao bóng bẩy
            glColor3f(0.88f, 0.12f, 0.16f);
            glPushMatrix();
                glTranslatef(0.0f, 0.55f, 0.0f);
                drawBox(3.8f, 0.55f, 1.8f);
            glPopMatrix();

            // Cản trước và cản sau thể thao màu đen
            glColor3f(0.15f, 0.15f, 0.18f);
            glPushMatrix();
                glTranslatef( 1.95f, 0.42f, 0.0f); // Cản trước
                drawBox(0.2f, 0.30f, 1.82f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-1.95f, 0.42f, 0.0f); // Cản sau
                drawBox(0.2f, 0.30f, 1.82f);
            glPopMatrix();

            // 2.3 BUỒNG LÁI & MUI XE (CABIN)
            glColor3f(0.82f, 0.10f, 0.14f);
            glPushMatrix();
                glTranslatef(-0.20f, 1.05f, 0.0f);
                drawBox(2.1f, 0.52f, 1.5f);
            glPopMatrix();

            // Kính chắn gió trước (Kính nghiêng màu xanh bóng)
            glColor3f(0.35f, 0.70f, 0.95f);
            glBegin(GL_QUADS);
                glNormal3f(0.7f, 0.7f, 0.0f);
                glVertex3f( 0.85f, 0.82f, -0.70f);
                glVertex3f( 0.85f, 0.82f,  0.70f);
                glVertex3f( 0.65f, 1.30f,  0.65f);
                glVertex3f( 0.65f, 1.30f, -0.65f);
            glEnd();

            // Kính chắn gió sau
            glBegin(GL_QUADS);
                glNormal3f(-0.7f, 0.7f, 0.0f);
                glVertex3f(-1.25f, 1.30f, -0.65f);
                glVertex3f(-1.25f, 1.30f,  0.65f);
                glVertex3f(-1.45f, 0.82f,  0.70f);
                glVertex3f(-1.45f, 0.82f, -0.70f);
            glEnd();

            // Kính cửa sổ bên trái (+Z) và bên phải (-Z)
            glBegin(GL_QUADS);
                // Trái (+Z)
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(-1.15f, 0.86f,  0.76f);
                glVertex3f( 0.70f, 0.86f,  0.76f);
                glVertex3f( 0.55f, 1.25f,  0.76f);
                glVertex3f(-1.10f, 1.25f,  0.76f);

                // Phải (-Z)
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(-1.15f, 0.86f, -0.76f);
                glVertex3f(-1.10f, 1.25f, -0.76f);
                glVertex3f( 0.55f, 1.25f, -0.76f);
                glVertex3f( 0.70f, 0.86f, -0.76f);
            glEnd();

            // 2.4 CẶP ĐÈN PHA TRƯỚC (Vàng Xenon rực rỡ)
            glColor3f(1.0f, 0.95f, 0.35f);
            glPushMatrix();
                glTranslatef(1.92f, 0.62f, 0.58f);
                drawBox(0.05f, 0.16f, 0.32f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(1.92f, 0.62f, -0.58f);
                drawBox(0.05f, 0.16f, 0.32f);
            glPopMatrix();

            // Lưới tản nhiệt trước xe
            glColor3f(0.1f, 0.1f, 0.12f);
            glPushMatrix();
                glTranslatef(1.92f, 0.50f, 0.0f);
                drawBox(0.04f, 0.22f, 0.65f);
            glPopMatrix();

            // 2.5 CẶP ĐÈN HẬU SAU XE (Đỏ Ruby)
            glColor3f(0.95f, 0.1f, 0.1f);
            glPushMatrix();
                glTranslatef(-1.92f, 0.65f, 0.62f);
                drawBox(0.05f, 0.14f, 0.35f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-1.92f, 0.65f, -0.62f);
                drawBox(0.05f, 0.14f, 0.35f);
            glPopMatrix();

            // Ống xả kép mạ Crom ở đuôi xe
            glColor3f(0.85f, 0.85f, 0.90f);
            glPushMatrix();
                glTranslatef(-1.98f, 0.35f, 0.35f);
                drawBox(0.15f, 0.10f, 0.12f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-1.98f, 0.35f, -0.35f);
                drawBox(0.15f, 0.10f, 0.12f);
            glPopMatrix();

            // Gương chiếu hậu 2 bên
            glColor3f(0.80f, 0.10f, 0.14f);
            glPushMatrix();
                glTranslatef(0.65f, 0.95f, 0.88f);
                drawBox(0.15f, 0.10f, 0.20f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.65f, 0.95f, -0.88f);
                drawBox(0.15f, 0.10f, 0.20f);
            glPopMatrix();

            // 2.6 BỐN BÁNH XE TỰ QUAY KHI XE CHUYỂN ĐỘNG
            float spinAngle = time * 320.0f; // Bánh xe lăn tròn
            float wheelR = 0.40f;
            float wheelW = 0.28f;

            // Bánh trước - trái
            glPushMatrix();
                glTranslatef(1.15f, 0.40f, 0.95f);
                drawCarWheel(wheelR, wheelW, spinAngle);
            glPopMatrix();

            // Bánh trước - phải
            glPushMatrix();
                glTranslatef(1.15f, 0.40f, -0.95f);
                drawCarWheel(wheelR, wheelW, spinAngle);
            glPopMatrix();

            // Bánh sau - trái
            glPushMatrix();
                glTranslatef(-1.15f, 0.40f, 0.95f);
                drawCarWheel(wheelR, wheelW, spinAngle);
            glPopMatrix();

            // Bánh sau - phải
            glPushMatrix();
                glTranslatef(-1.15f, 0.40f, -0.95f);
                drawCarWheel(wheelR, wheelW, spinAngle);
            glPopMatrix();

        glPopMatrix();
    }


    // =========================================================================
    // 3. VẬT THỂ 3D HOÀN CHỈNH 2: MÁY BAY TRỰC THĂNG 3D (3D HELICOPTER)
    // =========================================================================
    void render3DHelicopter(float time) {
        setupLighting();

        glPushMatrix();
            // Lắc lư nhẹ nhàng như đang bay lơ lửng (Hovering)
            float hoverY = sin(time * 2.5f) * 0.12f;
            float tiltZ  = sin(time * 2.0f) * 2.5f;
            glTranslatef(0.0f, hoverY, 0.0f);
            glRotatef(tiltZ, 0.0f, 0.0f, 1.0f);

            // 3.1 THÂN MÁY BAY CHÍNH (Xanh dương Hải Quân)
            glColor3f(0.18f, 0.42f, 0.78f);
            glPushMatrix();
                glScalef(1.9f, 1.25f, 1.25f);
                drawSphere3D(0.9f, 24, 24);
            glPopMatrix();

            // 3.2 KÍNH BUỒNG LÁI TRƯỚC (Xanh ngọc phản chiếu)
            glColor3f(0.35f, 0.80f, 0.95f);
            glPushMatrix();
                glTranslatef(0.85f, 0.15f, 0.0f);
                glScalef(0.85f, 0.80f, 0.95f);
                drawSphere3D(0.65f, 16, 16);
            glPopMatrix();

            // 3.3 ĐUÔI MÁY BAY DÀI (Khối thon kéo dài về phía sau)
            glColor3f(0.15f, 0.36f, 0.70f);
            glPushMatrix();
                glTranslatef(-2.2f, 0.25f, 0.0f);
                drawBox(2.2f, 0.25f, 0.22f);
            glPopMatrix();

            // Cánh đuôi đứng (Vertical Tail Fin)
            glColor3f(0.90f, 0.20f, 0.20f); // Sơn đỏ nổi bật
            glPushMatrix();
                glTranslatef(-3.25f, 0.65f, 0.0f);
                drawBox(0.45f, 0.85f, 0.08f);
            glPopMatrix();

            // Cánh đuôi ngang (Horizontal Stabilizer)
            glColor3f(0.85f, 0.85f, 0.90f);
            glPushMatrix();
                glTranslatef(-3.05f, 0.35f, 0.0f);
                drawBox(0.25f, 0.06f, 0.95f);
            glPopMatrix();

            // 3.4 CÁNH QUẠT ĐUÔI QUAY RẤT NHANH (Tail Rotor)
            glPushMatrix();
                glTranslatef(-3.25f, 0.85f, 0.07f);
                glRotatef(time * 1200.0f, 0.0f, 0.0f, 1.0f); // Xoay 1200 độ/s
                glColor3f(0.2f, 0.2f, 0.22f);
                drawBox(0.85f, 0.08f, 0.02f);
            glPopMatrix();

            // 3.5 TRỤC VÀ CÁNH QUẠT CHÍNH TRÊN ĐỈNH (Main Rotor)
            // Cột trục cánh quạt
            glColor3f(0.3f, 0.3f, 0.35f);
            glPushMatrix();
                glTranslatef(0.0f, 1.25f, 0.0f);
                drawBox(0.14f, 0.40f, 0.14f);
            glPopMatrix();

            // 4 Cánh quạt chính lớn quay tít
            glPushMatrix();
                glTranslatef(0.0f, 1.45f, 0.0f);
                glRotatef(time * 800.0f, 0.0f, 1.0f, 0.0f); // Xoay 800 độ/s

                // Đĩa tâm trục cánh quạt
                glColor3f(0.1f, 0.1f, 0.12f);
                drawBox(0.4f, 0.08f, 0.4f);

                // 4 Cánh quạt dài mỏng
                glColor3f(0.25f, 0.25f, 0.28f);
                for (int i = 0; i < 4; i++) {
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                    glPushMatrix();
                        glTranslatef(1.9f, 0.0f, 0.0f);
                        drawBox(3.5f, 0.03f, 0.22f);
                        // Đầu mút cánh quạt sơn vàng cảnh báo
                        glColor3f(1.0f, 0.85f, 0.1f);
                        glTranslatef(1.65f, 0.0f, 0.0f);
                        drawBox(0.3f, 0.032f, 0.225f);
                        glColor3f(0.25f, 0.25f, 0.28f);
                    glPopMatrix();
                }
            glPopMatrix();

            // 3.6 BỘ CÀNG ĐÁP HẠ CÁNH (Landing Skids)
            glColor3f(0.40f, 0.40f, 0.45f);
            // Hai thanh trượt song song
            glPushMatrix();
                glTranslatef(0.0f, -1.25f, 0.75f);
                drawBox(3.4f, 0.08f, 0.08f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0f, -1.25f, -0.75f);
                drawBox(3.4f, 0.08f, 0.08f);
            glPopMatrix();

            // Các thanh chống nghiêng nối thân với càng
            float strutX[2] = { -0.7f, 0.7f };
            float strutZ[2] = { -0.55f, 0.55f };
            for (float x : strutX) {
                for (float z : strutZ) {
                    glPushMatrix();
                        glTranslatef(x, -0.95f, z);
                        drawBox(0.07f, 0.55f, 0.07f);
                    glPopMatrix();
                }
            }

            // Đèn chiếu sáng / Tìm kiếm phía mũi máy bay
            glColor3f(1.0f, 0.95f, 0.5f);
            glPushMatrix();
                glTranslatef(1.5f, -0.4f, 0.0f);
                drawBox(0.2f, 0.18f, 0.18f);
            glPopMatrix();

        glPopMatrix();
    }

} // namespace Shapes
