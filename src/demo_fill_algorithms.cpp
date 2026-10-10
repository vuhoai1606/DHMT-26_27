#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

// =============================================================================
// MÔ PHỎNG FRAMEBUFFER (CANVAS ĐIỂM ẢNH) ĐỂ TEST THUẬT TOÁN
// =============================================================================
const int CANVAS_WIDTH  = 30;
const int CANVAS_HEIGHT = 16;

// Mã màu quy ước
const int COLOR_BACKGROUND = 0; // Ký tự '.'
const int COLOR_BOUNDARY   = 1; // Ký tự '#' (đường biên)
const int COLOR_FILL       = 2; // Ký tự '*' (màu tô)

int canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

void clearCanvas() {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            canvas[y][x] = COLOR_BACKGROUND;
        }
    }
}

// Hàm getpixel mô phỏng hàm getpixel() của hệ thống đồ họa
int getpixel(int x, int y) {
    if (x < 0 || x >= CANVAS_WIDTH || y < 0 || y >= CANVAS_HEIGHT)
        return COLOR_BOUNDARY; // Ngoài biên coi như chạm biên
    return canvas[y][x];
}

// Hàm putpixel mô phỏng hàm putpixel() của hệ thống đồ họa
void putpixel(int x, int y, int color) {
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        canvas[y][x] = color;
    }
}

// Hàm in màn hình canvas ra console để xem trực quan kết quả tô màu
void printCanvas(const string& title) {
    cout << "\n================ " << title << " ================\n";
    cout << "   ";
    for (int x = 0; x < CANVAS_WIDTH; x++) cout << (x % 10);
    cout << "\n";
    for (int y = CANVAS_HEIGHT - 1; y >= 0; y--) { // In từ trên xuống dưới (y lớn xuống nhỏ)
        cout << setw(2) << y << " ";
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            if (canvas[y][x] == COLOR_BOUNDARY) cout << "#";
            else if (canvas[y][x] == COLOR_FILL) cout << "*";
            else cout << ".";
        }
        cout << "\n";
    }
    cout << "Chu thich: [.] Nen, [#] Bien da giac, [*] Mau da to\n";
}

