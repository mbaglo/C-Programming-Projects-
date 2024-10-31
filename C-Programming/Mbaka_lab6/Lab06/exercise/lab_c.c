/*-----------------------------------------------------------------------
  Student copy 
  Demonstrates a more complex combination of # string conversion commands 
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#define MACRO_NUMBER   512
#define STR(S) #S
#define XSTR(S) STR(S)



// Define your macros here

int main() {
	char sprintfBuffer1[50];
   	char *stringTest = {"C will " "combine these" " string\n"};

   	// Print stringTest to the standard output
      	printf("%s\n", stringTest);
   
   	// Use snprintf to format the string and store it in sprintfBuffer1
   	snprintf(sprintfBuffer1, 50, "snprintf buffer size is (%d) bytes\n", MACRO_NUMBER);

   	// Print the content of sprintfBuffer1 to the console
   	printf("%s\n", sprintfBuffer1);

   	// char *simpleMacroBuffer2 = 
     	char *simpleMacroBuffer2 = "Simple buffer size is (" STR(256) ") bytes";
	 
	// Print simpleMacroBuffer2 to the standard output
  	printf("%s\n", simpleMacroBuffer2);



	//   char *complexMacroBuffer3 =
   	char *complexMacroBuffer3 = "Complex buffer size is (" XSTR(MACRO_NUMBER) ") bytes";

  	// Print complexMacroBuffer3 to the standard output
   	printf("%s\n", complexMacroBuffer3);


   	return(0);
}