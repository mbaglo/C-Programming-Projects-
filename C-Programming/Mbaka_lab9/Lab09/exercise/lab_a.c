/*-----------------------------------------------------------------------
  Simple program to dump IEEE single precision floating point numbers
  student copy
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIGN_MASK 0x80000000       // Mask to extract the sign bit
#define SIGN_SHIFT 31              // Shift needed to right-align the sign bit
#define EXPONENT_MASK 0x7F800000   // Mask to extract the exponent bits
#define EXPONENT_SHIFT 23          // Shift needed to right-align the exponent bits
#define EXPONENT_BIAS 127          // Bias for the exponent field
#define SIGNIFICAND_MASK 0x007FFFFF // Mask to extract the significand bits


void ieeePrint(float fnum);

int main(int argc, char * argv[]) {
   
      // Must have a number
   if (2 == argc) {
      ieeePrint(atof(argv[1]));
   }
   else {
      printf("This decodes IEEE single precision numbers\n");
      printf("Error: Syntax is: %s number\n", argv[0]);
   }

   return(0);
}


void ieeePrint(float fnum) {
    int sign, exponent, significand;
    
    
    // We require a union because C won't let us mask floating point directly
    union {
        float f;
        unsigned int i;
    } u;
    u.f = fnum; // Assign the floating-point number to the union
    
    // Break the floating point item into the proper parts
    sign = (u.i & SIGN_MASK) >> SIGN_SHIFT;
    exponent = ((u.i & EXPONENT_MASK) >> EXPONENT_SHIFT) - EXPONENT_BIAS;
    significand = (u.i & SIGNIFICAND_MASK) << 1;
    


   
   printf("Number %f => Bin sign:%d Dec exponent:%d  Hex significand .%x\n", fnum, sign, exponent, significand);
   return;
}

