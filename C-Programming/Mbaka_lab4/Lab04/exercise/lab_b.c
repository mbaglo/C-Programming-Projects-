/****************************************************************************
 The purpose of this example is to exercise, creating, using and destroying
 pass by pointer and pass by value data structures
 
 student version 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_a.c -o lab_a
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99 ./lab_a
 
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*---------------------------------------------------------------------------
  The polynomial data structure contains the number of (size) of the
  allocated array of polynomial coefficients 
  e.g     nterms = 4;
          polyCoef = malloc(sizeof(double)*4);    
---------------------------------------------------------------------------*/
typedef struct {
    unsigned int nterms;      /* number of terms */
    double *polyCoef;         /* coefficients         */
} Poly;

// Function prototypes
void createPoly(Poly *p, int n);
Poly *createPoly_p(int n);
void destroyPoly(Poly *p);
void destroyPoly_p(Poly *p);
void printPolyRef(Poly *p);
void printPolyVal(Poly data);

int main(int argc, char *argv[]) {
   	Poly stackPoly;         // Stack data structure
   	Poly *poly_p;           // Pointer 
   
   	// Create and use the data structure
   	createPoly(&stackPoly, 10); // Create polynomial with 10 terms
    
   	// Print using pass by pointer AND value
   	printPolyRef(&stackPoly);
   	printPolyVal(stackPoly);

   	// Destroy the polynomial variable
   	destroyPoly(&stackPoly);

   
   	// Create and use the pointer as a data structure NOT using creatPoly_p()
  	poly_p = (Poly *)malloc(sizeof(Poly));
    	if (poly_p == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    	}
	
    	createPoly(poly_p, 10);

   	// Print using pass by pointer AND value
	printPolyRef(poly_p);
	printPolyVal(*poly_p);

	// Destroy the polynomial variable by freeing the allocated memory
    	destroyPoly_p(poly_p);
   
   	// Create and use the pointer as a data structure using creatPoly_p()
 	poly_p = createPoly_p(10);
   	// Print using pass by pointer AND value
	printPolyRef(poly_p);
	printPolyVal(*poly_p);

	// Destroy the polynomial variable by freeing the allocated memory
    	destroyPoly_p(poly_p);
   	return 0;
}

/*---------------------------------------------------------------------------
  Create a test polynomial using a pointer to the data structure
  Be sure to initialize the elements aka: polyCoef[i]  2.0*i
---------------------------------------------------------------------------*/
void createPoly(Poly *p, int n){
   
   p->nterms = n;
    p->polyCoef = malloc(sizeof(double) * n);
    if (p->polyCoef == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        p->polyCoef[i] = 2.0 * i;
    }
}

/*---------------------------------------------------------------------------
  Create a test polynomial using a pointer to the data structure
---------------------------------------------------------------------------*/
// Destroy data structure based test polynomial 
void destroyPoly(Poly *p){
   free(p->polyCoef);
   
}

/*---------------------------------------------------------------------------
  Create a test polynomial returning a pointer to the data structure
    Be sure to initialize the elements aka: polyCoef[i]  2.0*i
---------------------------------------------------------------------------*/
Poly *createPoly_p(int n){
   Poly *p = malloc(sizeof(Poly));
    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    p->nterms = n;
    p->polyCoef = malloc(sizeof(double) * n);
    if (p->polyCoef == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        p->polyCoef[i] = 2.0 * i;
    }
    return p;
}

/*---------------------------------------------------------------------------
  Destroy pointer based test polynomial 
---------------------------------------------------------------------------*/
void destroyPoly_p(Poly *p){
   free(p->polyCoef);
    free(p);
}

/*---------------------------------------------------------------------------
  Prints the polynomial data passing by reference use %3.0f
---------------------------------------------------------------------------*/
void printPolyRef(Poly *p){
   printf("Print poly by reference: ");
    for (int i = 0; i < p->nterms; i++) {
        printf("%3.0f ", p->polyCoef[i]);
    }
    printf("\n");
}

/*---------------------------------------------------------------------------
  Prints the polynomial data passing by value use %3.0f
---------------------------------------------------------------------------*/
void printPolyVal(Poly data){
   printf("Print poly by value: ");
    for (int i = 0; i < data.nterms; i++) {
        printf("%3.0f ", data.polyCoef[i]);
    }
    printf("\n");
}



