CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
LIBS = -lm
SRC = hw8c.c hw8c_opt1.c hw8c_opt2.c hw8c_opt3.c
BIN = $(SRC:.c=)
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=1
OUT = out.txt

.SILENT:
.PHONY: all test mem help clean

all: $(BIN) $(addsuffix _0,$(BIN)) $(addsuffix _2,$(BIN)) $(addsuffix _3,$(BIN)) $(addsuffix _mem,$(BIN))

%_0: %.c
	$(CC) $(CFLAGS) -O0 -DEN_TIME -o $@ $< $(LIBS)

%_2: %.c
	$(CC) $(CFLAGS) -O2 -DEN_TIME -o $@ $< $(LIBS)

%_3: %.c
	$(CC) $(CFLAGS) -O3 -DEN_TIME -o $@ $< $(LIBS)

%_mem: %.c
	$(CC) $(CFLAGS) -O1 -o $@ $< $(LIBS)

test: all
	@echo "Testing hw8c" > $(OUT) 
	-./hw8c data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_0" >> $(OUT) 
	-./hw8c_0 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_2" >> $(OUT) 
	-./hw8c_2 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_3" >> $(OUT) 
	-./hw8c_3 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_mem" >> $(OUT) 
	-./hw8c_mem data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt1" >> $(OUT) 
	-./hw8c_opt1 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt1_0" >> $(OUT) 
	-./hw8c_opt1_0 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt1_2" >> $(OUT) 
	-./hw8c_opt1_2 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt1_3" >> $(OUT) 
	-./hw8c_opt1_3 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt1_mem" >> $(OUT) 
	-./hw8c_opt1_mem data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt2" >> $(OUT) 
	-./hw8c_opt2 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt2_0" >> $(OUT) 
	-./hw8c_opt2_0 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt2_2" >> $(OUT) 
	-./hw8c_opt2_2 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt2_3" >> $(OUT) 
	-./hw8c_opt2_3 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt2_mem" >> $(OUT) 
	-./hw8c_opt2_mem data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt3" >> $(OUT) 
	-./hw8c_opt3 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt3_0" >> $(OUT) 
	-./hw8c_opt3_0 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt3_2" >> $(OUT) 
	-./hw8c_opt3_2 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt3_3" >> $(OUT) 
	-./hw8c_opt3_3 data.txt >> $(OUT) 2>&1
	@echo -e "\nTesting hw8c_opt3_mem" >> $(OUT) 
	-./hw8c_opt3_mem data.txt >> $(OUT) 2>&1

mem: $(addsuffix _mem,$(BIN))
	
	
	@echo -e "\nRunning Valgrind on hw8c_mem" >> $(OUT) 2>&1
	$(VALGRIND) ./hw8c_mem  >> $(OUT) 2>&1

	@echo -e "\nRunning Valgrind on hw8c_opt1_mem" >> $(OUT) 2>&1
	$(VALGRIND) ./hw8c_opt1_mem >> $(OUT) 2>&1

	@echo -e "\nRunning Valgrind on hw8c_opt2_mem" >> $(OUT) 2>&1
	$(VALGRIND) ./hw8c_opt2_mem >> $(OUT) 2>&1

	@echo -e "\nRunning Valgrind on hw8c_opt3_mem" >> $(OUT) 2>&1
	$(VALGRIND) ./hw8c_opt3_mem >> $(OUT) 2>&1

help:
	@echo "Available targets:"
	@echo "  all      - Compiles all binaries with various optimization levels and memory checking"
	@echo "  %_0      - Compiles the specified program with -O0 optimization"
	@echo "  %_2      - Compiles the specified program with -O2 optimization"
	@echo "  %_3      - Compiles the specified program with -O3 optimization"
	@echo "  %_mem    - Compiles the specified program for memory checking"
	@echo "  test     - Runs all compiled programs and outputs results to $(OUT)"
	@echo "  mem      - Runs memory checks on all compiled programs"
	@echo "  clean    - Removes all compiled binaries and output files"
	@echo "  help     - Displays this help"

clean:
	rm -f $(BIN) $(addsuffix _0,$(BIN)) $(addsuffix _2,$(BIN)) $(addsuffix _3,$(BIN)) $(addsuffix _mem,$(BIN)) $(OUT) 
