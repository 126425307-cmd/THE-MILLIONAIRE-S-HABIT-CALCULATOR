@echo off
REM --- Windows Build Script (ASCII Safe Mode) ---

REM 1. Create directories
if not exist bin mkdir bin

REM 2. Compile
echo Compiling project...
gcc src/main.c src/financials.c -Iinclude -o bin/millionaire_calc.exe

REM 3. Check for errors
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed!
    echo Please check your code.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Compilation complete. Starting app...
echo ==========================================

REM 4. Run the app
.\bin\millionaire_calc.exe
