CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g -O1
LDFLAGS = -lm
VERBOSE =

SRC = hw9.c poly.c
OBJ = $(SRC:.c=.o)
EXEC = hw9
OUT = out.txt
MEM = mem.txt

.SILENT:
.PHONY: all tests mem help clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(EXEC)
	./$(EXEC) -input simple.txt $(VERBOSE) >> $(OUT) 
	./$(EXEC) -input polynomials.txt $(VERBOSE) >> $(OUT)

mem: $(EXEC)
	valgrind --leak-check=full ./$(EXEC) -input simple.txt $(VERBOSE) > $(MEM) 2>&1
	valgrind --leak-check=full ./$(EXEC) -input polynomials.txt $(VERBOSE) >> $(MEM) 2>&1


help:
	@echo "Makefile usage:"
	@echo "  make all    - Build the hw9 executable"
	@echo "  make tests  - Run hw9 with simple.txt and polynomials.txt, append output to out.txt"
	@echo "  make mem    - Check hw9 with polynomials.txt for memory leaks using Valgrind, output to mem.txt"
	@echo "  make help   - Show this help message"
	@echo "  make clean  - Remove all generated files"
	@echo "Set VERBOSE=-verb to enable verbose mode in the hw9 program."

clean:
	rm -f $(OBJ) $(EXEC) $(OUT)  $(MEM)
