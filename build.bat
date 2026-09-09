@echo off
cd /d "%~dp0"

echo ========================================================
echo   DANG BIEN DICH DU AN OPENGL (DO HOA MAY TINH)...
echo ========================================================

if not exist bin mkdir bin

g++ -std=c++11 -O2 -Isrc src/main.cpp src/Window.cpp src/Camera.cpp src/Shapes.cpp -o bin/DHMT_OpenGL.exe -lopengl32 -lglu32 -lgdi32 -luser32

if %ERRORLEVEL% equ 0 (
    echo.
    echo [THANH CONG] File thuc thi da tao tai: bin\DHMT_OpenGL.exe
    echo Nhan phim bat ky de chay chuong trinh...
    pause
    start "" "bin\DHMT_OpenGL.exe"
) else (
    echo.
    echo [THAT BAI] Co loi trong qua trinh bien dich!
    pause
)
