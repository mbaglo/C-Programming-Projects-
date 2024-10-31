/***********************************************************************
 * student copy
 * Functions to calculate the roots and evaluate polynomials
 * Course: Applied Programming 
 * Revised: 2015
 *
 * 10/16/2015 R. Repka  - Removed evalPoly, extern
 * 04/03/2015 R. Repka  - Added cpow() and Horner comments, 
 * 10/24/2016 R. Repka  - Updated evalDerivs comments
 * 10/26/2016 R. Repka  - Changed createPoly to initPoly
 * 07/21/2017 R. Repka  - Added ClassErrors.h, converted to macro based
 *                        implementation
 * 10/09/2017 R, Repka  - Minor comment changes
 ***********************************************************************/ 
#include </usr/include/complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ClassErrors.h"
#include "poly.h"

/*---------------------------------------------------------------------------
  Define local functions
---------------------------------------------------------------------------*/



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
    double complex discriminant = csqrt(b * b - 4.0 * a * c);
    rootPair roots;
    roots.root[0] = (-b + discriminant) / (2.0 * a);
    roots.root[1] = (-b - discriminant) / (2.0 * a);
    return roots;


} // End quadraticRoots



/*---------------------------------------------------------------------------
  This function evaluates the polynomial derivatives at a point using a
  simple method. This assumes high order first storage
  
  Where: polynomial *p        - Pointer to a polynomial data to evaluate
         double complex point - The point to evaluate
         
  Returns: polyEval  - Data structure with derivatives d, d' and d'' 
  
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
polyEval evalDerivs(polynomial *p, double complex point) {
    polyEval result;
   
    /* Create local temporary polynomials for the derivatives */
    polynomial pDeriv, pDeriv2;
    initPoly(&pDeriv, p->nterms - 1);
    initPoly(&pDeriv2, pDeriv.nterms - 1);

   
    /* Take the first derivative */
    for (int i = 0; i < pDeriv.nterms; i++) {
        pDeriv.polyCoef[i] = p->polyCoef[i] * (p->nterms - 1 - i);
    }
   
    /* Take the second derivative */
    for (int i = 0; i < pDeriv2.nterms; i++) {
        pDeriv2.polyCoef[i] = pDeriv.polyCoef[i] * (pDeriv.nterms - 1 - i);
    }
   
    /* Evaluate everything */
    result.val[0] = cevalPoly(p, point);
    result.val[1] = cevalPoly(&pDeriv, point);
    result.val[2] = cevalPoly(&pDeriv2, point);

   
    /* Clean up */
    freePoly(&pDeriv);
    freePoly(&pDeriv2);
   
    return result;
} // End evalDerivs




