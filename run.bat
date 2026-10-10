@echo off
cd /d "%~dp0"
cls
echo ========================================================
echo        CHUONG TRINH DO HOA MAY TINH (OPENGL C++)
echo ========================================================
echo  Ban muon chay bai Lab nao?
echo.
echo   [0] Lab 0: Vat the 2D ^& 3D (Tranh 2D, Xe hoi 3D, May bay 3D)
echo   [1] Lab 1: Thuat toan Bresenham ^& Midpoint (Doan thang, Tron, Ellipse...)
echo   [2] Lab 2: Xen hinh (Cohen-Sutherland, Midpoint) ^& To mau (Boundary, Scanline)
echo   [3] Lab 3: Bien doi 2D (Tinh tien, Ti le, Quay, Doi xung y=ax+b)
echo   [E] Thoat
echo ========================================================
set /p choice="Nhap lua chon cua ban (0, 1, 2 hoac 3): "

if not exist bin mkdir bin

if "%choice%"=="0" (
    if not exist "bin\Lab0_OpenGL.exe" (
        echo Chua co file bin\Lab0_OpenGL.exe. Dang tu dong bien dich Lab 0...
        g++ -std=c++11 -O2 -Isrc/Lab0 src/Lab0/main.cpp src/Lab0/Window.cpp src/Lab0/Camera.cpp src/Lab0/Shapes.cpp -o bin/Lab0_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    )
    start "" "bin\Lab0_OpenGL.exe"
) else if "%choice%"=="1" (
    if not exist "bin\Lab1_OpenGL.exe" (
        echo Chua co file bin\Lab1_OpenGL.exe. Dang tu dong bien dich Lab 1...
        g++ -std=c++11 -O2 -Isrc/Lab1 src/Lab1/main.cpp src/Lab1/Window.cpp src/Lab1/Algorithms.cpp -o bin/Lab1_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    )
    start "" "bin\Lab1_OpenGL.exe"
) else if "%choice%"=="2" (
    if not exist "bin\Lab2_OpenGL.exe" (
        echo Chua co file bin\Lab2_OpenGL.exe. Dang tu dong bien dich Lab 2...
        g++ -std=c++11 -O2 -Isrc/Lab2 src/Lab2/main.cpp src/Lab2/Window.cpp src/Lab2/Clipping.cpp src/Lab2/Filling.cpp -o bin/Lab2_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32 -Wl,--stack,67108864
    )
    start "" "bin\Lab2_OpenGL.exe"
) else if "%choice%"=="3" (
    if not exist "bin\Lab3_OpenGL.exe" (
        echo Chua co file bin\Lab3_OpenGL.exe. Dang tu dong bien dich Lab 3...
        g++ -std=c++11 -O2 -Isrc/Lab3 src/Lab3/main.cpp src/Lab3/Window.cpp src/Lab3/Transform2D.cpp -o bin/Lab3_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    )
    start "" "bin\Lab3_OpenGL.exe"
) else (
    exit /b
)
