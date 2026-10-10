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
echo   [2] Bien dich Lab 2: Xen doan thang ^& To mau da giac
echo   [3] Bien dich Lab 3: Cac phep bien doi 2D (Ma tran 3x3)
echo   [A] Bien dich tat ca (Lab 0, Lab 1, Lab 2, Lab 3)
echo   [E] Thoat
echo ========================================================
set /p choice="Nhap lua chon cua ban (0, 1, 2, 3 hoac A): "

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
) else if "%choice%"=="2" (
    echo.
    echo --- Dang bien dich Lab 2... ---
    g++ -std=c++11 -O2 -Isrc/Lab2 src/Lab2/main.cpp src/Lab2/Window.cpp src/Lab2/Clipping.cpp src/Lab2/Filling.cpp -o bin/Lab2_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32 -Wl,--stack,67108864
    if %ERRORLEVEL% equ 0 (
        echo [THANH CONG] File thuc thi: bin\Lab2_OpenGL.exe
        echo Nhan phim bat ky de chay Lab 2...
        pause
        start "" "bin\Lab2_OpenGL.exe"
    ) else (
        echo [THAT BAI] Co loi trong qua trinh bien dich!
        pause
    )
) else if "%choice%"=="3" (
    echo.
    echo --- Dang bien dich Lab 3... ---
    g++ -std=c++11 -O2 -Isrc/Lab3 src/Lab3/main.cpp src/Lab3/Window.cpp src/Lab3/Transform2D.cpp -o bin/Lab3_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 (
        echo [THANH CONG] File thuc thi: bin\Lab3_OpenGL.exe
        echo Nhan phim bat ky de chay Lab 3...
        pause
        start "" "bin\Lab3_OpenGL.exe"
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
    echo --- Dang bien dich Lab 2... ---
    g++ -std=c++11 -O2 -Isrc/Lab2 src/Lab2/main.cpp src/Lab2/Window.cpp src/Lab2/Clipping.cpp src/Lab2/Filling.cpp -o bin/Lab2_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32 -Wl,--stack,67108864
    if %ERRORLEVEL% equ 0 ( echo [THANH CONG] Lab 2: bin\Lab2_OpenGL.exe ) else ( echo [THAT BAI] Lab 2 loi! )

    echo.
    echo --- Dang bien dich Lab 3... ---
    g++ -std=c++11 -O2 -Isrc/Lab3 src/Lab3/main.cpp src/Lab3/Window.cpp src/Lab3/Transform2D.cpp -o bin/Lab3_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32
    if %ERRORLEVEL% equ 0 ( echo [THANH CONG] Lab 3: bin\Lab3_OpenGL.exe ) else ( echo [THAT BAI] Lab 3 loi! )

    echo.
    echo Hoan tat bien dich tat ca cac bai Lab!
    pause
) else (
    exit /b
)
