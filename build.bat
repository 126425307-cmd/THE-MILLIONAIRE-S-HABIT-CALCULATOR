@echo off
REM --- Настройваме конзолата да поддържа кирилица (UTF-8) ---
chcp 65001 > nul

REM --- Прост скрипт за изграждане под Windows ---

REM 1. Създаване на директория, ако не съществуват
REM (bin = за изпълнимия файл)
if not exist bin mkdir bin

REM 2. Компилиране на кода
echo.
echo Компилиране на проекта...
gcc src/main.c src/financials.c -Iinclude -o bin/millionaire_calc.exe

REM 3. Проверка дали компилацията е успешна
REM %errorlevel% е променлива, която пази резултата от последната команда (GCC)
REM 0 = Успех, Всичко друго = Грешка
if %errorlevel% neq 0 (
    echo.
    echo [ГРЕШКА] Компилацията беше неуспешна!
    echo Моля, проверете кода за грешки.
    exit /b %errorlevel%
)

echo [УСПЕХ] Изграждането завърши успешно.
echo ==========================================

REM 4. Стартиране на програмата веднага
.\bin\millionaire_calc.exe