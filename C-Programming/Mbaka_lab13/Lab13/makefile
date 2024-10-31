
CC = gcc
CFLAGS = -g -Wall -pedantic -O0 -std=c99
LDFLAGS = -lpthread
OBJ = $(SRC:.c=.o)
PROG= hw13
SRC = hw13.c
VALGRIND = valgrind --tool=memcheck --leak-check=yes --error-exitcode=1
OUT = out.txt
MEM = mem.txt

.SILENT: 
.PHONY: all test mem help clean


all: $(PROG)

$(PROG): $(SRC)
	$(CC) $(CFLAGS) -c  $^ $(LDFLAGS) -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(PROG)
	
	./$(PROG) -t 1 -s -v >> $(OUT) 2>&1
	
	./$(PROG) -t 5 -s >> $(OUT) 2>&1
	
	./$(PROG) -t 8 -v -s >> $(OUT) 2>&1

	./$(PROG) -t 8 -m -s >> $(OUT) 2>&1

	-./$(PROG) -t 0 >> $(OUT) 2>&1
	
	-./$(PROG) -t 9 >> $(OUT) 2>&1

mem: $(PROG)
	$(VALGRIND) ./$(PROG) -t 8 -f -v > $(MEM) 2>&1

help:
	echo "Usage:"
	echo "  make        - Builds the default PROG (all)."
	echo "  make all    - Compiles and builds the entire project."
	echo "  make test   - Runs various tests and appends the output to 'out.txt'."
	echo "  make mem    - Runs memory check using Valgrind and outputs to 'mem.txt'."
	echo "  make clean  - Cleans up the project by removing executables and temporary files."
	echo "  make help   - Displays this help."

clean:
	rm -f $(PROG) $(OUT) $(MEM)
