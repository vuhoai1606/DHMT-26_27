#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

// Cấu trúc điểm
struct Point
{
    int x, y;
};

// Khai báo kích thước cửa sổ xén (Clipping Window)
const int X_MIN = 50;
const int X_MAX = 200;
const int Y_MIN = 50;
const int Y_MAX = 150;

// Hàm gán mã 4-bit cho một điểm
int getCode(Point p)
{
    int code = 0;
    if (p.x < X_MIN)
        code |= 1; // Bit 0: Left
    if (p.x > X_MAX)
        code |= 2; // Bit 1: Right
    if (p.y < Y_MIN)
        code |= 4; // Bit 2: Bottom
    if (p.y > Y_MAX)
        code |= 8; // Bit 3: Top
    return code;
}

// Hàm giả lập vẽ đoạn thẳng
void drawLine(Point A, Point B)
{
    cout << ">> Ve doan thang giu lai: ("
         << A.x << ", " << A.y << ") -> ("
         << B.x << ", " << B.y << ")\n";
}

// Thuật toán xén đoạn thẳng chia nhị phân (Midpoint Subdivision)
void BinaryClip(Point A, Point B)
{
    Point P, Q, M;

    if ((getCode(A) | getCode(B)) == 0)
    {
        drawLine(A, B);
        return;
    }

    if ((getCode(A) & getCode(B)) != 0)
    {
        cout << "-- Doan thang nam hoan toan ben ngoai, loai bo.\n";
        return;
    }

    if (getCode(A) != 0 && getCode(B) == 0)
    {
        swap(A, B);
    }

    if (getCode(A) == 0 && getCode(B) != 0)
    {
        P = A; 
        Q = B;

        // Lặp chia đôi đến khi khoảng cách Manhattan <= 2 pixel (hội tụ giao điểm)
        while ((abs(P.x - Q.x) + abs(P.y - Q.y)) > 2)
        {
            M.x = (P.x + Q.x) / 2;
            M.y = (P.y + Q.y) / 2;

            if (getCode(M) == 0)
            {
                P = M; 
            }
            else
            {
                Q = M; 
            }
        }
        drawLine(A, P);
        return;
    }

    if (getCode(A) != 0 && getCode(B) != 0 && ((getCode(A) & getCode(B)) == 0))
    {
        P = A;
        Q = B;
        M.x = (P.x + Q.x) / 2;
        M.y = (P.y + Q.y) / 2;

        // Tìm một điểm M nằm bên trong cửa sổ (Code(M) == 0)
        while ((getCode(M) != 0) && ((abs(P.x - Q.x) + abs(P.y - Q.y)) > 2))
        {
            if ((getCode(P) & getCode(M)) != 0)
            {
                P = M;
            }
            else
            {
                Q = M;
            }
            M.x = (P.x + Q.x) / 2;
            M.y = (P.y + Q.y) / 2;
        }

        if (getCode(M) == 0)
        {
            BinaryClip(P, M);
            BinaryClip(M, Q);
        }
        else
        {
            cout << "-- Doan thang khong cat qua cua so, loai bo.\n";
        }
    }
}

int main()
{
    cout << "=== CUA SO XEN: [" << X_MIN << ", " << X_MAX
         << "] x [" << Y_MIN << ", " << Y_MAX << "] ===\n\n";

    // Test 1: Nằm hoàn toàn bên trong
    cout << "Test 1: Doan (60, 60) -> (180, 140)\n";
    BinaryClip({60, 60}, {180, 140});

    // Test 2: Nằm hoàn toàn bên ngoài (cùng phía trên)
    cout << "\nTest 2: Doan (60, 180) -> (180, 200)\n";
    BinaryClip({60, 180}, {180, 200});

    // Test 3: Một điểm trong, một điểm ngoài
    cout << "\nTest 3: Doan (100, 100) -> (250, 100)\n";
    BinaryClip({100, 100}, {250, 100});

    // Test 4: Cả 2 điểm đều ở ngoài, cắt ngang qua cửa sổ
    cout << "\nTest 4: Doan (20, 100) -> (250, 100)\n";
    BinaryClip({20, 100}, {250, 100});

    return 0;
}