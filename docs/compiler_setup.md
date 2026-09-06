# C Compiler Toolchain Setup & Verification

## Environment Summary

* **Compiler**: GCC (GNU Compiler Collection)
* **Distribution**: MSYS2 UCRT64 (x86_64-w64-mingw32)
* **Version**: 15.2.0 (Rev14, Built by MSYS2 project)
* **Architecture**: x86_64 (64-bit Windows)
* **C Standard**: C99 / C11 compatible

## Toolchain Path
To invoke the compiler from PowerShell or Command Prompt:
```powershell
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
gcc --version
```

## Compiler Selection Rationale
* **GCC via MSYS2 UCRT64** provides standard C runtime (Universal C Runtime - `ucrtbase.dll`), matching modern Windows standards.
* Native support for standard OpenGL (`-lopengl32`, `-lglu32`) and seamless package integration with FreeGLUT (`-lfreeglut`).
* Full 64-bit architecture support without legacy MSVCRT dependencies.
* No C++ toolchain required; pure C compiler toolchain.

## Compilation Test
Verified by compiling a standard C test:
```bash
gcc -std=c99 -Wall -Wextra -O2 test.c -o test.exe
```
Result: Successful compilation and execution with clean return status.