// Thuật toán Bresenham vẽ đoạn thẳng tạo khung biên đa giác
void drawLineBresenham(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        putpixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// =============================================================================
// PHẦN 1: THUẬT TOÁN TÔ MÀU THEO ĐƯỜNG BIÊN (BOUNDARY FILL)
// =============================================================================

// --- 1.1 Cài đặt Đệ quy (Recursive - Slide 84) ---
void BoundaryFill_Recursive(int x, int y, int fillColour, int boundaryColour) {
    int colour = getpixel(x, y);
    // Nếu pixel chưa phải là màu biên và chưa phải màu cần tô
    if ((colour != boundaryColour) && (colour != fillColour)) {
        putpixel(x, y, fillColour);
        // Loang sang 4 lân cận (4-connected)
        BoundaryFill_Recursive(x - 1, y,     fillColour, boundaryColour); // Sang trái
        BoundaryFill_Recursive(x,     y + 1, fillColour, boundaryColour); // Lên trên
        BoundaryFill_Recursive(x + 1, y,     fillColour, boundaryColour); // Sang phải
        BoundaryFill_Recursive(x,     y - 1, fillColour, boundaryColour); // Xuống dưới
    }
}

// --- 1.2 Cài đặt Khử đệ quy bằng Stack cấu trúc dữ liệu (Slide 86 - 88) ---
// Định nghĩa Stack bằng danh sách liên kết y hệt Slide 86
struct LIST {
    int x, y;
    struct LIST *next;
};

struct LIST *top = NULL;

void push(int x, int y) {
    struct LIST *P = (struct LIST *) calloc(1, sizeof(struct LIST));
    P->x = x;
    P->y = y;
    P->next = NULL;
    if (top != NULL) P->next = top;
    top = P;
}

void pop(int *x, int *y) {
    if (top == NULL) return;
    struct LIST *P = top;
    top = top->next;
    *x = P->x;
    *y = P->y;
    free(P);
}

// Hàm kiểm tra và tô màu pixel lân cận, sau đó push vào Stack (Slide 87)
void BoundaryFill_Step(int x, int y, int fillColour, int boundaryColour) {
    int colour = getpixel(x, y);
    if ((colour != fillColour) && (colour != boundaryColour)) {
        putpixel(x, y, fillColour);
        push(x, y);
    }
}

// Hàm điều khiển tô loang bằng Stack (Slide 88)
void BoundaryFill_Stack(int x0, int y0, int fillColour, int boundaryColour) {
    int x, y;
    // Bước 1: Tô màu điểm seed và đưa vào stack
    putpixel(x0, y0, fillColour);
    top = NULL;
    push(x0, y0);

    // Bước 2: Khi stack chưa rỗng thì lấy điểm ra và loang 4 lân cận
    while (top != NULL) {
        pop(&x, &y);
        BoundaryFill_Step(x - 1, y,     fillColour, boundaryColour); // Trái
        BoundaryFill_Step(x + 1, y,     fillColour, boundaryColour); // Phải
        BoundaryFill_Step(x,     y + 1, fillColour, boundaryColour); // Trên
        BoundaryFill_Step(x,     y - 1, fillColour, boundaryColour); // Dưới
    }
}

// =============================================================================
// PHẦN 2: THUẬT TOÁN TÔ MÀU THEO DÒNG QUÉT (SCANLINE POLYGON FILL)
// Bám sát 100% Slide CG.3 (Slide 80 - 82)
// =============================================================================

struct Point {
    int x, y;
};

// Cấu trúc một cạnh của đa giác phục vụ dòng quét
struct Edge {
    int yMin;
    int yMax;
    float xCurrent; // Hoành độ giao điểm tại yMin
    float invSlope; // 1/m = (x2 - x1) / (y2 - y1) (Slide 82)
};

// Thuật toán Dòng quét chi tiết bám theo Slide 80 - 82
void ScanlineFill(const vector<Point>& vertices, int fillColour) {
    int n = vertices.size();
    if (n < 3) return;

    // Bước 1: Tìm yMin và yMax của toàn bộ đa giác (Slide 80)
    int ybottom = vertices[0].y;
    int ytop    = vertices[0].y;
    for (int i = 1; i < n; i++) {
        if (vertices[i].y < ybottom) ybottom = vertices[i].y;
        if (vertices[i].y > ytop)    ytop    = vertices[i].y;
    }

    cout << "\n[Scanline] Pham vi dong quet: ybottom = " << ybottom << " -> ytop = " << ytop << "\n";

    // Khởi tạo danh sách các cạnh
    vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        Point p1 = vertices[i];
        Point p2 = vertices[(i + 1) % n];

        // Bỏ qua cạnh nằm ngang song song dòng quét vì không tạo khoảng cắt đứng
        if (p1.y == p2.y) continue;

        Edge e;
        if (p1.y < p2.y) {
            e.yMin = p1.y;
            e.yMax = p2.y;
            e.xCurrent = (float)p1.x;
        } else {
            e.yMin = p2.y;
            e.yMax = p1.y;
            e.xCurrent = (float)p2.x;
        }
        // Gia số 1/m theo công thức Slide 82: 1/m = (x2 - x1) / (y2 - y1)
        e.invSlope = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
        edges.push_back(e);
    }

    // Bước 2: Quét từng dòng y = k từ ybottom đến ytop (Slide 80)
    for (int k = ybottom; k <= ytop; k++) {
        vector<float> intersections;

        // Tìm tất cả hoành độ giao điểm của dòng quét y = k với các cạnh
        // Áp dụng quy tắc khoảng [yMin, yMax) để xử lý chuẩn xác Slide 81 (đỉnh đa giác)
        for (const auto& e : edges) {
            if (k >= e.yMin && k < e.yMax) {
                // Tính giao điểm bằng công thức gia số: x = xMin + (k - yMin) * (1/m)
                float xIntersect = e.xCurrent + (k - e.yMin) * e.invSlope;
                intersections.push_back(xIntersect);
            }
        }

        // Sắp xếp các hoành độ giao điểm theo thứ tự tăng dần (Slide 80)
        sort(intersections.begin(), intersections.end());

        // In nhật ký tính toán giao điểm tại dòng y = k
        if (!intersections.empty()) {
            cout << "Dong quet y = " << setw(2) << k << " | So giao diem: " << intersections.size() << " | x = [ ";
            for (float x : intersections) cout << fixed << setprecision(1) << x << " ";
            cout << "] -> To cac doan: ";
        }

        // Tô màu các đoạn thẳng giữa các cặp giao điểm (x1, x2), (x3, x4)... (Slide 80)
        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int xStart = (int)ceil(intersections[i]);
            int xEnd   = (int)floor(intersections[i + 1]);
            cout << "[" << xStart << ", " << xEnd << "] ";
            for (int x = xStart; x <= xEnd; x++) {
                putpixel(x, k, fillColour);
            }
        }
        if (!intersections.empty()) cout << "\n";
    }
}

