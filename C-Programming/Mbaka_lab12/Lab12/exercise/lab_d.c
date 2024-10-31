/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random polar data
 *
 * Use compiler directive -DN=size to change the size of the array
 *
 * Reference: A. Kelley and I Pohl "A book on C" 4th Ed.
 * Revised: 3/31/2015 (JCCK)
 */
 
#include <stdio.h>  
#include <math.h>
#include <float.h>
#include <stdlib.h> /* for qsort()    */

/* The polar data structure */
typedef struct {double mag; double ang;} polar;

/* Add your polar compare function prototype here */
 int comparePolar(const void *p1, const void *p2);
/* 
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it 
*/
int main(int argc, char *argv[]) {

   polar *array;
   int num;
   int i;
   
   
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
   array = (polar *)malloc(num * sizeof(polar));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }


   // Initialize the array with random data, make one magnitude identical
   for(i = 0; i < num-1; ++i) {
      array[i].mag = (rand() % 1001) /10.0 - 50.0;
      array[i].ang = (rand() % 1001) /10.0 - 50.0;
   }
   // Identical 
   array[i].mag = array[i-1].mag;
   array[i].ang = (rand() % 1001) /10.0 - 50.0;


   
   // Sort the data
   qsort(array, num, sizeof(polar), comparePolar);

   
   // Print the sorted dat
   for(i = 0; i < num; ++i) {
      printf("%10.1f %10.1f\n", array[i].mag, array[i].ang);
   }
   printf("\n");   
return(0);
}  
 
/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to polar in this case
---------------------------------------------------------------------------*/
// add your function and code here
int comparePolar(const void *p1, const void *p2) {
    const polar *a = (const polar *) p1;
    const polar *b = (const polar *) p2;

    // Compare magnitudes first
    if (a->mag > b->mag) return 1;
    if (a->mag < b->mag) return -1;

    // If magnitudes are equal, compare angles
    return (a->ang > b->ang) - (a->ang < b->ang);
}