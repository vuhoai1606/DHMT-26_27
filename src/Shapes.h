#ifndef SHAPES_H
#define SHAPES_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace Shapes {
    // =========================================================================
    // 1. BỨC TRANH 2D HOÀN CHỈNH: PHONG CẢNH ĐỒNG QUÊ & CỐI XAY GIÓ CHUYỂN ĐỘNG
    // =========================================================================
    // Vẽ bức tranh phong cảnh 2D hoàn chỉnh:
    // - Bầu trời xanh, mây trắng bồng bềnh
    // - Mặt trời vàng rực rỡ với các tia nắng tỏa xoay nhẹ
    // - Đồi cỏ xanh mượt và con đường uốn lượn
    // - Ngôi nhà 2D đầy đủ (thân nhà, mái ngói đỏ, ống khói, cửa chính, cửa sổ)
    // - Cây cối sum suê với những quả táo đỏ
    // - Chiếc cối xay gió có cánh quạt tự quay mượt mà
    void render2DLandscape(float time);


    // =========================================================================
    // 2. VẬT THỂ 3D HOÀN CHỈNH 1: CHIẾC XE Ô TÔ 3D (3D CAR MODEL)
    // =========================================================================
    // Vật thể 3D hoàn chỉnh đầy đủ chi tiết:
    // - Khung gầm & thân xe thể thao (Chassis) màu đỏ kim loại
    // - Buồng lái mui vòm (Cabin) với các mặt kính chắn gió xanh gương phản xạ
    // - 4 bánh xe cao su có mâm la-zăng kim loại, tự động lăn tròn khi xe chạy
    // - Cặp đèn pha trước chiếu sáng, cặp đèn hậu đỏ, lưới tản nhiệt
    // - Gương chiếu hậu 2 bên và ống xả
    void render3DCar(float time);


    // =========================================================================
    // 3. VẬT THỂ 3D HOÀN CHỈNH 2: MÁY BAY TRỰC THĂNG 3D (3D HELICOPTER)
    // =========================================================================
    // Vật thể 3D hoàn chỉnh cơ khí:
    // - Thân máy bay khí động học thuôn tròn với kính buồng lái lớn
    // - Càng hạ cánh (Landing skids) dạng ống thép đôi
    // - Đuôi dài khí động học kèm cánh đuôi định hướng
    // - Cánh quạt chính trên đỉnh quay tít và cánh quạt đuôi giữ thăng bằng
    void render3DHelicopter(float time);


    // =========================================================================
    // CÁC HÀM HỖ TRỢ HÌNH HỌC VÀ HIỆU ỨNG
    // =========================================================================
    void drawAxes3D(float length = 3.0f);
    void setupLighting();
    void disableLighting();
}

#endif // SHAPES_H
