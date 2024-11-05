
# Compiler and flags
CC = gcc

# Wall enables common warnings, g is for debugging
CFLAGS = -Wall -g

# Files and targets
LIBRARY = words.c
TARGET = hashmap

# Target builds the hashmap program
all: $(TARGET)

# Compile the library words.c with the header
$(LIBRARY): words.c
	$(CC) $(CFLAGS) -c words.c