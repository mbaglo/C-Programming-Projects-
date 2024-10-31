CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g -O1
SRC = intTest.c rootfinding.c
OBJ = $(SRC:.c=.o)
EXEC1 = intTest
EXEC2 = intTest_mem
VERBOSE_FLAG =
TIMING_FLAG = -DEN_TIME

OUT = out.txt
MEM = mem.txt

.SILENT:
.PHONY: all test mem help clean

# Default target
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJ)
	$(CC) $(CFLAGS) $(TIMING_FLAG)  -o $@ $(OBJ) -lm

$(EXEC2): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

%.o: %.c
	$(CC) $(CFLAGS) $(TIMING_FLAG) -c $< -o $@

test: $(EXEC1)
	./$(EXEC1) $(VERBOSE_FLAG) > $(OUT) 2>&1

mem: $(EXEC2)
	valgrind --leak-check=full ./$(EXEC2) > $(MEM) 2>&1

help:
	@echo "all - compiles all executables with timing enabled for intTest and disabled for intTest_mem"
	@echo "test - runs intTest with timing enabled and outputs to out.txt"
	@echo "mem - runs intTest_mem under Valgrind to check for memory leaks"
	@echo "clean - removes all generated files"
	@echo "help - displays this help"

clean:
	rm -f $(OBJ) $(EXEC1) $(EXEC2) $(OUT) $(MEM)
