@echo off
REM Banking Management System - Windows Compilation Script
REM This script compiles the banking system on Windows

setlocal enabledelayedexpansion

echo.
echo ========================================
echo Banking Management System - Compiler
echo ========================================
echo.

REM Check if GCC is installed
gcc --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: GCC compiler not found!
    echo Please install MinGW or another GCC distribution.
    echo Download from: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo Compiler found:
gcc --version | findstr /R "^"

echo.
echo Compiling banking_system.c...
echo.

REM Compile the program
gcc -Wall -Wextra -std=c99 -o banking_system.exe banking_system.c -lm

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Compilation SUCCESSFUL!
    echo ========================================
    echo.
    echo Executable created: banking_system.exe
    echo.
    echo To run the program, type: banking_system.exe
    echo.
    pause
) else (
    echo.
    echo ========================================
    echo Compilation FAILED!
    echo ========================================
    echo.
    echo Please check the error messages above.
    echo.
    pause
    exit /b 1
)

endlocal
