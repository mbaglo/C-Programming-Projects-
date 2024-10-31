/*---------------------------------------------------------------------------
  This program demonstrates a simple 2d dynamic array
  10/08/2019   R. Repka    Initial version
  
  professor version
  
  10/06/2020 	R. Repka	- Added grow feature
  
  valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99
---------------------------------------------------------------------------*/
#include<stdio.h>
#include <stdlib.h>  

void free_matrix_space(double **M, int rows);
void printMat(double **M, int rows, int cols); 


/* Dynamically Allocated Arrays */  
int main(int argc, char *argv[]) {    
   // row/index counters and row/col sizes
   int rows=3,cols=5;

   // The key trick to a 2D matrix
   double **A = NULL; 

   printf("Simple alloc: rows and columns: %d, %d\n", rows, cols);

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
   for (int i=0; i< rows; i++){ 
      A[i] = malloc(cols * sizeof(double) ); 
      if (NULL == A[i]) {
         fprintf(stderr, "Malloc error line %d\n", __LINE__);
         exit(99);
      }      
   }
   
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

   // Complex return of space
   free_matrix_space(A, rows);

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
   int i;
   // Free the row data
   for (i = 0; i < rows; i++) {
      free(M[i]);
      M[i] = NULL;
   }
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
