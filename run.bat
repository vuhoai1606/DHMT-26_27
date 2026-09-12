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
echo   [E] Thoat
echo ========================================================
set /p choice="Nhap lua chon cua ban (0 hoac 1): "

if "%choice%"=="0" (
    if not exist "bin\Lab0_OpenGL.exe" (
        echo Chua co file bin\Lab0_OpenGL.exe. Dang tu dong bien dich Lab 0...
        if not exist bin mkdir bin
        g++ -std=c++11 -O2 -Isrc/Lab0 src/Lab0/main.cpp src/Lab0/Window.cpp src/Lab0/Camera.cpp src/Lab0/Shapes.cpp -o bin/Lab0_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    )
    start "" "bin\Lab0_OpenGL.exe"
) else if "%choice%"=="1" (
    if not exist "bin\Lab1_OpenGL.exe" (
        echo Chua co file bin\Lab1_OpenGL.exe. Dang tu dong bien dich Lab 1...
        if not exist bin mkdir bin
        g++ -std=c++11 -O2 -Isrc/Lab1 src/Lab1/main.cpp src/Lab1/Window.cpp src/Lab1/Algorithms.cpp -o bin/Lab1_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    )
    start "" "bin\Lab1_OpenGL.exe"
) else (
    exit /b
)
