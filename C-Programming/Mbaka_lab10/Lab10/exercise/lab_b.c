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
   double fnum1, fnum2, fnum3;
   int  qnum1, qnum2, qnum3, qnval;
   clock_t  time1, time2;
   unsigned i;
   
   
 //------------------------------------------------------------------------
// Examine the division utilities
//------------------------------------------------------------------------
double fnum = -0.0625; // Starting floating-point number
qnum1 = (int)(fnum * (1 << 18)); // Convert to Qn18 format
qnum2 = qnum1; // Duplicate qnum1 for separate division tests
qnval = 18; // Specify the Qn format

printf("Division test\n");
printf("fnum        qnum1 (dec)  qnum2 (dec)  qnum1 (float)  qnum2 (float)\n");

// Loop until one of the Qn values becomes zero
while (qnum1 != 0) {
    fnum /= 2.0; // Divide the floating-point number by 2.0
    qnum1 /= 2; // Divide the first Qn number by 2 using integer division
    qnum2 = Qn_DIVIDE(qnum2, 2, qnval); // Divide the second Qn number by 2 using Qn_DIVIDE

printf("%-10f  %-12d  %-12d  %-13f  %-13f\n",
               fnum, qnum1, qnum2,
               qnum1 / (double)(1 << qnval),
               qnum2 / (double)(1 << qnval));
}


//------------------------------------------------------------------------
// Examine the multiplication utilities
//------------------------------------------------------------------------
printf("Multiplication test\n");

// Define and initialize Qn variables
fnum1 = 64.125, fnum2 = 0.755;
qnval = 18;  // Qn format specifier
qnum1 = (int)(fnum1 * (1 << qnval));  // Convert to Qn18 format
qnum2 = (int)(fnum2 * (1 << qnval));  // Convert to Qn18 format

// Perform Qn multiplication
int qnum_product = Qn_MULTIPLY(qnum1, qnum2, qnval);

// Convert the Qn product back to float to check for data loss
double fnum_product_back = qnum_product / (double)(1 << (2 * qnval));

// Print the results
printf("Product= %f float  qnx product= %d dec, converted back %f float\n",
       fnum1 * fnum2, qnum_product, fnum_product_back);
printf("\n");

//------------------------------------------------------------------------
// Implement floating point scientific equations two different ways
// using normal fractions and using decimal fractions
//------------------------------------------------------------------------
printf("\nComplex calculations test\n");
printf("\n");
double x = 1.0; // Value of x for evaluation
// Implementing Horner's method for F(x) = x^3 - 0.0001x^2 - 676x + 0.0676
double f_result = (((x - 0.0001) * x - 676) * x + 0.0676);
printf("Floating-point test\n");
printf("Product= %f float\n", f_result);
printf("\n");

// Implement Horner's method for F(x) = ((x * (1/10000)) * x - 676) * x + (169/2500)
double f_result_fractional = ((((1.0 / 10000) * x - 676) * x) + (169.0 / 2500));
printf("Fractional test\n");
printf("Product= %f float\n", f_result_fractional);
printf("\n");
//---------------------------------------------------------------------
// Implement QN scientific equations: x**3 - 0.0001x**2 - 676x + 0.0676
//------------------------------------------------------------------------   
printf("Qn test\n");
printf("\n");
int qn_x = (int)(1.0 * (1 << 18)); // Convert x=1.0 to Qn18
int qn_coeff1 = FLOAT_TO_FIXED(-0.0001, 18); // Convert coefficient to Qn18
int qn_coeff2 = FLOAT_TO_FIXED(0.0676, 18); // Convert coefficient to Qn18

// Qn evaluation of F(x) = x^3 - 0.0001x^2 - 676x + 0.0676
int qn_result = Qn_MULTIPLY(qn_x, Qn_MULTIPLY(qn_x, qn_x, 18), 18); // x^3
qn_result += Qn_MULTIPLY(Qn_MULTIPLY(qn_x, qn_x, 18), qn_coeff1, 18); // -0.0001x^2
qn_result -= Qn_MULTIPLY(FLOAT_TO_FIXED(676, 18), qn_x, 18); // -676x
qn_result += qn_coeff2; // +0.0676

// Convert the Qn result back to float for comparison
double qn_result_back = qn_result / (double)(1 << (2 * 18));

// Print all 3 answers
printf("Floating-point result: %f\n", -674.9325); // Assuming this is the reference floating-point result
printf("Qn product (decimal): %d\n", qn_result);
printf("Converted back to float: %f\n", qn_result_back);
printf("\n");

//---------------------------------------------------------------------
// Implement QN scientific equations: x**3-.0001x**2-676X+.0676
//------------------------------------------------------------------------ 


printf("Product= %f float qnx product= %d dec, converted back %f float\n",
       -674.9325, // Assuming this is the reference floating-point result
       qn_result,
       qn_result_back);
printf("\n");
//------------------------------------------------------------------------
// Examine the performance 
//------------------------------------------------------------------------
printf("\nPerformance test\n");
printf("\n");
// Initialize variables
 fnum1 = 3.1415, fnum2 = -674.9325;
 qnum1 = FLOAT_TO_FIXED(3.1415, 18); // Convert to Qn18
 qnum2 = FLOAT_TO_FIXED(-674.9325, 18); // Convert to Qn18

// Floating point addition
time1 = clock();
for(i = 0; i < LOOP_DELAY; i++) {
    fnum1 += fnum2; // Floating-point addition
}
time1 = clock() - time1;

// QN addition
time2 = clock();
for(i = 0; i < LOOP_DELAY; i++) {
    qnum1 += qnum2; // Qn addition
}
time2 = clock() - time2;

// Print out the clocks and which was faster for addition
printf("Floating-point addition took %lu clocks.\n", time1);
printf("Qn addition took %lu clocks.\n", time2);
printf("Faster operation for addition: %s\n", (time1 < time2) ? "Floating-point" : "Qn");
printf("\n");
// Reset variables for multiplication
fnum1 = 3.1415;
qnum1 = FLOAT_TO_FIXED(3.1415, 18);

// Floating multiplication
time1 = clock();
for(i = 0; i < LOOP_DELAY; i++) {
    fnum1 *= fnum2; // Floating-point multiplication
}
time1 = clock() - time1;

// QN multiplication
time2 = clock();
for(i = 0; i < LOOP_DELAY; i++) {
    qnum1 = Qn_MULTIPLY(qnum1, qnum2, 18); // Qn multiplication
}
time2 = clock() - time2;

// Print out the clocks and which was faster for multiplication
printf("Floating-point multiplication took %lu clocks.\n", time1);
printf("Qn multiplication took %lu clocks.\n", time2);
printf("Faster operation for multiplication: %s\n", (time1 < time2) ? "Floating-point" : "Qn");
printf("\n");
return 0;
}
/*---------------------------------------------------------------------------
  This prints a number in character binary bracketed by [..]
  
  Where: unsigned num - number to print
         short N      - number of bits to print
         
  Returns: nothing
  Error handling: none
----------------------------------------------------------------------------*/
void printBinary(unsigned num, short N) { 
   unsigned i; 
   printf("\t[");
   
   for (i = 1 << (N-1); i > 0; i = i >> 1) {
      (num & i)? printf("1"): printf("0");
    }        
   printf("]\n");
   return;
} 
