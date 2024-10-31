/****************************************************************************
 The purpose of this example is to exercise using data and function pointers.
 It also demonstrates the value of using NULL assignments to identify pointer
 issues.
 
 student version 
 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_b.c -o lab_b
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99 ./lab_b
 
 
 09/09/2019 R. Repka    Minor formatting changes
 02/09/2023 R. Repka    Add extra value
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
   int entry;
   int value;
   char string [40];
} Data;

int fun2x(int num);
int fun3x(int num);

Data ramData;


int main(int argc, char *argv[]) {
   int(*function_p)(int);
   int answer;
   Data stackData;
   static Data staticData;
   Data *heapData_p;
 
   //Allocate space for the heap data pointer 
	heapData_p = (Data *)malloc(sizeof(Data));
	if (heapData_p == NULL) {
    	fprintf(stderr, "Malloc failed\n");
    	exit(1);
}

   // Print out the addresses of items
	printf("Address of function main: %8p\n", (void *)main);
	printf("Address of static data: %8p\n", (void *)&staticData);
	printf("Address of RAM data: %8p\n", (void *)&ramData);
	printf("Address of heap data: %8p\n", (void *)heapData_p);
	printf("Address of stack data: %8p\n", (void *)&stackData);


           
   // Using function pointers
	function_p = fun2x;
	answer = function_p(2);
	printf("Passing the 2X function a 2 I got %d\n", answer);

	function_p = fun3x;
	answer = function_p(2);
	printf("Passing the 3X function a 2 I got %d\n", answer);


    
    // Initialize the stack data
       // Initialize the stack data variable 'stackData' with safer string copy
	stackData.entry = 1;
	stackData.value = 2;
	strncpy(stackData.string, "Stack", 39); // 39 ensures there's room for the null terminator
	stackData.string[39] = '\0'; // Explicitly null-terminate the string    //Initialize the heap data, free it and use it
 	// Print out the values of 'stackData'
	printf("Stack data values: %d %d '%s'\n", stackData.entry, stackData.value, stackData.string);

	// Initialize the heapData_p 
	heapData_p->entry = 3;
	heapData_p->value = 4;
	strncpy(heapData_p->string, "Pointer", 39);
	heapData_p->string[39] = '\0'; // Ensure null-termination

	// Print out the values of 'heapData_p'
	printf("Heap data values: %d %d '%s'\n", heapData_p->entry, heapData_p->value, heapData_p->string);

	free(heapData_p);
	
	heapData_p = NULL;
	
	// Re-printing Data After Free
	printf("Heap data values after free: %d %d '%s'\n", heapData_p->entry, heapData_p->value, heapData_p->string);


	




   return 0;
}


int fun2x(int num) {
   return(num*2);
}
int fun3x(int num) {
   return(num*3);
}