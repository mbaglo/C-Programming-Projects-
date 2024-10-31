//  This fills ram with +3 sequential integers
//  student file
//
//   gcc -g -O0 -std=c99 hw13.c -lpthread -o hw13 -Wall -pedantic
//  valgrind --tool=memcheck --leak-check=yes --error-exitcode=99 ./hw13 -f -s
// 05/10/2021  R. Repka    Minor changes
                                                            

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>
#define EN_TIME
#include "Timers.h"
#include "ClassErrors.h"

/*--------------------------------------------------------------------------
  Local data structures and defines 
--------------------------------------------------------------------------*/
// Use the larger data size of the Linux cluster, smaller for
// a typical PC. 
// guarantees divisible by 2/3/4/5/7/8  Note: this number is VERY large
// and MAY cause overflow issues so order of operations will be 
// important.  aka    a*b/c  needs to be written as a*(b/c)  because
// it results in a smaller intermediate value 
#define DATA_SIZE           (136*3*5*7*146*512)
#define DEBUG_DATA_SIZE     (136*3*5*7*146)
#define VALGRIND_DATA_SIZE  (30*3*5*7*8*1024)


// Maximum number of threads 
#define MAX_THREADS     (8)

// The number of iterations to slow down thread execution
#define DELAY_LOOPS_EXP        (5)     

// The percentage rate to update thread progress
#define STATUS_UPDATE_RATE (10)

// Thread information control structure 
struct ThreadData_s {
   int threadIndex;    // Contains the thread index number 0..n
    /*  Other data you might need, like the size of a segment */
    int segSize;
   unsigned *dataPtr; // Pointer to the one contagious array buffer that the thread will work on
   int trackStatus;   // Flag to identify if status updates should be reported
   int verbose;       // Flag to indicate if the task should run in verbose mode
}; 

// Controls the amount of time the mutex check thread will run 
#define MUTEX_CHECK_COUNT (2)
  
/* Function prototypes */
void *do_process(void *data);
void *mutex_check(void *data);
void print_help(void);

// The progress count, you must use this 
pthread_mutex_t lock;
volatile int processed = 0;   // Threads might impact this variable
int taskID [MAX_THREADS]; // Holds the process return code
volatile int progress = 0;

