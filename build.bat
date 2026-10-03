@echo off
REM ====================================================================
REM Interactive 3D Solar System and Space Exploration Simulator
REM Build script for Windows using GCC (MSYS2 UCRT64)
REM Week 6: Lighting & Shading
REM ====================================================================

setlocal

REM Ensure MSYS2 UCRT64 toolchain is accessible
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

if not exist bin (
    mkdir bin
)

echo [BUILD] Compiling 3D Solar System Simulator (Week 6: Lighting and Shading)...
gcc -Wall -Wextra -std=c99 src\main.c src\camera.c src\sphere.c src\lighting.c src\sun.c src\planet.c src\orbit.c src\texture.c src\ui.c -Iinclude -o bin\solar_sim.exe -lfreeglut -lopengl32 -lglu32
if %ERRORLEVEL% equ 0 (
    echo [BUILD] Build successful! Binary: bin\solar_sim.exe
) else (
    echo [BUILD] Compilation failed with exit code %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
)

endlocal
