/***********************************************************************
 * Header file for polynomial module (uses C99)
 * Course: Applied Programming
 * Revised: 2015
 * 
 * Module provides: 
 *         public:  createPoly, freePolym, printPoly, 
 *                  cevalPoly, printRoots, roots.
 * Author: JCCK
 * Revised: March 2015
 * 11/08/2015 R. Repka  - Removed evalPoly, added double complex, 
 *                        removed extern, added NAN
 * 10/26/2016 R. Repka  - Changed createPoly to initPoly
 * 07/21/2017 R. Repka  - Removed NAN, now in ClassErrors.h
 * 04/03/2018 R. Repka  - Added SIGN macro
 ***********************************************************************/ 
#include </usr/include/complex.h>

#ifndef _POLY_H_
#define _POLY_H_

/* Macros */
#define SIGN(x) ((x) < 0 ? -1 :1)

#define TOLERANCE        (0.00000001)
#define MAX_LAGUERRE (30)
#define MAX_POLY    256

/* Typemarks **********************************************************
 * The data structure to hold all coefficients of a polynomial.
 * A integer value to indicate how many coefficients in the struct and
 * a double complex pointer to the value of coefficients.
 **********************************************************************/
typedef struct {
   int nterms;                /* number of terms */
   double complex *polyCoef;  /* coefficients    */
} polynomial;

// Roots from the quadratic equation 
typedef struct {
   double complex root[2];
} rootPair;

// Polynomial, first and second derivative return value
typedef struct {
   double complex val[3];
} polyEval;


/* Function prototypes ************************************************/
void initPoly(polynomial *p, int nterms);
void freePoly(polynomial *p);
void printPoly(polynomial *p);
void printRoots(polynomial *p, int verb);
void printPoint(double complex x, double complex y);
double complex cevalPoly(polynomial *p, double complex z);
double complex* roots( polynomial *p, int verb);
rootPair quadraticRoots( polynomial *p);
double complex laguerre(polynomial *p, double tol, int verb);
polynomial* deflPoly(polynomial *p, double complex root);
polyEval evalDerivs(polynomial *p, double complex point);
void printComplex(double complex x);
#endif /* _POLY_H_ */