int main(int argc, char *argv[]) {
   /*------------------------------------------------------------------------
     General purpose variables 
   ------------------------------------------------------------------------*/
   DECLARE_TIMER(timer); 
 
   time_t wallTime;    // Used to report wall execution time.
 
   /*------------------------------------------------------------------------
     Thread process information
   ------------------------------------------------------------------------*/
  pthread_t threadProcess[MAX_THREADS];
   pthread_t mutexCheckProcess;
   struct ThreadData_s* threadData;
   int segSize;
   int rc;
   void *rcp = NULL;
   int dataSize = DATA_SIZE; // Default to full data size
   unsigned *inData;
   int i = 0;
    if (pthread_mutex_init (&lock, NULL) != 0){
    printf("Mutex initialization failed.n");
    exit(99);
    }
   /*------------------------------------------------------------------------
      UI variables with sentential values
   ------------------------------------------------------------------------*/
    int numThreads = 1; 
    int verbose = 0; 
    int trackStatus = 0; 
    int mutexCheck = 0; 
   /*------------------------------------------------------------------------
     These variables are used to control the getopt_long_only command line 
     parsing utility.  
   ------------------------------------------------------------------------*/
    char *getoptOptions = "t:svmfh";
    int option_index = 0;
    struct option longOpts[] = {
       {"threads"   , required_argument , 0, 't'},
       {"status"    , no_argument       , 0, 's'},
       {"verbose"   , no_argument       , 0, 'v'},
       {"fast"      , no_argument       , 0, 'f'},
       {"mutexcheck", no_argument       , 0, 'm'},
       {"help"      , no_argument       , 0, 'h'},
       {0, 0, 0, 0}
    };
   
   
    while ((rc = getopt_long(argc, argv, getoptOptions, longOpts,  &option_index)) != -1) {
       switch (rc) {
           case 't':
               numThreads = atoi(optarg);
               break;
           case 's':
               trackStatus = 1;
               break;
           case 'v':
               verbose = 1;
               break;
           case 'f':
              dataSize = VALGRIND_DATA_SIZE;
               break;
           case 'm':
               mutexCheck = 1;
               break;
           case 'h':
             print_help();
             exit(0);
           case '?':
               printf ("Internal error: undefined option %0xX\n", rc);
              exit(PGM_INTERNAL_ERROR);
       } /*end switch*/
    } /*end while loop*/

   /*------------------------------------------------------------------------
     Check for command line syntax errors
   ------------------------------------------------------------------------*/
    if ((optind < argc) || (numThreads < 1 || numThreads > MAX_THREADS)){
       fprintf(stderr, "This program demonstrates threading performance.\n");
      fprintf(stderr, "usage: hw13 -t[hreads] num [-s[tatus]] [-f[ast]] [-m[utex[check]]] [-v[erbose]] \n");
      fprintf(stderr, "Where: -t[hreads] num  - number of threads 1 to %d,required\n", MAX_THREADS);
      fprintf(stderr, "       -s[tatus]       - display thread progress, optional\n"); 
      fprintf(stderr, "       -v[erbose]      - verbose flag, optional\n");
      fprintf(stderr, "       -f[ast]         - shorter run for Valgrind, optional\n");
      fprintf(stderr, "       -m[utex[check]] - add a progress variable mutex text verification step\n");
      fprintf(stderr, "       -h[elp]         - this help message\n");
      
      fprintf(stderr, "eg: hw13 -t 3 -status\n");
      fflush(stderr);
      return(PGM_SYNTAX_ERROR);
    } /* End if error */
    

   /* Get space for the data */
    segSize = dataSize/numThreads;
    threadData = (struct ThreadData_s*) malloc(numThreads * sizeof(struct ThreadData_s));
    if(threadData == NULL){
	fprintf(stderr,"Malloc for threadData failed!\n");
	exit(99);
    }
  
    inData = (unsigned*)calloc(dataSize, sizeof(unsigned));
    if(inData == NULL){
	fprintf(stderr,"Calloc for inData failed!\n");
	exit(-100);
    }
   
   // Print message before starting the timer
    printf("\nStarting %d threads generating %d numbers\n\n", numThreads, dataSize);   
 
    START_TIMER(timer);
    wallTime = time(NULL);

  // Spin up the mutex check thread if required, use your mutex check UI flag here
    if (mutexCheck) {
      rc = pthread_create(&mutexCheckProcess, NULL, mutex_check, NULL);
      if (rc) {
         printf("Thread failed to start rc= %d\n", rc);
         exit(PGM_INTERNAL_ERROR);
         } // End if rc
      if (verbose) {
         fprintf(stdout, "Mutex check thread index:%ld\n", (unsigned long int)mutexCheckProcess);
         fflush(stdout);
         }
    } // End mutexCheck
   
   
   // Spin up N threads
 
    for(i = 0; i < numThreads; i++) {
         // Build the thread specific information
    threadData[i].threadIndex = i;
    threadData[i].trackStatus = trackStatus;
    threadData[i].verbose = verbose;
    threadData[i].dataPtr = inData;
    threadData[i].segSize = segSize;
      
    // Start the thread
  
    rc = pthread_create(&threadProcess[i], NULL, do_process, &threadData[i]);
    if (rc)  {
    fprintf(stderr, "Thread %d failed to start, rc = %d\n", i, rc);
    }
    
    if (verbose) {
         fprintf(stdout, "Thread index:%d  ID:%ld started\n", i, (unsigned long int)threadProcess[i]);}
    } // End threads  
 
    /* Print out the progress trackstatus */
    if(trackStatus){
	pthread_mutex_lock(&lock);
	while(progress != (numThreads*STATUS_UPDATE_RATE)){
	pthread_mutex_unlock(&lock);
	sleep(1);
	pthread_mutex_lock(&lock);
	fprintf(stdout,"Processed: %d lines %d%% completed\n",
		(dataSize/(numThreads*STATUS_UPDATE_RATE)) * progress,
		(progress*100)/(numThreads * STATUS_UPDATE_RATE));
		fflush(stdout);
	}
	pthread_mutex_unlock(&lock);
    }

    /* Wait for all processes to end */
    for (int i = 0; i < numThreads; i++) {
    void* rcp = NULL; // Define a temporary pointer to hold the return value from each thread
    rc = pthread_join(threadProcess[i], &rcp);
    if (rc) {
       fprintf(stderr, "Thread %d failed to join, rc = %d\n", i, rc);
        
    } 
        if (verbose) {
            fprintf(stdout, "Thread index %d, join RC %d, task rc %ld\n", i, rc, (long)rcp);
            fflush(stdout);
        }
    
    } // End threads  

   
    STOP_TIMER(timer);
    wallTime = difftime(time(NULL), wallTime);
    PRINT_TIMER(timer);

    fprintf(stderr, "Total wall time = %d sec\n", (int)wallTime);
 

    printf("Verifying results...  ");
    for (unsigned i = 0; i < dataSize; i++) {
       if (inData[i] != 4*i) {
         printf("Error inData[%d]= %d != %d\n", i, inData[i], 4*i); 
          exit(PGM_INTERNAL_ERROR);}
    } // End verification
    printf("success\n\n");

   
   // Wait for the mutex check thread, use your mutex check UI flag here
   if (mutexCheck) {
      rc = pthread_join(mutexCheckProcess, &rcp);
       if (verbose) {
          fprintf(stdout, "Mutex check thread terminated\n");
          fflush(stdout);
       }
   } // End mutexCheck
   
   // Clean up
   free(inData);
   free(threadData);
   pthread_mutex_destroy(&lock);
   threadData = NULL;
   inData = NULL;
   
   return 0;
   } // End main

   