/*---------------------------------------------------------------------------
  Uses Laguerre's method to compute a root of a real polynomial
 
  Note: In addition to verifying that the update value alpha is greater than 
        two, also be sure to verify that polynomial evaluated at the 
        "current guess" is not less than the tolerance.  If p(x) is less than
        tol, you have found a root.
  
  Where: polynomial *p - Pointer to a polynomial structure to evaluate
         double tol    - Termination tolerance 
         int verb      - Verbose flag indicating whether to print intermediate
                         results or not ( 1 = TRUE, 0 = FALSE 
 Returns: double complex - The complex root or (NAN + 0.0*I) if  none is found
 Errors:  none
---------------------------------------------------------------------------*/
double complex laguerre(polynomial *p, double tol, int verb){
   // Possible variables, use them or not
   double complex g, h, f, alpha, x = 0.0 +0.0*I;
   polyEval derivList;
   int i = 0; // Iteration counter
   int n = p->nterms - 1;

   /*  Your solution here  */
    if (verb) {
    printf("Laguerre's Algorithm( tol = %g )\n", tol);
   }
  while (i < MAX_LAGUERRE) {
         
        derivList = evalDerivs(p, x); // Evaluate polynomial and its derivatives at x
        f = derivList.val[0];  // f(x)
        g = derivList.val[1] / f;  // G(x) = f'(x) / f(x)
        h = g * g - derivList.val[2] / f;  // H(x) = G(x)^2 - f''(x) / f(x)
      
      // Check if polynomial at current guess is below tolerance
        if (cabs(f) < tol) {
            
            return x; // Return the current guess as a root
        }
   // Calculate Laguerre's formula components
        double complex sqrtTerm = csqrt((n - 1) * (n * h - g * g));
        double complex denomPlus = g + sqrtTerm;
        double complex denomMinus = g - sqrtTerm;
        double complex denominator = (cabs(denomPlus) > cabs(denomMinus)) ? denomPlus : denomMinus;
        
        alpha = n / denominator;
        // Ensure denominator is not too small to avoid division by zero
        if (cabs(alpha) < tol) { // Check for convergence
            
            return x; // Root found
        }
           
         /* print out for verbose */
         if (1 == verb){
               fprintf(stdout, "      it: %d   x:",i );
               printComplex(x);
               fprintf(stdout, "\n    G(x):");
               printComplex(g);
               fprintf(stdout, "\n             H(x):");
               printComplex(h);
               fprintf(stdout, "\n             Alpha:");
               printComplex(alpha);
               fprintf(stdout, " \n");
         } // End if verbose

         x -= alpha; // Update the root estimate

         i++; // Increment iteration counter
      }
      // If the loop completes without returning, convergence was not achieved
      if (verb) {
        printf("Laguerre's method did not converge within %d iterations.\n", MAX_LAGUERRE);
      }
      return NAN + 0.0 * I; // Indicate failure to converge
  
} // End laguerre




/*---------------------------------------------------------------------------
 This finds all the roots (real and complex) of a real polynomial.  If there
 is a single root, it solves for it.  If there are two roots then it uses
 the quadratic formula.  If there are more than two roots, it uses Laguerre.
 
 If a complex root is found then BOTH the + and - roots are deflated from the
 polynomial.
 
 Where: polynomial *poly - Pointer to an array of polynomial coefficients
        int verb         - Verbose flag indicating whether to print intermediate
                           results or not ( 1 = TRUE, 0 = FALSE 
 
 Returns: double complex *roots - A pointer to a complex root array
 Errors:  prints an error and exits
---------------------------------------------------------------------------*/
double complex* roots(polynomial *poly, int verb){
   double complex *rootList;
   polynomial p;
   double complex x;
   rootPair quad;
   unsigned int i, n;
   
   n = poly->nterms - 1;
   initPoly(&p, n + 1);
   memcpy(p.polyCoef, poly->polyCoef, (n + 1) * sizeof(double complex));
   rootList = (double complex*)malloc(n * sizeof(double complex));
   if (NULL == rootList){
      fprintf(stderr, "malloc() failed in %s, line %d\n", __FILE__, __LINE__);
      fflush(stderr);
      exit(MALLOC_ERROR);
  }
   
   /* Process all the terms in the polynomial */
   for (i = 0; i < n; i++){
      if(p.nterms == 3){
         /*quadratic formula */
         quad = quadraticRoots(&p);
         rootList[i]     = quad.root[0];
         rootList[i + 1] = quad.root[1];
         freePoly(&p);
         if (1 == verb){
            fprintf(stdout, "    Found final two roots through quadratic formula \n");
         }
         return rootList; 
      } // End if 3 terms - quadratic
      
      else if(2 == p.nterms){
         /*simple math */
         rootList[i] = -p.polyCoef[1]/p.polyCoef[0];
         freePoly(&p);
         if(1 == verb){
            fprintf(stdout, "    Found final root with simple math \n");
         }
         return rootList;
      } // End if 2 terms
      
      /* More than 2 terms, use Laguerre */ 
      x = laguerre(&p, TOLERANCE, verb);
      if (1 == verb){
         fprintf(stdout, "    Found root %g + %gi \n", creal(x), cimag(x));
      }
      
      /* If this is a complex root then deflate both of them */
      if(fabs(cimag(x)) > TOLERANCE){
         /* imaginary numbers */
         rootList[i] = conj(x);
         deflPoly(&p, conj(x));
         i++;
         if( 1 == verb){
            fprintf(stdout, "    Found imaginary root, deflating twice \n");
         }
      } // End if complex root
      
      rootList[i] = x;
      deflPoly(&p, x);
      if(1 == verb){
         fprintf(stdout, "    Deflated: ");
         printPoly(&p);
      }
   } // End for
   freePoly(&p);
   return rootList;
} // End roots




