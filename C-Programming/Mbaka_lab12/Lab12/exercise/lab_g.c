/*---------------------------------------------------------------------------
  This program demonstrates the limitations of scanf() and the use of fseek()
  student copy
---------------------------------------------------------------------------*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 256

int main(int argc, char *argv[]) {
   // File name to read and the handle
   FILE *DataFile = NULL;
   char *fileName = "data.txt";
   
   // The data to parse
   int num, v1, v2, v3;
   char *buffer = NULL;
   size_t buffsize = 0;  // Required by getline

   // Open the data file
   DataFile = fopen(fileName, "r");
   if (NULL == DataFile) {
      printf("Error could not open '%s'\n", fileName);
      exit(99);
   }
   
   // Read all the EXACT data in the file using getline(), in a while loop
   // and print the results out, removing any trailing LF \n character, if any.
   printf("Using getline() to read data:\n");
   while (getline(&buffer, &buffsize, DataFile) != -1) {
      buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline and carriage return if any
      printf("Read: '%s'\n", buffer);
   }
   
   // Use fseek() to start back at the beginning of the file
   fseek(DataFile, 0, SEEK_SET);
   
   // Read all the data in the file using fscanf with 3
   // parsing variables and then print the results out
   printf("\nUsing fscanf() to parse integers:\n");
   while ((num = fscanf(DataFile, "%d %d %d", &v1, &v2, &v3)) == 3) {
      printf("%d parameters %d %d %d\n", num, v1, v2, v3);
   }
   
   // Free dynamically allocated memory, close files
   free(buffer);
   fclose(DataFile);

   return 0;
}
