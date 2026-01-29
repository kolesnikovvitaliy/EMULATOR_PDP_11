TARGET = pdp.exe

CC = gcc

CFLAGS = -O2 -g3 -Wall -Wextra -pedantic -std=c11 -Iinclude

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

.PHONY: all clean
