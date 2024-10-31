/******************************************************************
 * Program to correct the data for the sensor
 * Note: Each student gets unique data, so this exact equation will
 * differ for each student's solution
 * Be sure to use Horner's factorization  
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Define polynomial coefficients as macros */
#ifndef A2
#define A2 -0.000358309
#endif

#ifndef A1
#define A1 1.24539
#endif

#ifndef A0
#define A0 -395.213
#endif

/* Runs the data through the fitting line */

int main(int argc, char *argv[])
{
    int real, ideal;
    float res;
    
    while(scanf("%d %d", &ideal, &real) != EOF)
    {
        /* Insert your polynomial here, be sure to round properly */
        // Using Horner's method to evaluate the polynomial at the point 'real'
        res = A2;                  // Start with the highest degree coefficient
        res = res * real + A1;     // Add next lower coefficient
        res = res * real + A0;     // Finish with the lowest coefficient

        // Correct rounding of floating point numbers to integers
        int corrected_real = real - ((res >= 0) ? (int)(res + 0.5) : (int)(res - 0.5));

        printf("%d %d\n", ideal, corrected_real);
    }
    return 0;
}