/****************************************************************************
  This process is used to test mutex locking.  It locks the mutex, then 
  puts a temporary value in the shared data.  If any other thread, including
  main, tries to access the shared data improperly then you should see the 
  bad data.  You may not change anything in this function.
  
  This thread assumes the global process variable is "processed"
  
  void *mutex_check(void *data)
  
  Where: void *data - not used 
  Returns: void *   - returns null
  
  Errors: none
 
****************************************************************************/
void *mutex_check(void *data) {
   volatile int processedCopy = 0;
   int count = MUTEX_CHECK_COUNT;
   
   // Print out the thread status
   fprintf(stdout, "*** mutex_check thread started\n");
   fflush(stdout);
   
   // Process all the data in the portion of the array
   while (count--) {
      pthread_mutex_lock(&lock);
      
      // Keep processing until nothing changes 
      if (processedCopy != processed) { 
         count = MUTEX_CHECK_COUNT;
      }
      
      // Save the good data, put in bad data
      processedCopy = processed;
      processed = -999999;
      
      // let other threads do stuff
      sleep(1);
      
      // Return the good data
      processed = processedCopy;         
      pthread_mutex_unlock(&lock);
      sleep(1);
   } // End while
   
   
   // Print out the thread status
   fprintf(stdout, "*** Mutex_check thread ended\n");
   fflush(stdout);
   
   pthread_exit(NULL);
} // End mutex_check
   
/****************************************************************************
  This threading process will initialize parts of a very large array by 3's
  It contains code to SLOW execution down so that status updates can be easily
  seen.  The function prototype is defined by pthread so we MUST use it.
  
  void *do_process(void *data)
  
  Where: void *data - pointer to some user defined data structure
                      We will use struct ThreadData_s
  Returns: void *   - pointer to some user defined return code structure
                      We will use an integer array
  Errors: none
 
****************************************************************************/
void *do_process(void *data) {
     struct ThreadData_s *td = (struct ThreadData_s *) data;
   int verbose = td->verbose;
   int trackStatus = td->trackStatus;
   int threadIndex = td->threadIndex;
   int segSize = td->segSize;
   unsigned *dataPtr = td->dataPtr;
   int start = segSize  * threadIndex;
   int end = start + segSize;
   float percent = 10.0;
    
    // Print out the thread status
    if (verbose) {
        fprintf(stdout, "Thread index:%d track status:%d seg size:%dK data ptr:%p\n",
        threadIndex, trackStatus, segSize, (void*) dataPtr);
        fflush(stdout);
    } // End verbose
   
    // Process all the data in the portion of the array
    for (int i = start; i < end; i++) {
    // Initialize the data to simulate 4*i;
        dataPtr[i] = 4 * i; 
    
        
        // Slow the CPU
    int delay = 1 << DELAY_LOOPS_EXP;
    while (delay) {
        delay--;
    }
        
        // Track status if required
        
     if ( trackStatus && percent < 100.0 && (((100.0 * (i-start))/segSize))>= percent) {
            pthread_mutex_lock(&lock);
            progress++;
            pthread_mutex_unlock(&lock);
            percent+=10;
            // Print out the thread status if tracking status
            // Only print verbose when updating progress.
             if (verbose) {
            fprintf(stdout, "%d ", threadIndex);
            fflush(stdout);
            } // End verbose
         }  // End if
      } // End i

    // There might be some status left to update
   if (trackStatus) {
	pthread_mutex_lock(&lock);
	progress++;
	pthread_mutex_unlock(&lock);
   }

   // Return the task ID number + 10
   taskID[threadIndex] = threadIndex +10;
   
   return(&taskID[threadIndex]);
} // End do_process


// Use this function both in the error handling section and for the `-h` case
void print_help() {
    fprintf(stderr, "Usage: hw13 -t[hreads] num [-s[tatus]] [-f[ast]] [-m[utex[check]]] [-v[erbose]]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -t, --threads=num    Specify the number of threads (1 to %d), required\n", MAX_THREADS);
    fprintf(stderr, "  -s, --status         Display thread progress, optional\n");
    fprintf(stderr, "  -v, --verbose        Enable verbose output, optional\n");
    fprintf(stderr, "  -f, --fast           Use a smaller data size for faster runs, optional\n");
    fprintf(stderr, "  -m, --mutexcheck     Enable mutex check, optional\n");
    fprintf(stderr, "  -h, --help           Display this help and exit\n");
    fprintf(stderr, "\nExamples:\n");
    fprintf(stderr, "  hw13 -t 3 -s -v       # Runs with 3 threads, status enabled, in verbose mode\n");
    fprintf(stderr, "  hw13 -t 2 -f          # Runs with 2 threads and uses smaller data size for fast execution\n");
    fprintf(stderr, "  hw13 -h               # Displays this help information\n");
}
