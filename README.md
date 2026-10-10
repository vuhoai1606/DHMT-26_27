# Đồ Họa Máy Tính (DHMT) - Trọn Bộ Lab 0, Lab 1, Lab 2 & Lab 3 (OpenGL C++)

Hệ thống mã nguồn thực hành môn **Đồ Họa Máy Tính** hoàn chỉnh, độc lập và tối ưu. Dự án được xây dựng bằng C++ và thư viện chuẩn OpenGL của hệ điều hành Windows (`-lopengl32 -lglu32 -lgdi32 -luser32`), biên dịch trực tiếp bằng **MinGW GCC / G++** mà không phụ thuộc bất kỳ file `.dll` ngoại lai nào.

---

## 📁 Cấu Trúc Thư Mục Dự Án

```
DHMT/Code/
├── .vscode/                 # Cấu hình biên dịch tự động trong VS Code
│   ├── tasks.json           # Tasks biên dịch Lab 0, 1, 2, 3
│   └── launch.json
├── bin/                     # Chứa các file thực thi (.exe) và dữ liệu test
│   ├── Lab0_OpenGL.exe      # Thực thi Lab 0: 2D Landscape & 3D Objects
│   ├── Lab1_OpenGL.exe      # Thực thi Lab 1: Bresenham & Midpoint Curves
│   ├── Lab2_OpenGL.exe      # Thực thi Lab 2: Line Clipping & Polygon Filling
│   ├── Lab3_OpenGL.exe      # Thực thi Lab 3: 2D Transformations (Matrix 3x3)
│   ├── test_labs.exe        # Test suite tự động kiểm thử thuật toán
│   ├── polygon.txt          # File dữ liệu đa giác mẫu chuẩn (4 đỉnh)
│   ├── polygon_star.txt     # File dữ liệu đa giác ngôi sao 5 cánh (10 đỉnh)
│   └── polygon_arrow.txt    # File dữ liệu đa giác mũi tên (7 đỉnh)
├── src/
│   ├── Lab0/                # Lab 0: Đối tượng 2D & 3D hoàn chỉnh có chuyển động
│   ├── Lab1/                # Lab 1: So sánh thuật toán Bresenham & Midpoint
│   ├── Lab2/                # Lab 2: Thuật toán Xén hình & Tô màu đa giác
│   │   ├── Clipping.h / .cpp # Cohen-Sutherland & Chia nhị phân (Midpoint Subdivision)
│   │   ├── Filling.h / .cpp  # Boundary Fill (đệ quy & stack), Scanline Fill, Framebuffer
│   │   ├── Window.h / .cpp   # Quản lý cửa sổ Win32 & OpenGL Context
│   │   └── main.cpp          # Giao diện tương tác trực quan Lab 2
│   ├── Lab3/                # Lab 3: Các phép biến đổi 2D bằng tọa độ thuần nhất
│   │   ├── Transform2D.h/.cpp# Ma trận 3x3: Tịnh tiến, Tỉ lệ, Quay, Đối xứng y=ax+b
│   │   ├── Window.h / .cpp   # Quản lý cửa sổ Win32 & OpenGL Context
│   │   └── main.cpp          # Giao diện tương tác trực quan Lab 3
│   └── test_labs.cpp        # Bộ kiểm thử toán học tự động cho Lab 2 & Lab 3
├── build.bat                # Menu biên dịch tất cả các bài Lab
└── run.bat                  # Menu chạy nhanh các bài Lab
```

---

## 🎯 Chi Tiết LAB 02: Xén Hình & Tô Màu Đa Giác

### 1. Thuật toán Xén đoạn thẳng (Line Clipping)
- **Cohen-Sutherland**: 
  - Gán mã vùng 4-bit OutCode (`LEFT=1, RIGHT=2, BOTTOM=4, TOP=8`).
  - Kiểm tra chấp nhận tức thì (`code1 | code2 == 0`) hoặc loại bỏ tức thì (`code1 & code2 != 0`).
  - Tính toán giao điểm đại số chính xác với 4 đường biên của cửa sổ chữ nhật.
- **Chia nhị phân (Midpoint Subdivision / Binary Clipping)**:
  - Phân đoạn nhị phân liên tục để hội tụ điểm giao với đường biên cửa sổ.
  - Xử lý đầy đủ 4 trường hợp: đoạn thẳng nằm trong, đoạn thẳng nằm ngoài, cắt 1 biên, hoặc xuyên qua 2 biên đối diện.

### 2. Thuật toán Tô màu đa giác (Polygon Filling)
- **Tô theo đường biên Đệ quy (Boundary Fill - Recursive)**:
  - Thuật toán loang 4 lân cận (4-connected).
  - Có cơ chế giới hạn độ sâu đệ quy và cấu hình Call Stack 64MB an toàn chống tràn stack.
