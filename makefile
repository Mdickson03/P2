
# Compiler and flags
CC = gcc

# Wall enables common warnings, g is for debugging
CFLAGS = -Wall -g

# Files and targets
TARGET = words

# Target builds the program
all: $(TARGET)

# Compile the library words.c with the header
$(TARGET): words.c
	$(CC) $(CFLAGS) -o $(TARGET) words.c