/*---------------------------------------------------------------------------
  This program demonstrates the limitations of scanf() and the use of fseek()
  student copy
---------------------------------------------------------------------------*/
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
   char *buffer;

   // Get memory
      buffer = malloc((BUFF_SIZE + 1) * sizeof(char));
      if (NULL == buffer) {
      printf("Malloc error\n");
      exit(99);
   }
   
   // Open the data
      DataFile = fopen(fileName, "r");
      if (NULL == DataFile) {
        printf("Error could not open '%s'\n", fileName);
        exit(99);
    }
      
   // Read all the EXACT data in the file using fgets(), in a while loop
   // and print the results out, removing any trailing LF \n character, if any.
      printf("Using fgets() to read data:\n");
      while (fgets(buffer, BUFF_SIZE, DataFile) != NULL) {
      // Linux terminates text files with LF (\n) but Windows terminates with CRLF (\r\n)
      // When transferring text files from Window to Linux some tools add the CR back in
      // remove the lf at the end AND cr (if any) 
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
   
   // Return memory, close files
      fclose(DataFile);
      free(buffer);

   return(0);
}  