- **Tô theo đường biên Khử đệ quy (Boundary Fill - Stack)**:
  - Khử đệ quy bằng `std::vector` Stack mô phỏng cấu trúc dữ liệu theo giáo trình.
  - Tốc độ tức thì, giải phóng hoàn toàn áp lực lên ngăn xếp hệ thống, tô được đa giác diện tích lớn.
- **Tô màu theo dòng quét (Scanline Polygon Fill)**:
  - Xác định $y_{min}, y_{max}$, xây dựng bảng cạnh (Edge Table) với hệ số góc nghịch đảo $1/m = \Delta x / \Delta y$.
  - Quét từng dòng quét $y$, tìm giao điểm kích hoạt ($y_{min} \le y < y_{max}$), sắp xếp hoành độ $x$ tăng dần và tô các đoạn ngang tương ứng. Không cần chọn điểm hạt giống (Seed Point).

### 3. Phím tắt & Thao tác LAB 02
| Phím / Chuột | Chức Năng |
| :---: | :--- |
| **`1`** | Chuyển sang **Chế độ Xén hình (Line Clipping)** |
| **`2`** | Chuyển sang **Chế độ Tô màu đa giác (Polygon Filling)** |
| **`C`** | Chọn thuật toán xén **Cohen-Sutherland** |
| **`M`** | Chọn thuật toán xén **Chia nhị phân (Midpoint)** |
| **`R`** | Chọn thuật toán tô **Đường biên Đệ quy (Boundary Recursive)** |
| **`K`** | Chọn thuật toán tô **Đường biên Khử đệ quy (Boundary Stack)** |
| **`S`** | Chọn thuật toán tô **Dòng quét (Scanline Fill)** |
| **Chuột trái (Chế độ xén)** | Click 1 chọn điểm đầu $\to$ Di chuột xem trước $\to$ Click 2 chốt đoạn thẳng |
| **Chuột trái (Chế độ tô)** | Chọn điểm hạt giống (Seed Point) để tô đường biên |
| **Chuột phải (Chế độ tô)** | Kích hoạt thuật toán tô màu (Trigger Fill Pipeline) |
| **Phím Mũi tên** | Di chuyển cửa sổ xén (Clipping Window) |
| **`[` / `]`** | Thu nhỏ / Mở rộng chiều rộng cửa sổ xén |
| **`,` / `.`** | Thu nhỏ / Mở rộng chiều cao cửa sổ xén |
| **`O`** | Mở file đa giác (`polygon.txt` hoặc nhập tên file) |
| **`F5` / `F6` / `F7`** | Nạp nhanh đa giác mẫu: Ngôi sao 5 cánh, Mũi tên, Đa giác lõm |
| **`G`** | Bật / Tắt lưới toạ độ |
| **`X`** | Xoá danh sách đoạn thẳng / Xoá kết quả tô màu |
| **`F9`** | Lưu ảnh chụp màn hình 24-bit BMP (`screenshot_lab2_###.bmp`) |

---

## 📐 Chi Tiết LAB 03: Các Phép Biến Đổi 2D (Tọa Độ Thuần Nhất 3x3)

Toàn bộ các phép biến đổi được cài đặt thông qua lớp ma trận thuần nhất $\mathbf{M} \in \mathbb{R}^{3 \times 3}$:

### 1. Phép Tịnh tiến (Translation)
$$\mathbf{T}(t_x, t_y) = \begin{bmatrix} 1 & 0 & t_x \\ 0 & 1 & t_y \\ 0 & 0 & 1 \end{bmatrix}$$

### 2. Phép Tỉ lệ với tâm tỉ lệ $(x_0, y_0)$ (Scaling)
$$\mathbf{S}(s_x, s_y, x_0, y_0) = \mathbf{T}(x_0, y_0) \cdot \mathbf{S}(s_x, s_y) \cdot \mathbf{T}(-x_0, -y_0) = \begin{bmatrix} s_x & 0 & x_0(1 - s_x) \\ 0 & s_y & y_0(1 - s_y) \\ 0 & 0 & 1 \end{bmatrix}$$

### 3. Phép Quay với tâm quay $(x_0, y_0)$ (Rotation)
Với góc quay $\theta$ (radian $\alpha = \theta \frac{\pi}{180}$):
$$\mathbf{R}(\theta, x_0, y_0) = \mathbf{T}(x_0, y_0) \cdot \mathbf{R}(\alpha) \cdot \mathbf{T}(-x_0, -y_0) = \begin{bmatrix} \cos\alpha & -\sin\alpha & x_0(1 - \cos\alpha) + y_0\sin\alpha \\ \sin\alpha & \cos\alpha & y_0(1 - \cos\alpha) - x_0\sin\alpha \\ 0 & 0 & 1 \end{bmatrix}$$

