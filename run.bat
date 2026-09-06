@echo off
REM ====================================================================
REM Interactive 3D Solar System and Space Exploration Simulator
REM Execution script for Windows
REM ====================================================================

setlocal

REM Ensure MSYS2 UCRT64 runtime DLLs (e.g. libfreeglut) are in PATH
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

if not exist "bin\solar_sim.exe" (
    echo [RUN] Binary not found. Building project...
    call build.bat
    if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
)

echo [RUN] Launching Solar System Simulator...
bin\solar_sim.exe

endlocal
