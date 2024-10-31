/**************************************************************************
  intTest.c - for students
  This program demonstrates the speed advantages of integer versus floating
  point math.
 12/28/2015 - R. Repka  Initial version
 10/12/2016 - R. Repka  Switched to PRINT_RTIMER() macro
 03/25/2018 - R. Repka  Switched to timing macros 
 11/18/2019 - R. Repka  Polynomial comment error 
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"
#include <float.h>
#include <math.h>





/****************************************************************************
  Use this to define the number of iterations in your performance measurements
  ----------- You will need to adjust this number  --------
****************************************************************************/
#define ITERATIONS   (5000000)

/* Optional verbose flag, compile with -DVERBOSE or not */
#ifdef VERBOSE
   #undef VERBOSE 
   #define VERBOSE (1)
#else 
   #define VERBOSE (0)
#endif



/****************************************************************************
  Remember to set the QN value in rootfinding.h
****************************************************************************/

/************************************************************************
   Simple code to test Qn performance vs floating point  
************************************************************************/
int main(int argc, char* argv[])
    {
    double Result;          /* The result from floating point calculation   */
    long  iResult;          /* The result from interger calculation   */
    
    /* The bisection guesses and tolerance value                        */
    double a = -25;
    double b = 25;
    double tol= 0.000001;

   // Convert the tolerance and guesses to fixed-point format using QN from rootfinding.h
    long iA = FLOAT_TO_FIX(a);
    long iB = FLOAT_TO_FIX(b);
    long iTol = FLOAT_TO_FIX(tol);

    /* You many need to modify the timing code for your macro implementation! */

    DECLARE_TIMER(timer);
    DECLARE_REPEAT_VAR(tloop);

    /************************************************************************
     Run the Qn format test 
    ************************************************************************/
    /* You many need to modify the timing code for your macro implementation! */

    START_TIMER(timer);
    BEGIN_REPEAT_TIMING(ITERATIONS,tloop)

         iResult = ibisection(&iPoly, iA, iB, iTol, VERBOSE);

    
    END_REPEAT_TIMING
    STOP_TIMER(timer);

    if(iNAN == iResult)
        {
        fprintf(stderr, "Qn %d bisection couldn't find a root. \n", QN);
        exit(99);
        }
    else
        {

        // Convert the fixed-point root back to floating-point
       
        double floatRoot = FIX_TO_FLOAT(iResult);
        fprintf(stderr, "Found the root %f using Qn %d interger bisection.\n", floatRoot, QN);
        }

 
    PRINT_TIMER(timer);
    PRINT_RTIMER(timer, ITERATIONS);

    /************************************************************************
     Run the floating point format test 
    ************************************************************************/
    RESET_TIMER(timer);
    START_TIMER(timer);
    BEGIN_REPEAT_TIMING(ITERATIONS,tloop)

        Result = bisection(&rPoly, a, b, tol, VERBOSE);

      
    END_REPEAT_TIMING
    STOP_TIMER(timer);

    if(NAN == Result)
        {
        fprintf(stderr, "Float bisection couldn't find a root. \n");
        exit(99);
        }
    else
        {
        fprintf(stderr, "Found the root %f using float bisection.\n",
                        Result);
        }

    PRINT_TIMER(timer);
    PRINT_RTIMER(timer, ITERATIONS);

    /* Display the percentage error */
     float PResult = (FIX_TO_FLOAT(iResult) - Result) / Result * 100.0;
    fprintf(stderr, "Qn %d error= %f\n", QN, PResult);
    return 0;
    } /* End main */   
    
    

/******************************************************************************
 Purpose: Finds a root of scalar, non-linear function f using the integer 
 bisection  method. a and b make up the initial bracket to start bisecting from.
 Only implement this after you have the floating point function implemented
 
 Where: ifunc1arg f - INTEGER function whose root is to be determined
                      must take a single argument of type long and return
        long a      - initial Qn root bracket guess
        long b      - initial Qn root bracket guess
        long atol   - absolute Qn error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE
        
Returns: long - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN                 
******************************************************************************/
long ibisection(ifunc1arg f, long a, long b, long atol, int verb) {

    long fa = f(a), fb = f(b);
    if (fa * fb >= 0) {
        fprintf(stderr, "A and B do not bracket a root\n");
        return iNAN;  // Make sure iNAN is a value that cannot be a valid root.
    }

    long c, fc;
    for (int i = 0; i < BI_LIMIT; i++) {
        c = Qn_DIVIDE(a + b, FLOAT_TO_FIX(2.0)); // Use fixed-point division
        fc = f(c);

        if (llabs(fc) < atol || Qn_DIVIDE(llabs(b - a), FLOAT_TO_FIX(2.0)) < atol) {  // Check using fixed-point arithmetic
            return c;
        }

        if (verb) {
            fprintf(stdout, "Iter: %d, a: %f, b: %f, Midpoint: %f, f(c): %f, Interval Width: %f\n",
            i,  // Current iteration
            FIX_TO_FLOAT(a),  // Convert 'a' from fixed to floating-point
            FIX_TO_FLOAT(b),  // Convert 'b' from fixed to floating-point
            FIX_TO_FLOAT(c),  // Convert midpoint 'c' from fixed to floating-point
            FIX_TO_FLOAT(fc), // Convert function value 'fc' at 'c' to floating-point
            FIX_TO_FLOAT(llabs(b - a)) / 2.0);  // Convert interval width to floating-point and divide by 2 for readability
}


        if (fc * fa < 0) b = c;
        else {
            a = c;
            fa = fc;
        }
    }
    fprintf(stderr, "Bisection method did not converge within %d iterations\n", BI_LIMIT);
    return iNAN;
}

/*---------------------------------------------------------------------------
  This function implements the real polynomial equation to be solved.  
  
  Where:   double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double rPoly(double x){
    //x**3-.0001x**2-676X+.0676    
    return(x*(x*(x-.0001)-676.0)+.0676);
}


/*---------------------------------------------------------------------------
  This function implements the integer polynomial equation to be solved.  

  ***   You must use Horner's to get good results  ***
  
  Where:    long x - the value to evaluate
  Returns: long - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
long iPoly(long x){
    // Polynomial: x^3 - 0.0001x^2 - 676x + 0.0676

    // Coefficients in Qn format
    long coef1 = FLOAT_TO_FIX(1.0);       // Coefficient for x^3
    long coef2 = FLOAT_TO_FIX(-0.0001);   // Coefficient for x^2
    long coef3 = FLOAT_TO_FIX(-676.0);    // Coefficient for x
    long coef4 = FLOAT_TO_FIX(0.0676);    // Constant term

    // Calculate each term of the polynomial
    long term1 = Qn_MULT(Qn_MULT(Qn_MULT(x, coef1), x), x);  // x^3 term
    long term2 = Qn_MULT(Qn_MULT(x, coef2), x);              // -0.0001x^2 term
    long term3 = Qn_MULT(x, coef3);                          // -676x term
    long term4 = coef4;                                      // 0.0676 term (constant)

    // Sum all terms to get the polynomial value
    long result = term1 + term2 + term3 + term4;

    return result;
}
