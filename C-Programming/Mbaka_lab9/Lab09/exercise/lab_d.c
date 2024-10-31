/*-----------------------------------------------------------------------
  Simple program to illustrate complex polynomial calculations including
  the quadradic equation. 
  student copy
-----------------------------------------------------------------------*/
#include </usr/include/complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ClassErrors.h"
#include "poly.h"


int main(int argc, char * argv[]) {
   
   // Variables to use or delete as you wish.
   polynomial p, pDeriv;    
   double complex val;
   rootPair res;                
   int i, n;
   
   
   // Create a polynomial to test
   p.nterms = 4;
   p.polyCoef = malloc(p.nterms*sizeof(double complex));
   

   // The derivative is always 1 order less
   pDeriv.nterms = p.nterms -1;
   pDeriv.polyCoef = malloc(pDeriv.nterms*sizeof(double complex));
   
   // Check the malloc
   if ((NULL == p.polyCoef) || (NULL == pDeriv.polyCoef)) {
      fprintf(stderr, "malloc() failed in %s, line %d\n", __FILE__, __LINE__);
      fflush(stderr);
      exit(MALLOC_ERROR);
   }

   // Put the coefficient of   x^3 +3x^2 +7x +5 into the polynomial structure
   // high term first.  C will automatically convert to complex
   p.polyCoef[0] = 1.0;
   p.polyCoef[1] = 3.0;
   p.polyCoef[2] = 7.0;
   p.polyCoef[3] = 5.0;
   
   /*------------------------------------------------------------------------
     Insert your derivative generation code here
   ------------------------------------------------------------------------*/

   for (i = 0; i < pDeriv.nterms; i++) {
        pDeriv.polyCoef[i] = p.polyCoef[i] * (pDeriv.nterms - i);
   }
   // Printing the original and derivative polynomials...
    printf("Original polynomial:\n");
    printPoly(&p);
    printf("Derivative polynomial:\n");
    printPoly(&pDeriv);


   /*------------------------------------------------------------------------
     Insert your polynomial evaluation code here
   ------------------------------------------------------------------------*/

   printf("Polynomial evaluations:\n");

    // Evaluate the original polynomial at 2.0 + 3.0I and print the result
    val = cevalPoly(&p, 2.0 + 3.0 * I);
    printf("p(2.0+3.0I) is ");
    printComplex(val);  // Ensure this prints in the format "a + bi"
    printf("\n");
   
    // Evaluate the derivative polynomial at 2.0 + 3.0I and print the result
    val = cevalPoly(&pDeriv, 2.0 + 3.0 * I);
    printf("pDeriv(2.0+3.0I) is ");
    printComplex(val);  // Ensure this prints in the format "a + bi"
    printf("\n");


   /*------------------------------------------------------------------------
     Insert your quadradic equation code here
   ------------------------------------------------------------------------*/
   polynomial quad1 = {3, malloc(3 * sizeof(double complex))};
   polynomial quad2 = {3, malloc(3 * sizeof(double complex))};
   quad1.polyCoef[0] = 1.0; // x^2
   quad1.polyCoef[1] = -1.0; // x^1
   quad1.polyCoef[2] = 1.0; // x^0
   quad2.polyCoef[0] = 1.0; // x^2
   quad2.polyCoef[1] = 3.0; // x^1
   quad2.polyCoef[2] = 2.0; // x^0


 // For the first quadratic polynomial
   printf("Quadratic equation:\nP(x) = 1x^2 - 1x^1 + 1x^0\n");
   res = quadraticRoots(&quad1);  // Use 'res' to store the roots
   printf("Roots: ");
   printComplex(res.root[0]);  // Print the first root using 'res'
   printf(" and ");
   printComplex(res.root[1]);  // Print the second root using 'res'
   printf("\n");

   // For the second quadratic polynomial
   printf("Quadratic equation:\nP(x) = 1x^2 + 3x^1 + 2x^0\n");
   res = quadraticRoots(&quad2);  // Reuse 'res' for the second set of roots
   printf("Roots: ");
   printComplex(res.root[0]);  // Print the first root of the second equation using 'res'
   printf(" and ");
   printComplex(res.root[1]);  // Print the second root of the second equation using 'res'
   printf("\n");

   // Memory cleanup
   free(p.polyCoef);
   free(pDeriv.polyCoef);
   free(quad1.polyCoef);
   free(quad2.polyCoef);

   return(0);
}