/*---------------------------------------------------------------------------
  Initializes a polynomial data structure with nterms.  This allocates storage
  for the actual polynomial.
  
  Where: polynomial *p       - Pointer to polynomial data structure to create
         unsigned int nterms - The number of elements to create
  Returns: nothing
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void initPoly(polynomial *p, int nterms){
   p->nterms = nterms;
   p->polyCoef = (double complex*)malloc(p->nterms * sizeof(double complex));
   if (NULL == p->polyCoef){
      fprintf(stderr, "malloc() failed in %s, line %d\n", __FILE__, __LINE__);
      fflush(stderr);
      exit(MALLOC_ERROR);
   }
} // End initPoly


/*---------------------------------------------------------------------------
  Destroys/frees a polynomial
  
  Where: polynomial *p - Pointer to polynomial data structure to destroy
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void freePoly(polynomial *p){
   p->nterms = 0;
   free(p->polyCoef);
   p->polyCoef = NULL;
} // End freePoly


/*---------------------------------------------------------------------------
  Deflates a root from the polynomial, returning the new polynomial. Data is
      high order first.  
  
  Where: polynomial *p       - Pointer to the polynomial to deflate
                               this will be freed after use
         double complex root - The root to use 
         
  Returns: polynomial *      - Pointer to the deflated polynomial
  Errors:  none
---------------------------------------------------------------------------*/
polynomial* deflPoly(polynomial *p, double complex root){
   polynomial defl;
   unsigned int i, n;
   n = p->nterms - 1;
    
   /* Create a new polynomial structure*/
   initPoly(&defl, n);
   
   // Save the high order term
   defl.polyCoef[0] = p->polyCoef[0];
   
   // process the rest of the terms
   // Drop nth number down, multiply by root, transfer to 2nd spot, then  add. 
   for(i = 1; i < n; i++){
      defl.polyCoef[i] = root*defl.polyCoef[i - 1] + p->polyCoef[i];
   }
   
   /* destroy the old polynomial */
   freePoly(p);
   initPoly(p, n);
   memcpy(p->polyCoef, defl.polyCoef, n * sizeof(double complex));
   freePoly(&defl);
   return p;
} // End deflPoly


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
  Prints the roots of a polynomial from largest (in magnitude) to smallest
  
  Where: polynomial *p - Pointer to polynomial data structure to print
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void printRoots (polynomial *p, int verb){
   int i, j, n, max;
   double complex temp;
   double complex* rootRes;
   
   /* Calculate the roots */
   rootRes= roots(p, verb); 
   
   /* first sort them based on magnitude of complex roots) */
   n = p->nterms - 1;
   for (i = 0; i < n; ++i){
      max = i;
      for (j = i+1; j < n; ++j){
         if (cabs(rootRes[j]) > cabs(rootRes[max])) { max = j;}
      }
      
      temp = rootRes[i];
      rootRes[i] = rootRes[max];
      rootRes[max] = temp;
   } // End for
   
   /* Print the roots */
   fprintf( stdout, "Roots: \n");
   for (i = 0; i < n; i++){
      fprintf(stdout, "    ");
      printComplex(rootRes[i]);
      fprintf(stdout, " \n");
   } // End for
   
   free(rootRes);
   rootRes = NULL;
   fprintf(stdout, "\n");
} // End printRoots



