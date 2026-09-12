@echo off
cd /d "%~dp0"
cls
echo ========================================================
echo        BIEN DICH DO HOA MAY TINH (OPENGL C++)
echo ========================================================
echo  Ban muon bien dich bai Lab nao?
echo.
echo   [0] Bien dich Lab 0: Vat the 2D ^& 3D
echo   [1] Bien dich Lab 1: Thuat toan Bresenham ^& Midpoint
echo   [A] Bien dich ca hai (Lab 0 va Lab 1)
echo   [E] Thoat
echo ========================================================
set /p choice="Nhap lua chon cua ban (0, 1 hoac A): "

if not exist bin mkdir bin

if "%choice%"=="0" (
    echo.
    echo --- Dang bien dich Lab 0... ---
    g++ -std=c++11 -O2 -Isrc/Lab0 src/Lab0/main.cpp src/Lab0/Window.cpp src/Lab0/Camera.cpp src/Lab0/Shapes.cpp -o bin/Lab0_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 (
        echo [THANH CONG] File thuc thi: bin\Lab0_OpenGL.exe
        echo Nhan phim bat ky de chay Lab 0...
        pause
        start "" "bin\Lab0_OpenGL.exe"
    ) else (
        echo [THAT BAI] Co loi trong qua trinh bien dich!
        pause
    )
) else if "%choice%"=="1" (
    echo.
    echo --- Dang bien dich Lab 1... ---
    g++ -std=c++11 -O2 -Isrc/Lab1 src/Lab1/main.cpp src/Lab1/Window.cpp src/Lab1/Algorithms.cpp -o bin/Lab1_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 (
        echo [THANH CONG] File thuc thi: bin\Lab1_OpenGL.exe
        echo Nhan phim bat ky de chay Lab 1...
        pause
        start "" "bin\Lab1_OpenGL.exe"
    ) else (
        echo [THAT BAI] Co loi trong qua trinh bien dich!
        pause
    )
) else if /i "%choice%"=="A" (
    echo.
    echo --- Dang bien dich Lab 0... ---
    g++ -std=c++11 -O2 -Isrc/Lab0 src/Lab0/main.cpp src/Lab0/Window.cpp src/Lab0/Camera.cpp src/Lab0/Shapes.cpp -o bin/Lab0_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 ( echo [THANH CONG] Lab 0: bin\Lab0_OpenGL.exe ) else ( echo [THAT BAI] Lab 0 loi! )

    echo.
    echo --- Dang bien dich Lab 1... ---
    g++ -std=c++11 -O2 -Isrc/Lab1 src/Lab1/main.cpp src/Lab1/Window.cpp src/Lab1/Algorithms.cpp -o bin/Lab1_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 ( echo [THANH CONG] Lab 1: bin\Lab1_OpenGL.exe ) else ( echo [THAT BAI] Lab 1 loi! )

    echo.
    echo Hoan tat bien dich ca 2 Lab.
    pause
) else (
    exit /b
)
