/*---------------------------------------------------------------------------
  student copy
  This demonstrates taking control of a return address using a buffer 
  overflow attack
  gcc -g lab_b.c  -o lab_b-yes  -O0
  gcc -g -fno-stack-protector  lab_b.c  -o lab_b-no -O0
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BSIZE   (0x10)

// Local prototypes
void goodFun();
void hackedFun();
void printStack(char *stackPtr, long int *j);

// The data to smash the stack, adjust the size as necessary
// Add more hackedFun initializers to increase the size of the array
// C will auto allocate the array size
void *hackedData[] = {(void *)&hackedFun, (void *)&hackedFun, (void *)&hackedFun, 
                      (void *)&hackedFun, (void *)&hackedFun, (void *)&hackedFun};

 

int main(int argc, char * argv[]) {
   // Create a stack variable of size BSIZE and initialize to 0x0a.
   char mainBuff[BSIZE];  // Main buffer for demonstration
    for (int i = 0; i < BSIZE; i++) {
        mainBuff[i] = 0x0a;  // Initialize buffer with 0x0A to visualize it in the stack
    }
 
   printf("This program demonstrates a buffer overflow attack to take control of\n");
   printf("the return address to force another function to execute.\n\n");
   printf("Pointer size %ld  Int size %ld\n", sizeof(int *), sizeof(int));
   printf("main(): %8p,  goodFun(): %8p, hackedFun(): %8p\n", 
                                       &main, &goodFun, &hackedFun);
   
   // Call the good function
   printf("Calling goodFun()\n");
   goodFun();
   printf("Returned from goodFun()\n");  // Notice returned message
   
   return(0);
}


/*---------------------------------------------------------------------------
   This function has an unprotected buffer in it 
   
   char *buff - pointer to a buffer from main to bracket the stack
   
   returns: nothing
   
   errors: none
---------------------------------------------------------------------------*/
void goodFun() {
   // Create a stack variable of size BSIZE and initialize to 0x0b.
   // and a long int initialized to 0x12345678
   char subBuff[BSIZE];  // Subroutine buffer to be overflowed
    long int j = 0x12345678;  // An arbitrary stack variable to visualize the stack end

    for (int i = 0; i < BSIZE; i++) {
        subBuff[i] = 0x0b;  // Initialize buffer with 0x0B for visualization
    }

   
   // Print out important stack features
   printf("\nInitial stack state before overflow:\n");
   printStack((char *)subBuff, &j);
    
   //slam data in and redisplay the stack when directed
   printf("\nSimulating a local buffer overflow attack\n");
   
   // The first parameter in memcpy should be your stack variable
   // WITHOUT any indexing or pointer adjustment
   memcpy(subBuff, hackedData, sizeof(hackedData));  // Perform buffer overflow here

   printf("\nStack state after overflow:\n");
   printStack((char *)subBuff, &j);

   return;
}


/**************************************************************************
 Our hacked function that is never 'called' but hacked into the 
 return register 
**************************************************************************/
void hackedFun() {
   printf("\n\n------ HACKED!  In hackedFun()------\n\n");
   exit(0);
}


/**************************************************************************
 This function prints out stack parameters
 
      char *bytePtr   - Pointer to the start of the stack frame
      long int *j     - Pointer to the local index variable

returns: nothing

errors: none
 
**************************************************************************/
void printStack(char *bytePtr, long int *j) {
   long int *stackPtr = (long int *)bytePtr;
   // Display the stack status
   printf("The stack data in hex \nHigh memory\n");
   int count = 13;
   stackPtr+=9;
   
   while (count--) {
       printf("%8p  %016lxX ", stackPtr, *stackPtr);
       
       if (*stackPtr == 0x0b0b0b0b0b0b0b0b) {
          printf("subroutine buffer");
       }
       else if (*stackPtr == 0x0a0a0a0a0a0a0a0a) {
          printf("main buffer");
       }
       else if ((void *)stackPtr == j) {
          printf("loop counter 'j'");
       }
        else if ((((long)*stackPtr) ) == 
                    (((long)&hackedFun))) {
          printf("hacked return address");
       }
       else if ((((long)(*stackPtr)) & 0xfffffffff000) == 
                           (((long)&main) & 0xfffffffff000)) {
          printf("return address");
       }
       else if (((unsigned long) *stackPtr) >> 56) {
          // GCC puts in large random numbers to detect stack issues
          printf("stack smashing detector");
       }
       printf("\n");
       
       // next stack frame
       stackPtr--;
   }
   printf("Low memory\n");
   return;
}