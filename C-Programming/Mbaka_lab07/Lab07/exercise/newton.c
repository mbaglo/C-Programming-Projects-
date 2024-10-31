/*-----------------------------------------------------------------------
  Simple code to test a newton solver.
  student copy
-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ClassErrors.h"
#include "../rootfinding.h"

double newton(func1arg f, func1arg df, double x0, int Nmax, double atol, int verb);

#define NEWTON_LIMIT   (1000)
#define VERBOSE        (1)

/*-----------------------------------------------------------------------
  The function contains the equation to solve.
  
  Where:   double x  - the point to evaluate
  Returns: double    - the value of the function at the point.
  Error Handling: none
-----------------------------------------------------------------------*/
double func(double x) { 
   return ((0.02 * x  - 0.75)* x  - 52.2 )* x + 1909.0;
} 

/*-----------------------------------------------------------------------
  The function contains the derivative of the equation to solve.
  
  Where:   double x  - the point to evaluate
  Returns: double    - the value of the function at the point.
  Error Handling: none
-----------------------------------------------------------------------*/
double funcDer(double x) { 
   return (0.06 * x  - 1.5 )* x - 52.2; 
} 

int main(int argc, char *argv[]) { 
   // The initial guess 
   double a = -50.0;
   double tol = 0.01;
   double result;
   
   result = newton(&func, &funcDer, a, NEWTON_LIMIT, tol, VERBOSE);
   printf("Newton returned root: %f\n", result);
   return 0; 
} 

/******************************************************************************
 Purpose: Finds a root of function f using the newton method. x0 is the initial 
 guess,  df is the derivative of function f , Nmax is the maximum number
  of iterations, atol is the tolerance, and verb will turn a verbose print* out

  Where: func1arg f  - function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         func1arg df - derivative of function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         double x0   - initial guess for location of root
         int Nmax    - maximum number of iterations to achieve convergence
         double atol - absolute error convergence tolerance
         int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns NAN
******************************************************************************/
double newton(func1arg f, func1arg df, double x0, int Nmax, double atol, int verb) {
   double fx0;		/* Value of f(x0) */
   double dx0;		/* Derivative of f(x0) */
   double x1;		/* New x0 value */
   int i=0;		/* Iteration */

   if(fabs(f(x0))< atol){
       return x0;
   }

   while(i < Nmax)     {
      /* Evaluate the function at the initial point */
      fx0 = f(x0);
      dx0 = df(x0);

      /* If the slope or derivative is close to zero then Newton won't converge */ 
      if(fabs(dx0) < atol) {
         fprintf(stdout, "Newton could NOT converge on a root.\n");
         return(NAN);
      }
      
      /* Calculate the updated point */
      x1 = x0 - fx0/dx0;
      
      /* If the two guesses are very close we are done */
      if(fabs(x1-x0) < atol) {
         return x1;
      }
      
      if(verb) {
         fprintf(stdout, "iter:%d	x0:%f	x1:%f	err:%f\n",
                  i, x0, x1, fabs(x1 - x0));
      }
      x0 = x1;
      i++;
   }
   fprintf(stdout, "Newton could not find a root with %d iterations\n", Nmax);
   return(NAN);
}
