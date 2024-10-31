CC = gcc
CXX = g++
CFLAGS = -Wall -std=c99 -pedantic -O1 -DMOVE_FOPEN -DEN_TIME
CXXFLAGS = -Wall -pedantic -O1 -DMOVE_FOPEN -DEN_TIME
LDFLAGS = -lm


# Source files
C_SRC=hw6c.c
CPP_SRC=hw6cpp.cpp


# Output binaries
C_OUT = hw6c
CPP_OUT = hw6cpp

.SILENT:

.PHONY: all test help clean

# Build all targets
all: $(C_OUT) $(CPP_OUT)

# C program
$(C_OUT): $(C_SRC)
	$(CC) $(CFLAGS)  -o $@ $< $(LDFLAGS)

# C++ program
$(CPP_OUT): $(CPP_SRC)
	$(CXX) $(CXXFLAGS)  -o $@ $< $(LDFLAGS)

# Run tests
test:
	@echo "Running $(C_OUT) with data.txt" > out.txt
	./$(C_OUT) data.txt >> out.txt 2>&1
	@echo "Running $(CPP_OUT) with data.txt" >> out.txt
	./$(CPP_OUT) data.txt >> out.txt 2>&1

# Display help information
help:
	@echo "Available targets:"
	@echo "  all  - Builds all executables with timing enabled"
	@echo "  test - Runs both $(C_OUT) and $(CPP_OUT) with data.txt and redirects output to out.txt"
	@echo "  clean - Removes all generated files"
	@echo "  help - Displays this help message"

# Clean up generated files
clean:
	@rm -f $(C_OUT) $(CPP_OUT) out.txt
