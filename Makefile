# Makefile for Banking Management System

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

# Source and output files
SOURCE = banking_system.c
OUTPUT = banking_system

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    RM = rm -f
endif
ifeq ($(UNAME_S),Darwin)
    RM = rm -f
endif
ifeq ($(OS),Windows_NT)
    RM = del
    OUTPUT = banking_system.exe
endif

# Default target
all: $(OUTPUT)

# Compilation target
$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCE) $(LDFLAGS)
	@echo "Compilation successful! Run with: ./$(OUTPUT)"

# Run the program
run: $(OUTPUT)
	./$(OUTPUT)

# Clean build artifacts
clean:
	$(RM) $(OUTPUT)
	@echo "Clean complete"

# Clean all including data files
distclean: clean
	$(RM) credit.dat transactions.txt accounts.txt
	@echo "All data files removed"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Compile the program (default)"
	@echo "  run       - Compile and run the program"
	@echo "  clean     - Remove compiled executable"
	@echo "  distclean - Remove executable and all data files"
	@echo "  help      - Show this help message"

.PHONY: all run clean distclean help
