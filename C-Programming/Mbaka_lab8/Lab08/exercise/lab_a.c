/*---------------------------------------------------------------------------
  This program demonstrates a simple 2d dynamic array
  10/08/2019   R. Repka    Initial version
  
  professor version
  
  10/06/2020 	R. Repka	- Added grow feature
  
  valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include  "Timers.h"

void free_matrix_space(double **M, int rows);
void printMat(double **M, int rows, int cols); 


/* Dynamically Allocated Arrays */  
int main(int argc, char *argv[]) { 
#ifdef EN_TIME
   DECLARE_TIMER(Time); // Declare the memory allocation timer 
   DECLARE_REPEAT_VAR(repeatVar);
#endif
   // row/index counters and row/col sizes
   int rows = ROWS, cols = COLS;
   // The key trick to a 2D matrix
   double **A = NULL; 

   printf("Simple alloc: rows and columns: %d, %d\n", rows, cols);
   
   // Start measuring the performance of memory allocation
#ifdef EN_TIME
   START_TIMER(Time); // Start the timer
   BEGIN_REPEAT_TIMING(REPEAT_NUM, repeatVar);
#endif
   // Allocate space for array of pointers
   A = malloc( rows * sizeof(double *));
   if (NULL == A) {
      fprintf(stderr, "Malloc error line %d\n", __LINE__);
      exit(99);
   }
   /*-------------------------------------------------------
     Replace this code with a SINGLE calloc()/malloc() memory 
     allocation and looping pointer table fixup
   -------------------------------------------------------*/
   // Allocate space for matrix data elements  
   A[0] = malloc(rows * cols * sizeof(double));
   if (NULL == A[0]) {
      fprintf(stderr, "Malloc error line %d\\n", __LINE__);
      exit(99);
   }

   // Point the row pointers to the correct location
   for (int i = 1; i < rows; i++) {
      A[i] = A[0] + i * cols;
   }

   // No need to measure the initialization and printing code, so it's wrapped with #ifndef EN_TIME
#ifndef EN_TIME
   // Initialize the matrix to nice data
   for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
         A[i][j] = (double)i +(double)j/10.0;
      }
   }
   // Use to ensure random access works - do not change
   A[1][1] = 999;
   A[2][2] = 999;

    // Print out the 3x5 matrix
   printMat(A, rows, cols);
#endif

   // Complex return of space
   free_matrix_space(A, rows);
   // Stop measuring the performance of memory deallocation
#ifdef EN_TIME
    END_REPEAT_TIMING;
    STOP_TIMER(Time); // Stop the timer
    PRINT_TIMER(Time);
    PRINT_RTIMER(Time, REPEAT_NUM); // Print the timer results
    RESET_TIMER(Time); 
#endif
  return 0;
}

/*---------------------------------------------------------------------------
  This function frees a 2D matrix generated using individual row allocations
  
  where:  double **M - Point to an array of pointers to free  
          int row    - The number of array row elments in M
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
   /*-------------------------------------------------------
     Modify the function to properly free the single calloc
     block.  Remove "rows" won't be need in this new solution
   -------------------------------------------------------*/
void free_matrix_space(double **M, int rows) {
   // Free the single allocated block
   free(M[0]);
   // Now free the pointer list
   free(M);
   M = NULL;
}

/*---------------------------------------------------------------------------
  This function prints the contents of a 2d array
  
  where:  double **M - Point to the matrix row pointer holder  
          int row    - The number of array row elements
		  int cols	 - The number of array column elements
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
void  printMat(double **M, int rows, int cols) {
	int i,j;
   // Print out the matrix
   printf("The %dx%d 2D dynamic matrix\n", rows, cols); 
   for (i=0; i< rows; i++){
      for (j=0; j< cols; j++){
         printf("%4.1f\t", M[i][j]);
      }
      printf("\n");
   }
}
