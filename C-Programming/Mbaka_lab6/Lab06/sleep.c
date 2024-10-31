/*---------------------------------------------------------------------------
  sleep.c - student file
  01/01/2016    R. Repka    This code is only used to verify your timing macros
  02/202016     R. Repka    Added include file hint
  10/21/2018    R. Repka    Added time.h
---------------------------------------------------------------------------*/
#include <unistd.h>
#include <stdio.h>
#include <time.h>
/* add other include files as necessary */
// Define macros for timing
#define START_TIMER() clock_t start_t = clock();
#define STOP_TIMER() clock_t stop_t = clock();
#define PRINT_TIME(description) printf("%s: %f seconds\n", description, (double)(stop_t - start_t) / CLOCKS_PER_SEC);

int main()
   {
   clock_t end_t;
   int delay;

   printf("Start\n");
   /* Your macro stuff here */
     
   /* wait for 60 seconds */
   end_t = clock() + 60 * CLOCKS_PER_SEC;
   while (end_t > clock())
      {
      /* Consume CPU time */
      delay = 1<<19;
      while (delay) 
         {
         delay--;
         }
      }
   
   /* more of your macro stuff */   
   // Stop timing and print the elapsed time
   

   printf("End\n");
   return 0;
}
