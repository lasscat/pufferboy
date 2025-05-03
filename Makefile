# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g  # Enable warnings and debugging symbols

# List of source files
SRCS = main.c bus.c memory.c romloading.c cpu.c

# List of object files (same as SRCS but with .c replaced with .o)
OBJS = $(SRCS:.c=.o)

# Output executable name
EXEC = my_gameboy_emulator

# Default target to build the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Rule to compile each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild everything from scratch
rebuild: clean $(EXEC)

