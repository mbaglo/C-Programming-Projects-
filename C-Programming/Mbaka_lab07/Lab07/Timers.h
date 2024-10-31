/*************************************************************************
* Timers.h - Instrumentation Macros Module  - Student file
*
* The timing instrumentation macros can be globally enabled by setting 
* the variable EN_TIME (ENable TIMErs) in the compiler command line, e.g., 
* use "gcc -DEN_TIME...." to enable the timers when needed.
* A compiler warning will be issued when the timers are enabled.  
* Note: All timing output should be directed to stderr.
*
*  10/08/2016   R. Repka    Fixed C++ struct warnings 
*  12/03/2016   R. Repka    Added semicolon to DECLARE_TIMER
*  09/10/2018   R. Repka    Always use time.h
*  10/21/2018   R. Repka    Minor change to the initialized data
*  03/15/2022   R. Repka    Added RESET_TIMER() comments
*************************************************************************/
#ifndef _TIMERS_H_
#define _TIMERS_H_
#include <stdio.h>
#include <time.h>

/******************************************************************************
* Timers instrumentation macro definitions and use.
* If TIMING is *not* defined the macros are defined as null comments
* Each timer must be declared before use (pay attention to the scope)
* A timer should be stopped before printing, but in case it is printed when
* running, it will be automatically stopped.
* If a timer is not be explicitly started, the starting point will be the same 
* as the declaration
* Previously stopped timers must be started in order to accumulate time values 
* across multiple stop/run sessions, restarting a timer is performed only to
* re-stablish the starting point.
******************************************************************************/
#if defined(EN_TIME)
    struct timerDetails {                                                    
      clock_t Start;    /* Start Time   - set when the timer is started */
      clock_t Stop;     /* Stop Time    - set when the timer is stopped */
      clock_t Elapsed;  /* Elapsed Time - Accumulated when the timer is stopped */
      int State;        /* Timer State  - 0=stopped / 1=running */
    }; /* Elapsed Time and State must be initialized to zero */
	
  /****************************************************************************
  * Declare the timer.  Initialize the timer (particularly the accumulated
  * elapsed time and the timer state).
  * DO NOT USE A SEMICOLON AFTER THE MACRO in your C code, it is defined here
  ****************************************************************************/
  #define DECLARE_TIMER(A)                                                    \
    struct timerDetails                                                       \
     A = /* Elapsed Time and State must be initialized to zero */             \
      {                                                                       \
      /* Start   = */ 0,                                                      \
      /* Stop    = */ 0,                                                      \
      /* Elapsed = */ 0,                                                      \
      /* State   = */ 0                                                       \
      }; /* Timer has been declared and defined,   ;  IS required */

  /****************************************************************************
  * Start the timer.  Print an error if it is already running, set the state 
  * to running and then start the timer.
  ****************************************************************************/
  #define START_TIMER(A)                                                      \
    {                                                                         \
     /* It is an error if the timer is currently running */                   \
     if (1 == A.State) {                                                      \
       fprintf(stderr, "Error, running timer "#A" started.\n");               \
     }                                                                        \
     /* Set the state to running */                                           \
     A.State = 1;                                                             \
     /* Set the start time, done last to maximize accuracy */                 \
     A.Start = clock();                                                       \
    } /* START_TIMER() */

  /****************************************************************************
  * Reset the timer.  Clear the elapsed time.  This should be used after
  * all timer related printing is complete
   ****************************************************************************/
  #define RESET_TIMER(A)                                                      \
    {                                                                         \
     /* Reset the elapsed time to zero */                                     \
     A.Elapsed = 0;                                                           \
    } /* RESET_TIMER() */

  /****************************************************************************
  * Stop the timer.  Set the stop time, print an error message if the timer
  * is already stopped otherwise accumulate the elapsed time (works for
  * both one-time and repeating timing operations), set the state to
  * stopped.  Note: stopping the timer does NOT reset it
  ****************************************************************************/
  #define STOP_TIMER(A) \
    {                                                                         \
     /* Set the stop time, done first to maximize accuracy */                 \
     A.Stop = clock();                                                        \
     /* It is an error if the timer is currently stopped */                   \
     if (0 == A.State) {                                                      \
       fprintf(stderr, "Error, stopped timer "#A" stopped again.\n");         \
     }                                                                        \
     else { /* accumulate running total only if previously running */         \
       A.Elapsed += A.Stop - A.Start;                                         \
     }                                                                        \
     /* Set the state to stopped */                                           \
     A.State = 0;                                                             \
    } /* STOP_TIMER() */
      
/************** Write your timing macros here **************/
  /****************************************************************************
  * Declare the timer separately from the repeat loop.  
  ****************************************************************************/
  #define DECLARE_REPEAT_VAR(V)   int  _t_ ## V                               \

  /* R - repeat value, V - variable */            
  #define BEGIN_REPEAT_TIMING(R,V)                                            \
                                                                              \
  for(_t_ ## V=0; _t_ ## V < R; _t_ ## V++){                                   \

  #define END_REPEAT_TIMING    }

  /****************************************************************************
  * Print the timer.  Check the timer state and stop it if necessary, print
  * the elapsed time (in seconds). Note: You will normally want to RESET_TIMER()
  * when all printing is complete.
  ****************************************************************************/
  /* Use the print format string %.6e  to print out the per loop performance */
 #define PRINT_TIMER(A) \
    if (1 == (A).State) { \
        STOP_TIMER(A); \
    } \
    fprintf(stderr, "Elapsed CPU Time "#A" = %.2f seconds\n", ((A).Elapsed / (double)CLOCKS_PER_SEC)) \
 
 #define PRINT_RTIMER(A, R) \
    if (1 == (A).State) { \
        STOP_TIMER(A); \
    } \
    fprintf(stderr, "Elapsed CPU Time per Iteration (%s, %d) = %.6e seconds\n", #A, R, ((A).Elapsed / (double)CLOCKS_PER_SEC) / (R)) \


  
#else /* not defined(TIMING) */
  /* Declare null macros for error-free compilation */
  #define DECLARE_TIMER(A)          /* Null Macro */
  #define START_TIMER(A)            /* Null Macro */
  #define RESET_TIMER(A)            /* Null Macro */
  #define STOP_TIMER(A)             /* Null Macro */
  #define DECLARE_REPEAT_VAR(V)     /* Null Macro */
  #define BEGIN_REPEAT_TIMING(R,V)  /* Null Macro */
  #define END_REPEAT_TIMING         /* Null Macro */
  #define PRINT_RTIMER(A,R)         /* Null Macro */
  
/************** Write your dummy macros here **************/

#endif /* EN_TIME */  
#endif /* _TIMERS_H_ */
