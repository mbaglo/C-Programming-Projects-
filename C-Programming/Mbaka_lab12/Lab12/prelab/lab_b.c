/*---------------------------------------------------------------------------
  This prints out argv and env variables 
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {
   //your code here 
 
 // Print command line arguments
    printf("Command-line arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    // Print environment variables
    printf("Environment variables:\n");
    for (int i = 0; env[i] != NULL; i++) {
        printf("env[%d]: %s\n", i, env[i]);
    }

    return 0;
}
