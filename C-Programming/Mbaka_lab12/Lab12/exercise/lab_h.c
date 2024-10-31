/*---------------------------------------------------------------------------
  This program demonstrates the use of strtod() for parsing instead of fscanf()
  and the use of fseek(). Also includes errno to check for conversion errors.
---------------------------------------------------------------------------*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFF_SIZE 256

int main(int argc, char *argv[]) {
    // File name to read and the handle
    FILE *DataFile = NULL;
    char *fileName = "data.txt";

    // The data to parse and buffer handling
    char *buffer = NULL;
    size_t buffsize = 0;  // Required by getline
    char *endPtr;

    // Open the data file
    DataFile = fopen(fileName, "r");
    if (NULL == DataFile) {
        printf("Error could not open '%s'\n", fileName);
        exit(99);
    }

    // Read all the data in the file using getline(), parse with strtod()
    printf("Using getline() and strtod() to read and parse data:\n");
    while (getline(&buffer, &buffsize, DataFile) != -1) {
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline and carriage return if any
        printf("Read: '%s'\n", buffer);

        char *ptr = buffer;
        int count = 0;
        double value;

        // Parse each number in the line
        while (*ptr != '\0') {
            errno = 0;
            value = strtod(ptr, &endPtr);
            if (ptr == endPtr) {
                break; // No more numbers
            }
            if (errno == ERANGE) {
                perror("Range error");
                break;
            }
            printf("Value: %f\n", value);
            count++;
            ptr = endPtr;
        }
        printf("%d parameters\n", count);
    }

    // Free dynamically allocated memory, close files
    free(buffer);
    fclose(DataFile);

    return 0;
}
