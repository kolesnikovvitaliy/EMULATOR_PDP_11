# 1. Определение операционной системы
ifeq ($(OS),Windows_NT)
    # Настройки для Windows (MinGW/Cygwin/MSYS2)
    TARGET_EXT = .exe
    # Проверяем, запущен ли Make в окружении MSYS/Cygwin (где есть rm и mkdir)
    # Если запущен в обычном cmd.exe, используем встроенные команды Windows
    ifeq ($(SHELL),amd64.exe)
        FIX_PATH = $(subst /,\,$1)
        RM = del /Q /F
        RMDIR = rmdir /S /Q
        MKDIR = mkdir
    else ifeq ($(SHELL),cmd.exe)
        FIX_PATH = $(subst /,\,$1)
        RM = del /Q /F
        RMDIR = rmdir /S /Q
        MKDIR = mkdir
    else
        # Окружение со shell-эмуляцией (Cygwin, MSYS2, Git Bash)
        FIX_PATH = $1
        RM = rm -f
        RMDIR = rm -rf
        MKDIR = mkdir -p
    endif
else
    # Настройки для Linux / macOS
    TARGET_EXT = .out
    FIX_PATH = $1
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
endif


TARGET = pdp$(TARGET_EXT)

.PHONY: all format_code_webfit_style doxygen clean
all: format_code_webfit_style $(TARGET) clean doxygen

DOXYFILE = Doxyfile
DOC_DIR = doc/html/

CC = gcc

CFLAGS = -O2 -g3 -Wall -Wextra -Wpedantic -Wpointer-arith -Wstrict-prototypes -Wtautological-compare -Wuninitialized \
	-std=c11 -Iinclude -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough\
	-fsanitize=address -fsanitize=undefined -static-libasan -static-libubsan\
	-Werror=format-security -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS\
	-Warray-bounds -fstack-clash-protection -fstack-protector-strong -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro \
	-Wl,-z,now

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Рекурсивный поиск файлов (функция rwildcard)
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# 1. Находим вообще ВСЕ файлы .c в текущем каталоге и подкаталогах
ALL_C_FILES := $(call rwildcard,.,*.c)

# 2. ИСКЛЮЧАЕМ из поиска файлы, которые могут случайно оказаться в папке build
SRC_FILES   := $(filter-out $(OBJ_DIR)/%, $(ALL_C_FILES))

# 3. Находим заголовочные файлы
DEPS        := $(call rwildcard,$(INC_DIR),*.h)

# 4. Превращаем пути исходников в пути для объектных файлов внутри build/
# Элегантно обрабатывает файлы как в корне (./main.c -> build/main.o), так и в подпапках
OBJS := $(patsubst ./%, $(OBJ_DIR)/%, $(SRC_FILES:.c=.o))

# Главная цель сборки
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Универсальное шаблонное правило компиляции.
# На входе принимает 'build/path/file.o', а компилирует './path/file.c'
$(OBJ_DIR)/%.o: ./%.c $(DEPS)
# 	@$(call MKDIR,$(dir $@))
	@$(MKDIR) $(call FIX_PATH,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

# Кросплатформенная очистка
clean:
	@$(call RM, $(subst /,$(if $(filter Windows_NT,$(OS)),\,/),$(OBJS)))


# Объединяем все файлы, которые нужно отформатировать
FORMAT_FILES := $(SRC_FILES) $(DEPS)

format_code_webfit_style:
	clang-format -i $(FORMAT_FILES)

doxygen:
	doxygen $(DOXYFILE) > /dev/null 2>&1
