/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random doubles
 *
 */
 
#include <stdio.h>  
#include <stdlib.h> /* for qsort()    */

/* Function prototypes */
int cmpdbl(const void *p1,const void *p2); /* for qsort() */
 
/* 
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it 
*/
int main(int argc, char *argv[]) {
   double *array;
   int num;
   
   // Check the command line entry
   if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of entries>\n", argv[0]);
        return 1;  // Return non-zero for error
    }

    num = atoi(argv[1]);  // Convert string to integer

    // Validate the number of entries
    if (num < 2) {
        fprintf(stderr, "Error: Number of entries must be at least 2\n");
        return 1;
    }
   
   // Get the memory
   array = (double *)malloc(num * sizeof(double));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
   
   // Initialize the array with random data
   for(int i = 0; i < num; ++i) {
      array[i] = (rand() % 1001) /10.0 - 50.0;
   }
   
   // Sort the data
   qsort(array, num, sizeof(double), cmpdbl);
   
   // Print the sorted dat
   for(int i = 0; i < num; ++i) {
      printf("% 10.1f\n", array[i]);
   }
   printf("\n");   

   // Free allocated memory
    free(array);
return(0);
}  
 
/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to doubles in this case
---------------------------------------------------------------------------*/
int cmpdbl(const void *p1, const void *p2) {
const double *a = (const double *) p1;
const double *b = (const double *) p2;
return (*a > *b) - (*a < *b);

}


