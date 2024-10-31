CC := gcc
CFLAGS := -g -Wall -pedantic -std=c99
VALGRIND := valgrind --leak-check=full
LIBS := -lgsl -lgslcblas -lm
SRCS := hw11.c correction.c DynamicArrays.c
OBJS := $(SRCS:.c=.o)
PROGS := hw11 correction


CORRECT := correct
CORRECTION := ./correction
REAL_DEVICE := ./realDevice
DET_ERROR := ./detError
MYPLOT := ./myplot
DIFFVAL := ./diffVal
DOALL := ./doall.sh
MEM := mem.txt

.SILENT:
.PHONY: all base correct plot mem doall x help clean

all: $(PROG)

hw11: hw11.c DynamicArrays.c
	$(CC) $(CFLAGS) -DHW11 $^ $(LIBS) -o $@

correction: correction.c
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	@echo "Compiling '$*'"
	$(CC) $(CFLAGS) $*.c -c
base:
	@$(REAL_DEVICE) | $(DET_ERROR)

correct:
	@$(REAL_DEVICE) | $(CORRECTION) | $(DET_ERROR)

plot:
	@$(REAL_DEVICE) > realdevice.txt
	@$(REAL_DEVICE) | $(CORRECTION) > correction.txt
	@paste realdevice.txt correction.txt > alldata.txt
	@$(MYPLOT) -in alldata.txt -out correction.png

mem:
	@$(VALGRIND) ./hw11 -order 3 -p data.txt > $(MEM) 2>&1

doall:
	@$(DOALL) correction.c $(CORRECT)

x:
	@chmod +x $(REAL_DEVICE) $(DET_ERROR) $(MYPLOT) $(DIFFVAL)

help:
	@echo "Available commands:"
	@echo "  all      - Compile all programs."
	@echo "  base     - Calculate base error using realDevice and detError."
	@echo "  correct  - Calculate error using correction and detError."
	@echo "  plot     - Generate plots for real and corrected data."
	@echo "  mem      - Run memory checks on hw11 and output to mem.txt."
	@echo "  doall    - Execute the doall.sh script."
	@echo "  x        - Make binaries executable."
	@echo "  clean    - Clean up generated files."
	@echo "  help     - Display this help message."

clean:
	@rm -f $(OBJS) $(PROGS) *.txt *.png $(MEM) $(CORRECT)*
