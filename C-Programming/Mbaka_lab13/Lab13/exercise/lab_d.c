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

// Global variables and mutex
int taskRC[3];
int processed = 0;
pthread_mutex_t lock;

int main(int argc, char *argv[]) { 
   // Thread data structure
   pthread_t thread_id[3];  // Array of thread IDs
   
   // Pointer to the process return code, used later
   //void *rcp;

   
   // Process index id number 
   int num[3] = {1, 2, 3};  // Array of thread identifiers
   
   int rc;

     // Initialize the mutex
    pthread_mutex_init(&lock, NULL);
   
    // Start timing
    time_t start_time = time(NULL);

    // Create multiple threads
    for (int i = 0; i < 3; i++) {
        printf("Main: Starting thread %d...\n", i + 1);
        rc = pthread_create(&thread_id[i], NULL, mySimpleThread, &num[i]);
        if (rc) {
            printf("Thread %d failed to start rc= %d\n", i + 1, rc);
            exit(99);
        }
    }

    // Monitor progress
    while (processed < 18) {
        pthread_mutex_lock(&lock);
        printf("Processed: %d tasks\n", processed);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }

   // Student should add wait for the thread to finish
   // when instructed. Always check the join RC as you
   // may get memory leaks otherwise.
   
   // Wait for all threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(thread_id[i], NULL);
   // Print the return code from the thread
   printf("Thread %d finished with return code: %d\n", i + 1, taskRC[i]);
    }
   
   // Calculate and display wall time
    time_t end_time = time(NULL);
    printf("Total wall time: %ld seconds\n", (long)(end_time - start_time));

    printf("Exiting main thread\n");
   pthread_mutex_destroy(&lock);
   pthread_exit(NULL);
}

/*---------------------------------------------------------------------------
  A thread that prints out it's ID number and sleeps in a loop,
  printing status
---------------------------------------------------------------------------*/
void *mySimpleThread(void *num) {
    int index = *(int *)num - 1;
    for (int i = 0; i < 6; i++) {
        sleep(1);
        pthread_mutex_lock(&lock);
        processed++;
        pthread_mutex_unlock(&lock);
    }
    taskRC[index] = index + 1; // Directly set taskRC at the index to a unique number
    return NULL;
}