CC = gcc
CFLAGS = -O1 -g -Wall -std=c99 -pedantic

# Macro for source and object files
SOURCES = TestSearch.c DynamicArrays.c LinkedLists.c
OBJECTS = $(SOURCES:.c=.o)
PROG = TestSearch
SEARCHWORD = space
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=1
ENGWORDS = engWords.txt
OUTPUT = out.txt
MEM = mem.txt

.SILENT:

.PHONY: all

all: $(PROG)

$(PROG): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	@echo "Compiling '$*'"
	$(CC) $(CFLAGS) $*.c -c

.PHONY: test
test:
	./$(PROG) $(ENGWORDS) $(SEARCHWORD) > $(OUTPUT) 2>&1
	./$(PROG) $(ENGWORDS) xyzzy >> $(OUTPUT) 2>&1

.PHONY: mem
mem:
	$(VALGRIND) ./$(PROG) $(ENGWORDS) $(SEARCHWORD) > $(MEM) 2>&1

.PHONY: help
help:
	@echo "Valid targets:"
	@echo "  all: builds the TestSearch program"
	@echo "  test: runs test cases"
	@echo "  mem: runs memory check"
	@echo "  clean: cleans up the directory"

clean:
	rm -f $(PROG) $(OBJECTS $(OUTPUT)  $(MEM)
