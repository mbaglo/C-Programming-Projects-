/*-----------------------------------------------------------------------
  Simple program to dump IEEE single precision floating point numbers
  student copy
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


int main( int argc, char *argv[] ) {
 float epsilon     = 1.0f;
 float lastEpsilon = epsilon;
 double doubleEpsilon = 1.0; 
 double lastDoubleEpsilon = doubleEpsilon; 

 /*------------------------------------------------------------------------
   Add code to calculate the smallest single precision floating point number
 ------------------------------------------------------------------------*/
 while (epsilon > 0.0f) {
        lastEpsilon = epsilon;  // Save the current epsilon before halving it
        epsilon /= 2.0f;  // Halve epsilon to approach the smallest non-zero value
    }
 // Print out the last non-zero value single precision floating point number
   printf("Smallest Non-Zero Single Precision Value: %2.6g\n", lastEpsilon);
   printf("\n");
/*------------------------------------------------------------------------
   Add code to calculate single precision epsilon, the smallest single 
   precision floating point number you can add to 1.0 and not get 1.0
 ------------------------------------------------------------------------*/
 epsilon = 1.0f; // Reset epsilon to 1.0 for epsilon calculation
 while ((float)(1.0f + epsilon) != (float)1.0f) {
   printf("%10.8g\t%.20f\n",epsilon,(1.0 + epsilon));

   // Insert your code here
   lastEpsilon = epsilon; // Save the current epsilon before halving it
   epsilon /= 2.0f; // Halve epsilon to find the smallest increment that still changes the sum
    
 }  

    // Print out the calculated and actual Epsilon
    printf("\n");
    printf("Calculated Machine Epsilon: %2.6g\n",lastEpsilon);
    printf("Actual Machine Epsilon:     %2.6g\n", FLT_EPSILON );
    printf("\n");
 /*------------------------------------------------------------------------
       Calculate the smallest non-zero double precision floating point value
  ------------------------------------------------------------------------*/
doubleEpsilon = 1.0; 
while (doubleEpsilon > 0.0) {
    lastDoubleEpsilon = doubleEpsilon;  // Save the current doubleEpsilon before halving it
    doubleEpsilon /= 2.0;  // Halve doubleEpsilon to approach the smallest non-zero value
    }
    // Print out the smallest non-zero double precision floating point value
   
    printf("Smallest Non-Zero Double Precision Value: %2.6g\n", lastDoubleEpsilon);
    printf("\n");

 /*------------------------------------------------------------------------
   Insert your double precision code here when instructed
 ------------------------------------------------------------------------*/
doubleEpsilon = 1.0; 
while (1.0 + doubleEpsilon != 1.0) {
    printf("%10.8g\t%.20f\n", doubleEpsilon, (1.0 + doubleEpsilon));
    lastDoubleEpsilon = doubleEpsilon; // Save the current doubleEpsilon before halving it
    doubleEpsilon /= 2.0; // Halve doubleEpsilon to find the smallest increment that still changes the sum
}

// Print out the calculated and actual Epsilon for double precision
printf("\n");
printf("Calculated Machine Double Precision Epsilon: %2.6g\n", lastDoubleEpsilon);
printf("Actual Machine Double Precision Epsilon:     %2.6g\n", DBL_EPSILON);

 
 return 0;
} 

