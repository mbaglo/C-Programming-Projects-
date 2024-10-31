/*---------------------------------------------------------------------------
  Implement integer square root code - student copy
         gcc -g lab_c.c -O3 -o lab_c -lm 
         
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Prototypes
int i_sqrt(int n);
int q_sqrt(int n, int q);
void findRoots( double a, double b, double c, int qn);

// QN math macros that pass the number of bits "n" 
#define Qn_MULTIPLY(A,B,qnval) (((long int)(A)*(long int)(B)) >>(qnval))
#define Qn_DIVIDE(A,B,qnval) ((int) (((long int)(A)<<(qnval))/(B) ))

// QN data conversion macros.
#define FLOAT_TO_FIXED( x,  qnval)  (int)( (x) * (double)(1 << (qnval)) )
#define FIXED_TO_FLOAT( x,  qnval)  ((double)(x) / (double) (1 << (qnval)))



int main(int argc, char *argv[]) {
   int q;
   double value;
   
   // Test the pure integer square root code
   q = 0;
   value = 16.0;
   printf("value %f float square root %f  i_sqrt %d ", value, sqrt(value), i_sqrt(value));
   printf("q_sqrt %f\n", FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q),q));
 
   q = 16;
   value = 23425.0;
   printf("value %f float square root %f  i_sqrt %d ", value, sqrt(value), i_sqrt(value));
   printf("q_sqrt %f\n", FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q),q));
   
   q = 16;
   value = 0.0;
   printf("value %f float square root %f  i_sqrt %d ", value, sqrt(value), i_sqrt(value));
   printf("q_sqrt %f\n", FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q),q));
   
   q = 16;
   value = 0.5;
   printf("value %f float square root %f  i_sqrt %d ", value, sqrt(value), i_sqrt(value));
   printf("q_sqrt %f\n", FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q),q));

   // Automated testing of Qn square root
   q = 16;
   for (int i = 0; i < 1000; i++) {
      // Generate a random value
      value = (double)rand()/ (double)RAND_MAX*1000.0;
      
      // Check the percent error
      if (fabs(FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q), q)-sqrt(value))/sqrt(value) > .0001 )
      {
         printf("Error: Value %f float square root %f  ", value, sqrt(value));
         printf("q_sqrt %f\n", FIXED_TO_FLOAT(q_sqrt(FLOAT_TO_FIXED(value, q), q),q));
      }
   } // End for
   printf("automated testing complete\n");

   // Find the roots of x^2 + 10x + 23 should be   -3.585786 -6.414214
   findRoots(1.0, 10.0, 23.0, 12);

   return(0);
}

/*---------------------------------------------------------------------------
  Description: This program finds the roots of a 2n order polynomial using the 
      quadratic equation and then prints out the results a Qn and floating
      point implementation.
      
   roots = -b +/- sqrt(b^2-4AC))
           ---------------------
                 2a
               
   Where:   double a - The x^2 polynomial coefficient 
            double b - The x^1 polynomial coefficient
            double c - The x^0 polynomial coefficient
   
   Returns: nothing
   Errors:  none
         
---------------------------------------------------------------------------*/
void findRoots(double a, double b, double c, int qn) {
    double froot[2];
    int iroot[2];

    // Traditional floating point implementation
    double discriminant = b * b - 4 * a * c;
    double sqrt_discriminant = sqrt(discriminant);
    froot[0] = (-b + sqrt_discriminant) / (2 * a);
    froot[1] = (-b - sqrt_discriminant) / (2 * a);

    printf("The floating point roots of %fx^2 + %fx + %f are %f %f\n", a, b, c, froot[0], froot[1]);

    // Qn implementation
    // Convert coefficients to Qn format
    int qn_a = FLOAT_TO_FIXED(a, qn);
    int qn_b = FLOAT_TO_FIXED(b, qn);
    int qn_c = FLOAT_TO_FIXED(c, qn);

    // Calculate discriminant in Qn format
    int qn_discriminant = Qn_MULTIPLY(qn_b, qn_b, qn) - Qn_MULTIPLY(FLOAT_TO_FIXED(4, qn), Qn_MULTIPLY(qn_a, qn_c, qn), qn);

    // Calculate square root of the discriminant in Qn format (assuming q_sqrt function is implemented)
    int qn_sqrt_discriminant = q_sqrt(qn_discriminant, qn);

    // Calculate roots in Qn format
    iroot[0] = Qn_DIVIDE(-qn_b + qn_sqrt_discriminant, Qn_MULTIPLY(FLOAT_TO_FIXED(2, qn), qn_a, qn), qn);
    iroot[1] = Qn_DIVIDE(-qn_b - qn_sqrt_discriminant, Qn_MULTIPLY(FLOAT_TO_FIXED(2, qn), qn_a, qn), qn);

    printf("The Qn %d roots of %fx^2 + %fx + %f are %f %f\n", qn, a, b, c, FIXED_TO_FLOAT(iroot[0], qn), FIXED_TO_FLOAT(iroot[1], qn));

    return;
} // End findRoots


/*---------------------------------------------------------------------------
  Description: This algorithm uses a Modified Newton approach to find the
               closest, smaller value, of the square root of passed value.
               The value passed must be non-negative.  This does not support
               imaginary values.
               
   Where:   int s - The value 
   
   Returns: the integer square root
   Errors:  Exits with a return code of 99 on error
---------------------------------------------------------------------------*/
int i_sqrt ( int s ) {
   int x0, x1;
   
   // Check for errors
   if (s < 0) {
      printf("Error: i_sqrt works for only non-negative inputs %d\n", s); 
      exit(99);
   }
   
   // If the value is less than 2 the root is the value
   if (s < 2) { 
      return s;
   }

   // Initial estimate and guess
   x0 = s >> 1; 
   x1 = ( x0 + (s / x0) ) >> 1;  
                     
   // Newtons method, 
   while ( x1 != x0 ) {       
      x0 = x1;
      x1 = ( x0 + (s / x0) ) >> 1;
   }  // End while

   return x0;
} // End i_sqrt



/*---------------------------------------------------------------------------
  Description: This algorithm uses a Modified Newton approach to find the
               closest, smaller value, of the square root of the passed Qn
               value.  The value passed must be non-negative.  This does not 
               support imaginary values.
               
   Where:   int s - The Qn based value 
            int q - Qn base
            
   Returns: the integer Qn square root
   Errors:  Exits with a return code of 99 on error
---------------------------------------------------------------------------*/
int q_sqrt(int s, int q) {
    int x0, x1;

    // Check for errors
    if (s < 0) {
        printf("Error: q_sqrt works for only non-negative inputs %d\n", s);
        exit(99);
    }

    // If the value is less than 2 the root is the value
    if (s < 2) {
        return s;
    }

    // Initial estimate and guess, adjusted for Qn format
    x0 = s >> 1;
    x1 = (x0 + Qn_DIVIDE(s, x0, q)) >> 1;

    // Modified Newton's method, adjusted for Qn format
    while (x1 < x0) {  // Continue until the estimate starts increasing
        x0 = x1;
        x1 = (x0 + Qn_DIVIDE(s, x0, q)) >> 1;
    }  // End while

    return x0;
} // End q_sqrt


