#
# Makefile
# Version 20170714
# Written by Harry Wong (RedAndBlueEraser)
#

# Compiler and compiler flags.
CC = gcc
CFLAGS = -Wall -ansi -pedantic

# Source, object and executable files.
SRC = main.c sort.c
OBJ = main.o sort.o
EXE = main

# Create executable file.
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# Clean: Remove object files.
clean:
	$(RM) $(OBJ)

# Clobber: Performs Clean and remove executable file.
clobber: clean
	$(RM) $(EXE)

# Test: Run the executable file three times.
test:
	./$(EXE)
	./$(EXE)
	./$(EXE)

# Dependencies.
sort.o: Makefile sort.h
main.o: Makefile sort.h
