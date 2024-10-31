/****************************************************************************
 Lab 2 student file
 10/05/2019 R. Repka    Initial release
****************************************************************************/
#include <stdio.h> 
#include <stdint.h> 
#include <string.h>
   
/*---------------------------------------------------------------------------
  Student data structure
---------------------------------------------------------------------------*/
struct bandMember {
   char f_name[256]; 
   char l_name[256]; 
   int  year; 
};	

typedef struct bandMember band; // Typedef the structure

union anumber1 {
   int i;
   float f;
} aNumber1;  //define union

struct anumber2 {
	int i;
	float f;
} aNumber2;  //define structure



int main(int argc, char *argv[]) {
   /*------------------------------------------------------------------------
     Add code here
   ------------------------------------------------------------------------*/
 //1.	A union of a float and integer 

  	aNumber1.i = 5; // Assign the integer value
        aNumber1.f = 6.0; // Assign the integer value
	
	printf("The value of float and integer is %f and  %d\n", aNumber1.f, aNumber1.i); 


 //2.	A structure of a float and integer

 	aNumber2.i = 5; // Assign the integer value
	aNumber2.f = 6.0; // Assign the integer value
	
	printf("The value of float and integer is %f and  %d\n", aNumber2.f, aNumber2.i);
	
	
 //3. Use the sizeof() operator on the data structures 

	printf("Size of the union 'anumber1': %zu bytes\n", sizeof(union anumber1));  // Print the size of the union
	
	printf("Size of the struct 'anumber2': %zu bytes\n", sizeof(struct anumber2)); // Print the size of the structure

	


 //4. Print-out of list of Beatle band members and birth years using a typedef and a for loop.
	band beatles[4]; // Array of 4 band members

	// Initialize the array with The Beatles members information
	// Using strncpy for safety
	
	// John Lennon
	strncpy(beatles[0].f_name, "John", 255);
	strncpy(beatles[0].l_name, "Lennon", sizeof(beatles[0].l_name) - 1);
	beatles[0].year = 1940;

	//Paul McCartney
	strncpy(beatles[1].f_name, "Paul", sizeof(beatles[1].f_name) - 1);
    	strncpy(beatles[1].l_name, "McCartney", sizeof(beatles[1].l_name) - 1);
    	beatles[1].year = 1942;

	// George Harrison
	strncpy(beatles[2].f_name, "George", sizeof(beatles[2].f_name) - 1);
	strncpy(beatles[2].l_name, "Harrison", sizeof(beatles[2].l_name) - 1);
	beatles[2].year = 1943;

	// Ringo Starr
    	strncpy(beatles[3].f_name, "Ringo", sizeof(beatles[3].f_name) - 1);
    	strncpy(beatles[3].l_name, "Starr", sizeof(beatles[3].l_name) - 1);
    	beatles[3].year = 1940;

	// Ensure strings are null-terminated
    	for (int i = 0; i < 4; i++) {
        	beatles[i].f_name[sizeof(beatles[i].f_name) - 1] = '\0';
        	beatles[i].l_name[sizeof(beatles[i].l_name) - 1] = '\0';
    	}
	

	// Use a for loop to print the data
    	for (int i = 0; i < 4; i++) {
        printf("Beatle %s %s born %d\n", beatles[i].f_name, beatles[i].l_name, beatles[i].year);
    }

    



 // 5. A run on string of Beatle members first names	

	
	// Declare a string to hold the run-on string of first names
    	char runOnNames[1024] = "";  // Initialize with an empty string

	// Use a for loop to concatenate first names
    	for (int i = 0; i < 4; i++) {
        strncat(runOnNames, beatles[i].f_name, 1024 - strlen(runOnNames) - 1);
    	}

	// Print the run-on string
    	printf("Run on string of first names: %s\n", runOnNames);

    	




 // 6. Code to return the index number for the “Ringo.

	// Initialize the array with The Beatles members' first names
    	
    
        int ringoIndex = -1; // Initialize with -1 to indicate "not found" by default

    	for (int i = 0; i < 4; i++) {
        if (strcmp(beatles[i].f_name, "Ringo") == 0) {
            ringoIndex = i; // Found Ringo, store the index
            break; // Exit the loop since we found what we were looking for
        	}
    }

    if (ringoIndex != -1) {
        // Ringo was found, print the index
        printf("The Ringo index is %d\n", ringoIndex);
    } else {
        // Ringo was not found in the array
        printf("Ringo not found in the array\n");
    }

 

 // 7.	Code to count from 1 to 255 inclusive

	uint8_t counter = 1; // Start counting from 1

    	while (counter != 0) { // Loop until overflow causes counter to wrap back to 0
        printf("%d\n", counter);
        counter++; // Increment counter, will overflow from 255 to 0
    	}

    



// 8. Code guaranteed to create a 64 bit long integer on ANY machine

	int64_t longInteger = 0; // Initialize a 64-bit integer

    	// Prove its size is 8 bytes
    	printf("Size of the 64-bit integer: %zu bytes\n", sizeof(longInteger));

    	return 0;







}
