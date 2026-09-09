@echo off
cd /d "%~dp0"

if exist "bin\DHMT_OpenGL.exe" (
    start "" "bin\DHMT_OpenGL.exe"
) else (
    echo Chua tim thay bin\DHMT_OpenGL.exe. Dang tien hanh bien dich...
    call build.bat
)
