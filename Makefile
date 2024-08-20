## Makefile

# Define the compiler
CC = gcc

# Define the flags 
CFLAGS = -Wall -Wextra -ggdb -mconsole

# Define the dirs
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build

# Define the target executable
TARGET = $(BUILD_DIR)/main.exe

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate object files in the obj directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default rule to build the target
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Rule to compile .c files to .o files in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Custom rule to compile and run the program with arguments
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Clean up the build files
.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rmdir $(OBJ_DIR) $(BUILD_DIR) 2>/dev/null || true