/*---------------------------------------------------------------------------
  10/08/2019   R. Repka    Initial version
---------------------------------------------------------------------------*/
#include<stdio.h>
#include<stdlib.h> 

/* string example to illustrate string functions */  
int main(int argc, char *argv[]) {    
   int A[3][5]; /* 2D array of integers (“3 rows, 5 columns”) */
   int i,j;
   
   for (i=0; i<3; i++) {/* Initialize to random */
      for (j=0; j<5; j++) {
         A[i][j] = rand() % 100; //0-99
      }
   }
   
   // Print out the array
   printf("The array data\n");
   for (i=0; i<3; i++){ /* print */
      for (j=0; j<5; j++){
         printf("%d\t", A[i][j]);
      }
      printf("\n");
   }
   printf("\n");
   
    /* test different indexing methods*/
   i=0;
   j=0;  
   printf("i= %d, j= %d\n", i,j);
   printf("A[i][j]            :%d\n", A[i][j]);
   printf("*( A[i] + j )      :%d\n", *(A[i]+j));
   printf("( *(A+i) )[j]      :%d\n", (*(A+i))[j]);
   printf("*( (*(A+i)) + j )  :%d\n", *((*(A+i))+j));
   printf("*(&A[0][0]+ 5*i+j ):%d\n", *(&A[0][0]+ 5*i+j));
  return 0;
}