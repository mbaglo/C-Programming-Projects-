CC = gcc
CFLAGS = -O1 -g -Wall -std=c99 -pedantic 
SRCS = hw7.c rootfinding.c eqn2solve.c
OBJS = $(SRCS:.c=.o)
PROG = hw7
LIBS = -lm
OUT = out.txt
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=1
MEM = mem.txt

.SILENT:

.PHONY: all

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.c.o:
	@echo "Compiling '$*'"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test
test:
	
	@echo "Verbose Bisection" > $(OUT)
	@echo "Test for bisection [-2.5, 2.5]" >> $(OUT)
	./$(PROG) --bisection --tolerance 0.000001 --guess1 -2.5 --guess2 2.5 --verbose >> $(OUT) 2>&1
	@echo >> $(OUT)
	@echo "Newton:" >> $(OUT)
	@echo "Test 1 for newton x0 = -1.5" >> $(OUT)
	./$(PROG) --newton --tolerance 0.000001 --guess1 -1.5 --verbose >> $(OUT) 2>&1
	
	@echo >> $(OUT)
	@echo "Test 2 for newton x0 = 1.5" >> $(OUT)
	./$(PROG) --newton --tolerance 0.000001 --guess1 1.5 --verbose >> $(OUT) 2>&1

	@echo >> $(OUT)
	@echo "Secant:" >> $(OUT)
	@echo "Test 1 for secant x0 = -1.5, x1 = -1.6" >> $(OUT)
	./$(PROG) --secant --tolerance 0.000001 --guess1 -1.5 --guess2 -1.6 --verbose >> $(OUT) 2>&1

	@echo >> $(OUT)
	@echo "Test 2 for secant x0 = 1.5, x1 = 1.6" >> $(OUT)
	./$(PROG) --secant --tolerance 0.000001 --guess1 1.5 --guess2 1.6 --verbose >> $(OUT) 2>&1

.PHONY: mem
mem:
	$(VALGRIND) ./$(PROG)  > $(MEM) 2>&1

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS) $(OUT)