### 4. Phép Đối xứng qua đường thẳng $y = ax + b$ (Reflection)
Với đường thẳng $y = ax + b$:
$$\mathbf{M}_{refl} = \begin{bmatrix} \frac{1 - a^2}{1 + a^2} & \frac{2a}{1 + a^2} & \frac{-2ab}{1 + a^2} \\ \frac{2a}{1 + a^2} & \frac{a^2 - 1}{1 + a^2} & \frac{2b}{1 + a^2} \\ 0 & 0 & 1 \end{bmatrix}$$
Chương trình tự động vẽ đường thẳng trục đối xứng $y = ax + b$ bằng nét đứt màu cam nổi bật cắt ngang màn hình để quan sát trực quan.

### 5. Phím tắt & Thao tác LAB 03
| Phím / Chuột | Chức Năng |
| :---: | :--- |
| **`T`** | Nhập vector tịnh tiến $(t_x, t_y)$ từ Console |
| **Phím Mũi tên** | Tịnh tiến tức thì trên màn hình ($\pm 15$ px) |
| **`Y` / `S`** | Nhập hệ số tỉ lệ $s_x, s_y$ và tâm tỉ lệ $(x_0, y_0)$ từ Console |
| **`+` / `-`** | Phóng to / Thu nhỏ tức thì $\times 1.1$ / $\times 0.9$ quanh tâm |
| **`U` / `R`** | Nhập góc quay $\theta$ (độ) và tâm quay $(x_0, y_0)$ từ Console |
| **`[` / `]`** | Quay tức thì $\pm 10^\circ$ quanh tâm |
| **`I` / `D`** | Nhập hệ số $a, b$ của đường thẳng đối xứng $y = ax + b$ |
| **Chuột trái** | Click trên màn hình để đặt tâm biến đổi (Pivot Crosshair) $(x_0, y_0)$ |
| **`V`** | Bật / Tắt hiển thị dấu tâm biến đổi (Crosshair) & Hộp bao quanh (Bounding Box) |
| **`Space`** | Bật / Tắt chế độ tự động quay liên tục (**Auto-spin**) |
| **`;` / `'`** | Giảm / Tăng tốc độ quay tự động ($\pm 10^\circ$/s) |
| **`A`** | Bật / Tắt tự động giữ hình trong màn hình (**Auto keep-in-window**) |
| **`F`** | Căn giữa và phóng to đa giác vừa vặn cửa sổ (**Auto-fit**) |
| **`Z`** | Hoàn tác phép biến đổi gần nhất (**Undo**) |
| **`X`** | Đặt lại về đa giác gốc ban đầu (**Reset**) |
| **`O`** | Mở file đa giác (`polygon.txt`) |
| **`F5` / `F6` / `F7`** | Nạp đa giác mẫu: Ngôi sao 5 cánh, Mũi tên, Ngôi nhà |
| **`F9`** | Lưu ảnh chụp màn hình BMP (`screenshot_lab3_###.bmp`) |

---

## 🚀 Hướng Dẫn Biên Dịch & Chạy

### Cách 1: Sử dụng file kịch bản Batch
- Nhấp đúp vào file [`build.bat`](file:///d:/Lap%20Trinh/Nam%203%20Ki%201/DHMT/Code/build.bat) và chọn `[2]` để biên dịch Lab 2, `[3]` để biên dịch Lab 3, hoặc `[A]` để biên dịch toàn bộ.
- Nhấp đúp vào file [`run.bat`](file:///d:/Lap%20Trinh/Nam%203%20Ki%201/DHMT/Code/run.bat) để chọn và chạy bài Lab mong muốn.

### Cách 2: Sử dụng dòng lệnh Terminal
```powershell
# Biên dịch Lab 2:
g++ -std=c++11 -O2 -Isrc/Lab2 src/Lab2/main.cpp src/Lab2/Window.cpp src/Lab2/Clipping.cpp src/Lab2/Filling.cpp -o bin/Lab2_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32 -Wl,--stack,67108864

# Chạy Lab 2:
.\bin\Lab2_OpenGL.exe

# Biên dịch Lab 3:
g++ -std=c++11 -O2 -Isrc/Lab3 src/Lab3/main.cpp src/Lab3/Window.cpp src/Lab3/Transform2D.cpp -o bin/Lab3_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32

# Chạy Lab 3:
.\bin\Lab3_OpenGL.exe
```

### Cách 3: Chạy bộ kiểm thử tự động
```powershell
.\bin\test_labs.exe
```
Kết quả kiểm thử tự động: Tất cả các trường hợp xén hình, đếm số pixel tô màu đường biên/dòng quét và các ma trận biến đổi thuần nhất đều đạt độ chính xác 100%.
