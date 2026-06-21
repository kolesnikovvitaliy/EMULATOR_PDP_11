TARGET = pdp.out
all: format_code_webfit_style $(TARGET) doxygen

DOXYFILE = Doxyfile
DOC_DIR = doc/html/

CC = gcc

CFLAGS = -O2 -g3 -Wall -Wextra -Wpedantic -std=c11 -Iinclude -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough\
	-fsanitize=address -fsanitize=undefined\
	-Werror=format-security -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS\
	-Warray-bounds -fstack-clash-protection -fstack-protector-strong -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro \
	-Wl,-z,now

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

SRC_DIRS := $(shell find $(SRC_DIR) -type d)
SRC_FILES := $(shell find . -type f -name "*.c")

OBJS = $(patsubst $(SRC_DIRS)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
#DEPS = $(wildcard $(INC_DIR)/*.h)
DEPS = $(shell find $(INC_DIR) -type f -name "*.h")

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm -rf $(DOC_DIR)

format_code_webfit_style:
	$(shell find . -iname "*.c" -o -iname "*.h" | xargs clang-format -i)

doxygen:
	doxygen $(DOXYFILE) > /dev/null 2>&1



.PHONY: all clean format_code_webfit_style doxygen
