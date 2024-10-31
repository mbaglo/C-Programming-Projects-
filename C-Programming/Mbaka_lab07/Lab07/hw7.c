/**************************************************************************
  Student frame work.   Add and remove as you see fit.
  
  07/14/2017    R. Repka    Initial release
 * ***********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#ifdef EN_TIME
#include "Timers.h"
#endif

#define BI_LIMIT   (1000)
#define R1 40000


/************************************************************************
  Tests three types of root finding, secant, newton, and bisection,
  based on user input and prints out the timing results.
************************************************************************/
int main(int argc, char* argv[])
{
   /*------------------------------------------------------------------------
      UI variables with sentinel values
   ------------------------------------------------------------------------*/
   int verbose = 0;
   enum modes {UNDEF, BISECT, SECANT, NEWTON} mode = UNDEF;
   double tol = 0.0;
   double guess1 = INFINITY;   
   double guess2 = INFINITY;
   double result;

  #ifdef EN_TIME
   DECLARE_TIMER(bisectionTimer); // Declare a timer for the Bisection method
   DECLARE_TIMER(newtonTimer);    // Declare a timer for the Newton method
   DECLARE_TIMER(secantTimer);    // Declare a timer for the Secant method
   DECLARE_REPEAT_VAR(repeatVar);
 #endif

   /*------------------------------------------------------------------------
     These variables are used to control the getopt_long_only command line 
     parsing utility.  
   ------------------------------------------------------------------------*/
   int rc;
   /* getopt_long stores the option index here. */
   int option_index = 0;
  
   /* This contains the short command line parameters list */
   char *getoptOptions = "vsbnt:g:u:";    /* add lots of stuff here */
  
   /* This contains the long command line parameter list, it should mostly 
     match the short list                                                  */
   struct option long_options[] = {
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"verbose",   no_argument,       0, 'v'},
      {"verb",      no_argument,       0, 'v'},
    /* add lots of stuff here */
      {"bisection", no_argument,       0, 'b'},
      {"secant",    no_argument,       0, 's'},
      {"newton",    no_argument,       0, 'n'},
      {"tolerance", required_argument, 0, 't'},
      {"tol",       required_argument, 0, 't'},
      {"guess1",    required_argument, 0, 'g'},
      {"ga",        required_argument, 0, 'g'},
      {"guess2",    required_argument, 0, 'u'},
      {"gb",        required_argument, 0, 'u'},
      {"bracket_a", required_argument, 0, 'g'},  
      {"ba",        required_argument, 0, 'g'},  
      {"bracket_b", required_argument, 0, 'u'},  
      {"bb",        required_argument, 0, 'u'},
      {0, 0, 0, 0}
};

  
   opterr = 1;           /* Enable automatic error reporting */
   while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, 
                                                    &option_index)) != -1) {
      
 //     printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);   
      /* Detect the end of the options. */
      switch (rc)
        {
        case 'v':                    /* Verbose */
          verbose = 1;
          break;

        /* add lots of stuff here */
        case 'b':
          mode = BISECT;
          break;
        case 's':
          mode = SECANT;
          break;
        case 'n':
          mode = NEWTON;
          break;
        case 't':
          tol = atof(optarg);
          break;
        case 'g':
          guess1 = atof(optarg);
          break;
        case 'u':
          guess2 = atof(optarg);
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
   if ((optind < argc) /* add lots of stuff here */  ){
      fprintf(stderr, "Tests root finding methods\n");
      fprintf(stderr, "usage: hw7 -b[isection] | -s[ecant] | -n[ewton]   -t[ol[erance]] number\n");
      fprintf(stderr, "          -g[uessa] number   <-g[u]essb number>   <-v[erb[ose]]> \n");
      fprintf(stderr, " e.g:   hw7 -bisection -tol 1E-6 -g -2 -u 3 -verb\n"); 
      
      fflush(stderr);
      return(PGM_INTERNAL_ERROR);
   } /* End if error */
   // Check command line syntax errors and mode selection
    if (mode == UNDEF || tol <= 0.0) {
        fprintf(stderr, "Syntax error: Mode not selected or invalid tolerance\n");
        return PGM_SYNTAX_ERROR;
    }
    if (tol <= 0.0) {
    fprintf(stderr, "Error: Tolerance must be positive.\n");
    return PGM_SYNTAX_ERROR;
}
   /* Performs the root finding with bisection */
   if(mode == BISECT)
   {
        if (isinf(guess1) || isinf(guess2)) {
            fprintf(stderr, "Error: Bisection method requires two initial guesses.\n");
            return PGM_SYNTAX_ERROR;
        }
        if (guess1 >= guess2) {
            fprintf(stderr, "Error: For Bisection, the first guess must be less than the second guess.\n");
            return PGM_SYNTAX_ERROR;
        }
#ifdef EN_TIME
        START_TIMER(bisectionTimer); // Start the timer
        BEGIN_REPEAT_TIMING(R1 , repeatVar) 
        result = bisection(func1, guess1, guess2, tol, verbose);
        END_REPEAT_TIMING;
        STOP_TIMER(bisectionTimer);  // Stop the timer
      
        // Print the timer result
        PRINT_TIMER(bisectionTimer);
        PRINT_RTIMER(bisectionTimer, R1);
        RESET_TIMER(bisectionTimer); 
        printf("Bisection returned root: %f\n", result);
#endif
#ifndef EN_TIME
        result = bisection(func1, guess1, guess2, tol, verbose);
        printf("Bisection returned root: %f\n", result);
#endif    
   }  // End if bisection
   
   /* Performs the root finding using the secant method */
   else if(mode == SECANT)
   {
        if (isinf(guess1) || isinf(guess2)) {
            fprintf(stderr, "Error: Secant method requires two initial guesses.\n");
            return PGM_SYNTAX_ERROR;
        }
#ifdef EN_TIME
        START_TIMER(secantTimer); // Start the timer
        BEGIN_REPEAT_TIMING(R1 , repeatVar) 
        result = secant(func1, guess1, guess2, BI_LIMIT, tol, verbose);
        END_REPEAT_TIMING;
        STOP_TIMER(secantTimer);  // Stop the timer
     
        // Print the timer result
    
        PRINT_TIMER(secantTimer);
        PRINT_RTIMER(secantTimer, R1);
        RESET_TIMER(secantTimer); 

        printf("Secant returned root: %f\n", result);
#endif
#ifndef EN_TIME
        result = secant(func1, guess1, guess2, BI_LIMIT, tol, verbose);
        printf("Secant returned root: %f\n", result);
#endif
   } // End secant
   
   /* Performs the root finding using newtons method */
   else /* must be newton */
   {
        if (isinf(guess1)) {
            fprintf(stderr, "Error: Newton method requires an initial guess.\n");
            return PGM_SYNTAX_ERROR;
        }
#ifdef EN_TIME
        START_TIMER(newtonTimer); // Start the timer
        BEGIN_REPEAT_TIMING(R1 , repeatVar)  
        result = newton(func1, func1Deriv, guess1, BI_LIMIT, tol, verbose);
        END_REPEAT_TIMING;
        STOP_TIMER(newtonTimer);  // Stop the timer

        // Print the timer result
        PRINT_TIMER(newtonTimer);
        PRINT_RTIMER(newtonTimer, R1);
        RESET_TIMER(newtonTimer); 
        printf("Newton returned root: %f\n", result);
#endif
#ifndef EN_TIME
        result = newton(func1, func1Deriv, guess1, BI_LIMIT, tol, verbose);
        printf("Newton returned root: %f\n", result);
#endif
   } // End newton 
   
        
    return PGM_SUCCESS;
}
