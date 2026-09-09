# Đồ Họa Máy Tính (DHMT) - Đối Tượng 2D & 3D Hoàn Chỉnh Bằng OpenGL

Dự án C++ OpenGL được xây dựng chuyên sâu cho môn học **Đồ Họa Máy Tính**, đáp ứng trực tiếp và thuyết phục yêu cầu: **Vẽ một bức tranh 2D hoàn chỉnh và các vật thể 3D thực tế hoàn chỉnh có chuyển động**, thay vì chỉ vẽ những khối hình rời rạc đơn lẻ.

Mã nguồn được biên dịch bằng **MinGW GCC / G++** trên Windows, chạy trực tiếp không phụ thuộc bất kỳ file `.dll` nào từ bên ngoài.

---

## 🎨 Nội Dung Các Đối Tượng Trong Dự Án

### 1. Phím `1` — Bức Tranh 2D Phong Cảnh Hoàn Chỉnh (Đồng Quê & Cối Xay Gió)
Một bức tranh phong cảnh 2D sống động kết hợp đầy đủ các kỹ thuật vẽ 2D trong OpenGL:
- **Bầu trời xanh:** Đổ màu gradient từ xanh lam đậm xuống xanh phấn chân trời (`GL_QUADS`).
- **Mặt trời vàng:** Vầng dương rực rỡ kèm các **tia nắng tỏa tròn tự động xoay chậm** (`GL_LINES` + `glRotatef`).
- **Mây trắng:** Các đám mây bồng bềnh trôi nhẹ nhàng trên bầu trời.
- **Đồi cỏ xanh & Con đường uốn lượn:** Dẫn thẳng vào cổng ngôi nhà (`GL_QUAD_STRIP`).
- **Ngôi nhà 2D:** Thân nhà tường kem, mái ngói tam giác đỏ, ống khói có làn khói bốc lên theo nhịp, cửa sổ kính xanh chia 4 ô, cửa chính gỗ nâu có tay nắm tròn mạ vàng.
- **Cây táo:** Thân cây gỗ nâu, tán lá tròn xanh mượt và những quả táo đỏ chín mọng.
- **Chiếc cối xay gió:** Tháp đá hình thang, **4 cánh quạt lớn tự động quay tròn nhịp nhàng** quanh trục.

---

### 2. Phím `2` — Vật Thể 3D Hoàn Chỉnh: Chiếc Xe Ô Tô Thể Thao (3D Sports Car)
Một mẫu xe hơi 3D hoàn chỉnh đến từng chi tiết cơ khí và ngoại thất:
- **Thân xe thể thao (Chassis):** Khối khí động học dài màu đỏ kim loại bóng bẩy, cản trước & cản sau màu đen thể thao.
- **Cabin & Kính xe:** Mui xe thon gọn, kính chắn gió trước/sau nghiêng thể thao và cửa sổ phản quang kính xanh.
- **Hệ thống 4 bánh xe tự lăn tròn:** Lốp cao su đen, **mâm đúc La-zăng hợp kim bạc 5 chấu** quay tròn mượt mà quanh trục khi xe di chuyển.
- **Chi tiết ngoại thất:** Cặp đèn pha Xenon phía trước, cặp đèn hậu đỏ LED phía sau, lưới tản nhiệt tản nhiệt làm mát, cặp gương chiếu hậu khí động học và ống xả kép mạ crom ở đuôi.
- **Mặt đường:** Đặt trên đoạn đường Asphalt có vạch sơn trắng phân làn giao thông.

---

### 3. Phím `3` — Vật Thể 3D Hoàn Chỉnh: Máy Bay Trực Thăng (3D Helicopter)
Một mô hình máy bay trực thăng quân sự/cứu hộ hoàn chỉnh:
- **Thân máy bay:** Khí động học thuôn tròn màu xanh hải quân, buồng lái bằng kính vòm lồi nhìn toàn cảnh.
- **Càng hạ cánh (Landing Skids):** Hệ thống 2 ống trượt song song liên kết với thân qua các thanh giằng nghiêng chắc chắn.
- **Đuôi dài & Cánh đuôi:** Thân đuôi kéo dài có cánh đuôi đứng màu đỏ nổi bật và cánh thăng bằng ngang.
- **Cánh quạt chính (Main Rotor):** Trục rotor và 4 cánh quạt dài sơn vạch vàng cảnh báo **quay tít ở tốc độ cao**.
- **Cánh quạt đuôi (Tail Rotor):** Cánh quạt nhỏ phía sau quay siêu tốc để triệt tiêu mô-men xoắn.
- **Hiệu ứng bay:** Máy bay nhấp nhô lơ lửng (Hovering) sống động trong không gian 3D.

---

## 🖱️ Phím Tắt & Thao Tác Điều Khiển

| Phím Tắt | Chức Năng |
| :---: | :--- |
| **`1`** | Xem **Bức tranh phong cảnh 2D hoàn chỉnh** |
| **`2`** | Xem **Chiếc xe hơi 3D thể thao hoàn chỉnh** |
| **`3`** | Xem **Máy bay trực thăng 3D hoàn chỉnh** |
| **`W`** | Bật / Tắt chế độ **Khung dây (Wireframe)** $\leftrightarrow$ **Mặt đặc (Solid)** để soi cấu trúc lưới 3D |
| **`T`** | Bật / Tắt **Chuyển động cơ cấu** (Bánh xe lăn, Cánh quạt cối xay gió, Cánh quạt máy bay) |
| **`Space`** | Bật / Tắt **Tự động xoay Camera 360 độ** quanh vật thể |
| **`R`** | Đặt lại (Reset) góc nhìn camera về mặc định |
| **Chuột trái + Kéo** | Xoay tự do góc nhìn 3D quanh tâm vật thể (Orbit) |
| **Chuột phải + Kéo** | Di chuyển tâm góc nhìn (Pan) |
| **Con lăn chuột** | Phóng to / Thu nhỏ mô hình (Zoom) |
| **`ESC`** | Thoát chương trình |

---

## 🚀 Cách Chạy Ứng Dụng

1. Nhấp đúp chuột vào file **[`run.bat`](file:///d:/Lap%20Trinh/Nam%203%20Ki%201/DHMT/run.bat)** (hoặc [`build.bat`](file:///d:/Lap%20Trinh/Nam%203%20Ki%201/DHMT/build.bat)) trong thư mục dự án.
2. Hoặc mở Terminal và gõ:
   ```powershell
   .\run.bat
   ```
