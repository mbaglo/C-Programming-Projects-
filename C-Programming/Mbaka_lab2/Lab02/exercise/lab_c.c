/****************************************************************************
Lab 2 student file
 
 To enable gdb logging
   gdb <binary>
   set logging file <file>
   set logging overwrite on
   set logging on
   
         gcc -std=c99  -g -O0 lab_d.c -o lab_d
         
 10/05/2019 R. Repka    Initial release
****************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


int main(int argc, char *argv[]) {
   char *text_p;        // do not change
      
   /*------------------------------------------------------------------------
     Add code here
   ------------------------------------------------------------------------*/
 
 // 1.	Malloc 12 bytes of memory for “text_p” with proper return code checking 

	// Allocate 12 bytes of memory for 'text_p'
    	text_p = (char *)malloc(12 * sizeof(char));

    	// Check if memory allocation was successful
   	 if (text_p == NULL) {
        // If malloc returns NULL, allocation failed
        printf("Memory allocation failed\n");
        return 1; // Return with an error code
   	 }

 // 2.	Use strncpy to copy the string

    	// n is chosen as 11 to fit the allocated memory 
    	// and we want to manually ensure the string is null-terminated
    	strncpy(text_p, "CMPE380 lab malloc try 1.", 11);

    	// Manually null-terminate the string, to be safe
    	text_p[11] = '\0';

// 3. 	Print out the resulting string with single quotes around the %s string variable 

	printf("'%s'\n", text_p);

// 4. 	Use realloc() to resize "text_p" to 20 bytes with proper error checking

    	char *temp_p = realloc(text_p, 20 * sizeof(char));
    	if (temp_p == NULL) {
        printf("Memory reallocation failed\n");
        free(text_p); // Free the original block to avoid a memory leak
        return 1; // Return with an error code
    	}
    	text_p = temp_p; // Update text_p to point to the new memory block

// 5. Use strncat() to append "-- Realloc" to "text_p"
    
	// Calculate 'n' as the remaining space in the buffer minus 1 for the null terminator
    	size_t remainingSpace = 20 - strlen(text_p) - 1;
    	strncat(text_p, "-- Realloc", remainingSpace);

// 6.	Print the modified string
    
	printf("After appending: '%s'\n", text_p);

// 7. 	Free dynamically allocated memory when done to avoid memory leaks
    	
	free(text_p);



// 8. 	Create a string variable containing the string “ 0005.6000 ”

	char aString[] = " 0005.6000 ";


// 9 	 Convert the string to corresponding integer and floating-point values and print them out with a friendly message.


	// Convert to integer value using atoi
    	int intValue = atoi(aString);

	// Convert to floating-point value using atof
    	float floatValue = atof(aString);

	// Print the results with friendly messages
    	printf("The integer value of the string is: %d\n", intValue);
    	printf("The floating-point value of the string is: %f\n", floatValue);

    	

	return 0; // Return success




}