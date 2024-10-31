/*---------------------------------------------------------------------------
  This is one of two parts of a program to print Fibonacci numbers.  It is
  used to illustrate make files.  
---------------------------------------------------------------------------*/
#include <stdio.h>

/* signatures of my functions */
void myPrint(void);
int myFib(int n);

/* main program*/
int main(int argc, char *argv[]) {
   int i;  
   myPrint(); // call myPrint() from the header file
  
   //Print Fibonacci numbers for [0-5]
   for (i = 0; i <= 5; i++) {
      printf("MyFib(%d):%d\n", i, myFib(i));
   }
  
   return(0);
}

//my simple print 
void myPrint(void) {
   printf("CMPE380 Makefile Example!\n");
   return;
}

//my simple recursive Fibonacci 
int myFib(int n) 
{ 
   if (n <= 1) { 
      return(n); 
   }
   return myFib(n-1) + myFib(n-2); 
} 