// =============================================================================
// HÀM MAIN KIỂM THỬ TRỰC QUAN CẢ 2 THUẬT TOÁN
// =============================================================================
int main() {
    cout << "====================================================================\n";
    cout << "   CHUONG TRINH MINH HOA 2 THUAT TOAN TO MAU (THEO SLIDE CG.3-2D)   \n";
    cout << "====================================================================\n";

    // Thiết lập một đa giác mẫu: Đa giác 5 đỉnh lõm (Hơi giống chữ V/ngôi sao)
    vector<Point> polygon = {
        {4, 2},
        {25, 2},
        {22, 13},
        {14, 7},   // Đỉnh lõm
        {6, 13}
    };

    // -------------------------------------------------------------------------
    // TEST 1: THUẬT TOÁN TÔ MÀU THEO ĐƯỜNG BIÊN (BOUNDARY FILL - STACK)
    // -------------------------------------------------------------------------
    clearCanvas();
    // Vẽ khung biên đa giác bằng thuật toán Bresenham
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        drawLineBresenham(polygon[i].x, polygon[i].y, 
                          polygon[(i + 1) % n].x, polygon[(i + 1) % n].y, 
                          COLOR_BOUNDARY);
    }
    printCanvas("1. KHUNG BIEN DA GIAC BAN DAU");

    // Chọn điểm hạt giống Seed Point chắc chắn nằm trong đa giác: (14, 4)
    Point seed = {14, 4};
    cout << "\n--> Dang thuc hien Boundary Fill bang Stack voi Seed Point = (" << seed.x << ", " << seed.y << ")...\n";
    BoundaryFill_Stack(seed.x, seed.y, COLOR_FILL, COLOR_BOUNDARY);
    printCanvas("2. KET QUA SAU KHI TO BANG BOUNDARY FILL (STACK)");

    // -------------------------------------------------------------------------
    // TEST 2: THUẬT TOÁN TÔ MÀU THEO DÒNG QUÉT (SCANLINE FILL)
    // -------------------------------------------------------------------------
    clearCanvas();
    cout << "\n--> Dang thuc hien Scanline Fill tren tap dinh da giac...\n";
    ScanlineFill(polygon, COLOR_FILL);

    // Vẽ lại đường viền để người xem nhìn rõ hình dáng
    for (int i = 0; i < n; i++) {
        drawLineBresenham(polygon[i].x, polygon[i].y, 
                          polygon[(i + 1) % n].x, polygon[(i + 1) % n].y, 
                          COLOR_BOUNDARY);
    }
    printCanvas("3. KET QUA SAU KHI TO BANG SCANLINE FILL");

    cout << "\n>>> Hoan tat minh hoa! Ban co the dung ket qua va code nay de thuyet trinh.\n";
    return 0;
}
