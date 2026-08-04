#!/bin/bash

# Укажите путь к каталогу с файлами и имя вашей программы
TARGET_DIR="."
PROGRAM="../../../pdp.out"

# Определение цветов
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color (Сброс цвета)

# Проверяем, существует ли каталог
if [ ! -d "$TARGET_DIR" ]; then
    echo "Ошибка: Каталог $TARGET_DIR не найден."
    exit 1
fi

# Перебираем все файлы в каталоге
for file in "$TARGET_DIR"/*; do
    # Проверяем, что это действительно файл, а не подкаталог
    if [ -f "$file" ]; then
        clean_name=$(basename "$file")
        echo -e "           ${GREEN}$clean_name${NC}"

        # Запуск программы с одним файлом в качестве аргумента
        # Запуск с флагом -disable_logs выводиться на экран только напечатанные символы
        # Пример "Hello, world!\n"
        "$PROGRAM" -t "../../../test/test_repo/pdp11_tests_succes/$clean_name" 2>&1 | tail -n 8

        echo "--------------------------------------"
    fi
done

echo "Все файлы обработаны!"
