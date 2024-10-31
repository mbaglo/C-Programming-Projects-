/*---------------------------------------------------------------------------
  QN code testing and verification
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>

#define LOOP_DELAY (1<<27)

// QN math macros that pass the number of bits "n" 
#define Qn_MULTIPLY(A,B,qnval) (((long int)(A)*(long int)(B)) >>(qnval))
#define Qn_DIVIDE(A,B,qnval) ((int) (((long int)(A)<<(qnval))/(B) ))

// QN data conversion macros.
#define FLOAT_TO_FIXED( x,  qnval)  (int)( (x) * (double)(1 << (qnval)) )
#define FIXED_TO_FLOAT( x,  qnval)  ((double)(x) / (double) (1 << (qnval)))


// QN math and printing functions
void  printBinary(unsigned num, short N);


/*---------------------------------------------------------------------------
  QN testing framework 
---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // QN testing variables, add more as necessary
    double fnum1, fnum2;
    int qnum1, qnval;

    //------------------------------------------------------------------------
    // Print data on sizes  
    //------------------------------------------------------------------------
    printf("Integers are %ld bits long\n", 8*sizeof(int));

    //------------------------------------------------------------------------
    // Prove floating point has limits 
    //------------------------------------------------------------------------
    printf("\nProving floating point has limits by adding Epsilon\n");
    double value = 10.0;
    double result = value + DBL_EPSILON;

    // Compare the original value with the result
    if (value == result) {
        printf("Epsilon value '%.15e' didn't add to 10.0, proving floating point limits.\n", DBL_EPSILON);
    } else {
        printf("Epsilon value '%.15e' added to 10.0 successfully, result: %.15e\n", DBL_EPSILON, result);
    }

    //------------------------------------------------------------------------
    // Examine the conversion utilities
    //------------------------------------------------------------------------
    printf("\nConversion test\n");

    // Convert 0.0 to Qn0 and back
    fnum1 = 0.0;
    qnval = 0; // Qn0 format
    qnum1 = FLOAT_TO_FIXED(fnum1, qnval);
    fnum2 = FIXED_TO_FLOAT(qnum1, qnval);
    printf("Started with %.2f converted to Qn%d = %d decimal then back to %.2f\n", fnum1, qnval, qnum1, fnum2);
    printf("Qn0 %d decimal in binary is ", qnum1);
    printBinary((unsigned)qnum1, 16); 

    // Convert 12.25 to Qn3 and back
    fnum1 = 12.25;
    qnval = 3; // Qn3 format
    qnum1 = FLOAT_TO_FIXED(fnum1, qnval);
    fnum2 = FIXED_TO_FLOAT(qnum1, qnval);
    printf("Started with %.2f converted to Qn%d = %d decimal then back to %.2f\n", fnum1, qnval, qnum1, fnum2);
    printf("Qn3 %d decimal in binary is ", qnum1);
    printBinary((unsigned)qnum1, 16); 

    // Convert 12.0625 to Qn3 and back
    fnum1 = 12.0625;
    qnum1 = FLOAT_TO_FIXED(fnum1, qnval); // Using Qn3
    fnum2 = FIXED_TO_FLOAT(qnum1, qnval);
    printf("Started with %.4f converted to Qn%d = %d decimal then back to %.4f\n", fnum1, qnval, qnum1, fnum2);
    printf("Qn3 %d decimal in binary is ", qnum1);
    printBinary((unsigned)qnum1, 16);

    // Convert 12.0625 to Qn4 and back
    qnval = 4; // Qn4 format
    qnum1 = FLOAT_TO_FIXED(fnum1, qnval); // Using Qn4
    fnum2 = FIXED_TO_FLOAT(qnum1, qnval);
    printf("Started with %.4f converted to Qn%d = %d decimal then back to %.4f\n", fnum1, qnval, qnum1, fnum2);
    printf("Qn4 %d decimal in binary is ", qnum1);
    printBinary((unsigned)qnum1, 16);

    return(0);
}

/*---------------------------------------------------------------------------
  This prints a number in character binary bracketed by [..]
  
  Where: unsigned num - number to print
         short N      - number of bits to print - typically 16
         
  Returns: nothing
  Error handling: none
----------------------------------------------------------------------------*/
void printBinary(unsigned num, short N) { 
    unsigned i; 
    printf("[");
    
    for (i = 1 << (N-1); i > 0; i = i / 2) { // Changed the decrement operation for clarity
        (num & i)? printf("1"): printf("0");
    }        
    printf("]\n");
    return;
}
