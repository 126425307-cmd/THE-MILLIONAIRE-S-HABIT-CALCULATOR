#!/bin/bash

# 1. Създаване на папки (-p означава "само ако липсва")
mkdir -p bin

# 2. Компилиране
echo ""
echo "[Linux/Mac] Компилиране на проекта..."
gcc src/main.c src/financials.c -Iinclude -o bin/millionaire_calc

# 3. Проверка за грешки ($? е кодът за грешка в Unix)
if [ $? -ne 0 ]; then
    echo "[ГРЕШКА] Компилацията се провали!"
    exit 1
fi

# 4. Стартиране
echo "[УСПЕХ] Стартиране..."
echo "=========================================="
./bin/millionaire_calc