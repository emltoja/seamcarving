## Makefile

# Define the compiler
CC = gcc

# Define the flags 
CFLAGS = -Wall -Wextra -ggdb

# Define the target 
TARGET = main 

# Find all the .c files in the current directory 
SOURCES = $(wildcard *.c)

# Generate the object files from the .c files 
OBJECTS = $(SOURCES:.c=.o)

# Default rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

# Rule to compile the .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the files generated during the build
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)

# Rule to run the program with given args 
run: 
	.\\$(TARGET) $(ARGS)