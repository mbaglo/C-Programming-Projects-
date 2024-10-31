CC = gcc
CFLAGS = -O1 -g -Wall -std=c99 -pedantic
LDFLAGS = -lm -lgsl -lgslcblas
VALGRIND = valgrind --tool=memcheck --leak-check=full --quiet --show-leak-kinds=all --track-origins=yes

# Name of the executable
PROG = hw12

# Source files
SRCS = hw12.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
MEM = mem.txt
OUT = out.txt
RAND = rand.txt

.SILENT:

.PHONY: all tests mem clean help

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(PROG)
	@for i in $$(seq 0 12); do \
        ./$(PROG) --input ge$$i.txt --verbose --data >> $(OUT) 2>&1; \
    done

mem: $(PROG) genRand
	./genRand > $(RAND)
	$(VALGRIND) ./$(PROG) --input $(RAND) --verbose --data > $(MEM) 2>&1

x:
	chmod +x ./genRand

help:
	@echo "Available targets:"
	@echo "  all   - Builds the main application."
	@echo "  tests - Runs all test cases and outputs the results to $(OUT)."
	@echo "  mem   - Runs the program through Valgrind with $(RAND) and outputs to $(MEM)."
	@echo "  clean - Removes all built files."
	@echo "  help  - Displays this help."

clean:
	rm -f $(PROG) $(OBJS) $(OUT) $(MEM) $(RAND)
