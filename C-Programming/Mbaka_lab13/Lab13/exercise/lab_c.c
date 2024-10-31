/*---------------------------------------------------------------------------
  Create a single thread and use pthread_exit()  
  student copy
  
  gcc -g -std=c99 lab14a.c -lpthread -o lab14a
---------------------------------------------------------------------------*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> //For sleep().  
#include <pthread.h> //For threads
#include <time.h>

void *mySimpleThread(void *name);

// Global variables
int taskRC[3];

int main(int argc, char *argv[]) { 
   // Thread data structure
   pthread_t thread_id[3];  // Array of thread IDs
   
   // Pointer to the process return code, used later
   //void *rcp;
   
   // Process index id number 
   int num[3] = {1, 2, 3};  // Array of thread identifiers
   
   int rc;

    // Create multiple threads
    for (int i = 0; i < 3; i++) {
        printf("Main: Starting thread %d...\n", i + 1);
        rc = pthread_create(&thread_id[i], NULL, mySimpleThread, &num[i]);
        if (rc) {
            printf("Thread %d failed to start rc= %d\n", i + 1, rc);
            exit(99);
        }
    }
    printf("Main: All threads started...\n"); 

   // Student should add wait for the thread to finish
   // when instructed. Always check the join RC as you
   // may get memory leaks otherwise.
   
   // Wait for all threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(thread_id[i], NULL);
   // Print the return code from the thread
   printf("Thread %d finished with return code: %d\n", i + 1, taskRC[i]);
    }
   
   // Print status in the main routine 
   for (int i = 0; i < 2; i++){
      printf("Main will sleep 1 second. \n"); 
      sleep(1); 
   }
   
   printf("Exiting main thread\n");
    pthread_exit(NULL); // Correct use of pthread_exit
}

/*---------------------------------------------------------------------------
  A thread that prints out it's ID number and sleeps in a loop,
  printing status
---------------------------------------------------------------------------*/
void *mySimpleThread(void *num) { 
    int index = *(int *)num - 1; // Adjust index for zero-based array indexing
    for (int i = 0; i < 6; i++) {
        printf("     Thread %d, will sleep 1 second. \n", *(int *)num); 
        sleep(1); 
    }
    taskRC[index] = *(int *)num; // Set task return code at the index to the thread number
    return NULL;

} 