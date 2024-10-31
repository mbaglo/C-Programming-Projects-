/******************************************************************************
 * student copy 
 * Program to test finding the roots of real polynomials that have real
 * and complex roots.  
 * 
 * 11/08/2015 R. Repka  - General clean up, pass a verbose flag
 * 10/26/2016 R. Repka  - Changed createPoly to initPoly
 * 07/11/2017 R. Repka  - Switched to getopt_long_only
 * 07/21/2017 R. Repka  - Added ClassErrors.h
 * 12/24/2017 R. Repka  - switched to strncpy()
 * *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "poly.h"


/* Local functions */
void ReadPoly(polynomial *poly, char *String);
void ReadPoint(double complex *point, char *String);

/*---------------------------------------------------------------------------
  This program is used to verify HW6, polynomials.  It is used test finding 
  roots of real polynomials
  Syntax:  ./hw6 verbose polynomials [points]
---------------------------------------------------------------------------*/
int main(int argc, char* argv[]){ 
    FILE *PolyFile;
    polynomial poly;
    char String[MAX_POLY];
    
   /*------------------------------------------------------------------------
      UI variables with sentential values
   ------------------------------------------------------------------------*/
   int verbose = 0;
   char *inName = NULL;

   /*------------------------------------------------------------------------
     These variables are used to control the getopt_long_only command line 
     parsing utility.  
   ------------------------------------------------------------------------*/
   int rc;
   /* getopt_long stores the option index here. */
   int option_index = 0;
  
   /* This contains the short command line parameters list */
   char *getoptOptions = "vi:";
  
   /* This contains the long command line parameter list, it should mostly 
     match the short list                                                  */
   struct option long_options[] = {
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"verbose", no_argument, 0, 'v'},
      {"verb",    no_argument, 0, 'v'},
      {"input",   required_argument, 0, 'i'},
      {"in",      required_argument, 0, 'i'},
      {0, 0, 0, 0}
   };
  
   opterr = 1;           /* Enable automatic error reporting */
   while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, 
                                                    &option_index)) != -1) {
      
   //  printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);   
      /* Detect the end of the options. */
      switch (rc)
        {
        case 'v':                    /* Verbose */
          verbose = 1;
          break;

        case 'i':                   /* input name */
          inName = optarg;
          break;

        case '?':  /* Handled by the default error handler */
          break;

       default:
          printf ("Internal error: undefined option %0xX\n", rc);
          exit(PGM_INTERNAL_ERROR);
       } // End switch 
   } /* end while */

   /*------------------------------------------------------------------------
     Check for command line syntax errors
   ------------------------------------------------------------------------*/
   if ((optind < argc) || (inName == NULL)){
      fprintf(stderr, "Program to test finding the roots of real polynomials\n");
      fprintf(stderr, "usage: hw9  -i[n[pt]] file   <-v[er[bose]]>\n");
      fprintf(stderr, " e.g.  ./hw9 -input poly.txt -ver \n");
      fflush(stderr);
      return(PGM_SYNTAX_ERROR);
   } /* End if error */

   /* Finding roots of polynomials in a file */
   PolyFile = fopen(inName, "r");
   if (NULL != PolyFile){
      /* Read the polynomial from the file, highest power first */
      while(fgets(String, MAX_POLY, PolyFile)){
         ReadPoly(&poly, String);
         printPoly(&poly);
         printRoots(&poly, verbose);
         freePoly(&poly);
         fprintf(stdout, " \n");
       } /* End while polyfile */
       fclose(PolyFile);   
   } /* End if file open good */
   else {
      fprintf(stderr, "Could not open '%s'\n", argv[2]);
      exit(PGM_FILE_NOT_FOUND);
   }

    return(PGM_SUCCESS);
} /* End main */


/*---------------------------------------------------------------------------
  Creates a polynomial from a string.  Data is assumed to be high order first
  
  Where: polynomial *poly - Pointer to the structure to store the parsed 
                            polynomial
         char *String     - The string containing the polynomial coefficients
                            highest power first
  Returns: nothing
  Errors: prints an error message and exits  
---------------------------------------------------------------------------*/
void ReadPoly(polynomial *poly, char *String){
   unsigned int i, j, k, start = 0;
   char *pch1, *pch2, *strCpy, *strCpy2;
   i = 0;
   j = 0;
   k = 0;

   strCpy  = (char *)malloc((MAX_POLY+1) * sizeof(char));
   strCpy2 = (char *)malloc((MAX_POLY+1) * sizeof(char));
   if (NULL == strCpy || NULL == strCpy2){
      fprintf(stderr, "ReadPoly(): malloc() failed in %s at line %d!\n", __FILE__, __LINE__);
      fflush(stderr);
      exit(MALLOC_ERROR);
   }
   strncpy(strCpy, String, MAX_POLY);
   strncpy(strCpy2, String, MAX_POLY);

   /* Loop through and find number of terms */
   pch1 = strtok(strCpy, " ");
   while ((pch1 != NULL) && (i < 100)){
      if((0 == start) && (TOLERANCE > abs(atof(pch1)))){
         k++;
      }
      else {
         start = 1;
      }
      i++;
      pch1 = strtok (NULL, " ");
   }
   initPoly(poly, i-k);
   
   /* Loop through and add coefficients */
   pch2 = strtok(strCpy2, " ");
   while ((pch2 != NULL) && (j < i)){
      if((1 == start) && (TOLERANCE > abs(atof(pch2)))){
         j++;
      }
      else{
         start = 0;
         poly->polyCoef[j-k] = atof(pch2) + 0.0*I;
         j++;
      }
      pch2 = strtok(NULL, " ");
   }
   free(strCpy);
   strCpy = NULL;
   free(strCpy2);
   strCpy2 = NULL;
} /* End ReadPoly() */