/*---------------------------------------------------------------------------
  Returns the two roots from a quadratic polynomial using the quadradic equation.
  Do NOT use cpow() function
  
               =  -b +-sqrt(b^2-4ac)             ^ - power 
                  ----------------------
                        2a
  
  Where:    polynomial *p - Pointer to a 2nd order polynomial
  
  Returns: rootPair - A pair of complex roots
  
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
rootPair quadraticRoots( polynomial *p){

    double complex a = p->polyCoef[0];
    double complex b = p->polyCoef[1];
    double complex c = p->polyCoef[2];
    double complex discriminant = csqrt(b * b - 4 * a * c);
    rootPair roots;
    roots.root[0] = (-b + discriminant) / (2 * a);
    roots.root[1] = (-b - discriminant) / (2 * a);
    return roots;


} // End quadraticRoots


/*---------------------------------------------------------------------------
  This function evaluates a polynomial at a complex point z.  
  Don't use the cpow() function, it generates round off errors.  You are 
  required to use Horner's factorization.  This assumes data is 
  stored high degree first
  
  Were: polynomial *p    - Pointer to the polynomial to evaluate
        double complex z - Point to evaluate 
        
  Returns: double complex- Complex solution
  
  Errors:  none
---------------------------------------------------------------------------*/
double complex cevalPoly(polynomial *p, double complex z){

    double complex sum = p->polyCoef[0];
    for (int i = 1; i < p->nterms; i++) {
        sum = sum * z + p->polyCoef[i];
    }
    return sum;


} // End cevalPoly
 
 
/*---------------------------------------------------------------------------
  Prints a polynomial
  Where: polynomial *p - Pointer to polynomial data structure to print
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void printPoly (polynomial *p){
   int i, n, end;
   n = p->nterms - 1;
   fprintf(stdout, "P(x) = ");
   fflush(stdout);

   /* Find the last term that is not a zero */
   for (i=n; i > 0; i--){
      if( TOLERANCE <  cabs(p->polyCoef[i])){ break;}
   } // End for
   
   /*Print out the first term */
   if (TOLERANCE > fabs(cimag(p->polyCoef[0]))){
      fprintf(stdout," %gx^%d ", creal(p->polyCoef[0]), n);
   }
   else{
      fprintf(stdout," [%g + %gi]x^%d ", creal(p->polyCoef[0]),cimag(p->polyCoef[0]), n);
   }

   /* Print out all except for the first term */
   end = i;
   for ( i=1; i <= end; i++){
      if (TOLERANCE < cabs(p->polyCoef[i])){
         if (TOLERANCE > fabs(cimag(p->polyCoef[i]))){
            if(TOLERANCE > creal(p->polyCoef[i])){
               fprintf(stdout,"- %gx^%d ", fabs(creal(p->polyCoef[i])), n-i);
            }
            else{
               fprintf(stdout,"+ %gx^%d ", creal(p->polyCoef[i]), n-i);
            }
         }
         else{
            fprintf(stdout,"+ [%g + %gi]x^%d ", creal(p->polyCoef[i]),cimag(p->polyCoef[i]), n-i);
         }
      } // End if zero
   } // End for i
   fprintf(stdout, " \n");
} // End printPoly


/*---------------------------------------------------------------------------
  The function prints out complex data
  
  Where: double complex x - the complex data to print out
  returns:  nothing
  errors:   none
---------------------------------------------------------------------------*/
void printComplex(double complex x){
   if (TOLERANCE > fabs(creal(x))){
       fprintf(stdout, " 0.0 ");
   }
   else{
       fprintf(stdout," %g ", creal(x));
   }
   
   if (TOLERANCE < fabs(cimag(x))){
      if (cimag(x) < 0.0) { fprintf(stdout," %gi ",cimag(x));}
      else                { fprintf(stdout," +%gi ",cimag(x));}

   }
} // End printComplex
