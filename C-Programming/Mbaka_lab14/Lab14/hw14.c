/*---------------------------------------------------------------------------
   hw14.c - for students 
   06/23/2020 R. Repka     - Added spring and tank modes, removed inverted pendulum
   05/03/2021 R. Repka     - Make tank required
 ---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "ODEsolvers.h"

/*---------------------------------------------------------------------------
  Test tool to verify the ODE solver 
---------------------------------------------------------------------------*/
int main (int argc, char *argv[]) {
   double t = 0;             /* time: temp variable                 */
   double x0[NX];            /* Pointer to initial conditions       */

   int i;                /* loop counter */
   char *solver;
   funArgs problemFun;
   void (*simulator)( simParm *sim, double t0, double *x0, funArgs f );

   /* Initialize the initial conditions to unknown */      
   for (i = 0; i < NX; i++)     {         /* read initial condition    */
      x0[i] = INFINITY;
   }
   
   /*------------------------------------------------------------------------
      UI variables with sentential values
   ------------------------------------------------------------------------*/
   int verbose = 0;
   double tf = INFINITY;             /* simulation final time               */
   simParm simParms = {NX, INFINITY, 0.0};  /* Holds the simulator parameters */

   /*------------------------------------------------------------------------
     These variables are used to control the getopt_long_only command line 
     parsing utility.  
   ------------------------------------------------------------------------*/
   int rc;
   /* getopt_long stores the option index here. */
   int option_index = 0;
  
   /* This contains the short command line parameters list */
   char *getoptOptions = "vz:f:d:";
   
   /* The execution mode */
   enum modes    {UNDEF_MODE = 0, EU = 1, RK2 = 2, RK3 = 3, RK4 = 4} mode = UNDEF_MODE;
   enum problems {UNDEF_PROB = 0, TANK = 1, SPRING = 2} problem = UNDEF_PROB;
   
   /* This contains the long command line parameter list, it should mostly 
     match the short list                                                  */
   struct option long_options[] = {
      /* These options don’t set a flag.
         We distinguish them by their indices. */

      {"verbose", no_argument, 0, 'v'},
      {"verb",    no_argument, 0, 'v'},
      
      {"eu",  no_argument, (int *)&mode, EU},
      {"rk2", no_argument, (int *)&mode, RK2},
      {"rk3", no_argument, (int *)&mode, RK3},
      {"rk4", no_argument, (int *)&mode, RK4},
            
      {"tank",     no_argument, (int *)&problem, TANK},
      {"spring",   no_argument, (int *)&problem, SPRING},
      
      {"damp",      required_argument, 0, 'd'},
      {"dampening", required_argument, 0, 'd'},

      {"x1", required_argument, 0, '1'},
      {"x2", required_argument, 0, '2'},
      
      {"stepSize",  required_argument, 0, 'z'},
      {"stepsize",  required_argument, 0, 'z'},
      {"step",      required_argument, 0, 'z'},
      {"size",      required_argument, 0, 'z'},
      
      {"finalTime", required_argument, 0, 'f'},
      {"finaltime", required_argument, 0, 'f'},
      {"ftime",     required_argument, 0, 'f'},
      {"final",     required_argument, 0, 'f'},
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

        case 'f':                   /* final time */
           tf = atof(optarg);
          break;

        case 'z':                   /* step size */
          simParms.h = atof(optarg);
          break;
          
        case 'd':                   /* dampening value */
          simParms.damp = atof(optarg);
          break;
          
        case '1':                   /* x1 */
          x0[0] = atof(optarg);
          break;
          
        case '2':                   /* x2 */
          x0[1] = atof(optarg);
          break;
          
        case  0 :  /* no short form case  */
          break;
          
        case '?':  /* Handled by the default error handler */
          break;

       default:
          printf ("Internal error: undefined option %0xX\n", rc);
          exit(PGM_INTERNAL_ERROR);
       } // End switch 
   } /* end while */
   
    if (verbose) {
       if      (mode == EU)  { solver = "EU";} 
       else if (mode == RK2) { solver = "RK2";}
       else if (mode == RK3) { solver = "RK3";}
       else if (mode == RK4) { solver = "RK4";}
       else                  { solver = "Undefined";}
    
      printf("Input parameters ODE %s, simulate by %f to %f\n",  solver, simParms.h, tf);
      printf("damp %f init value = %f %f\n\n", simParms.damp, x0[0], x0[1]);
      printf("   Time            x1               x2\n");
    }
   
 
   /* Make sure required parameters are set */      
   rc = 0;
   for (i = 0; i < NX; i++)     {         /* read initial condition    */
      if (x0[i] == INFINITY) { rc = 99;}
   }

   /* Check for missing command line arguments */
   if ((optind < argc) || (mode == UNDEF_MODE) || (tf == INFINITY) || 
       (problem == UNDEF_PROB) || (simParms.h  == INFINITY) || (rc) ) {
      /* Incorrect invocation **********************************************/
      printf("This simulates various system\n");
      printf("Usage: %s solver problem -step num -fTime num  initCond [%d] [-damp num]\n", argv[0], NX);
      printf("Where: solver    = -eu, -rk2, -rk3, or -rk4\n");
      printf("       problem   = -tank or -spring\n");
      printf("       -step     = simulation step size\n");
      printf("       -ftime    = final time\n");
      printf("       -damp     = spring dampening value, default is 0.0\n");
      printf("       -x1, -x2,  =initCond = initial conditions 'vector' (%d)\n", NX);
      printf(" e.g.   %s -rk4 -tank -step.15 -ftime 25 -x1 0 -x2 0\n", argv[0]);
      printf("        Use RK4 to simulate the water tank for 25 seconds at .15 sec/step\n\n"); 
      return(PGM_SYNTAX_ERROR);
    }
           
   /************************************************************************
   * Choose the problem to solve
   ************************************************************************/
   if (problem == SPRING)        { problemFun = &springMass; }
   else                          { problemFun = &tank;}
    
  /************************************************************************
   * Choose the simulator to use 
   ************************************************************************/
   if (mode == EU)       { simulator = &eu; }
   else if (mode == RK2) { simulator = &rk2;}
   else if (mode == RK3) { simulator = &rk3;}
   else                  { simulator = &rk4;}
       
   /************************************************************************
   * Simulate the system 
   ************************************************************************/
   do
     {
       printf (" %14.8e", t);
       /* initial conditions  */
       for (i = 0; i < NX; i++){    
          printf (" %14.8e", x0[i]);
       }
       printf ("\n");
       
       /* Simulate */
       simulator(&simParms, t, x0, problemFun);
       t = t + simParms.h; 
     } while (t <= tf);

  return(PGM_SUCCESS);
} /* main */
