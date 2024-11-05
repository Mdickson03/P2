
# Compiler and flags
CC = gcc

# Wall enables common warnings, g is for debugging
CFLAGS = -Wall -g

# Files and targets
LIBRARY = words.c

# Target builds the main program
all: $(LIBRARY)

# Compile the library words.c with the header
$(LIBRARY): words.c words.h
	$(CC) $(CFLAGS) -c words.c