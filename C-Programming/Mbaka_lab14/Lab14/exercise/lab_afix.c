/*---------------------------------------------------------------------------
  student copy
  This program illustrates a simple buffer overflow attack
  04/24/2022 R. Repka	Print special characters
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PW_LEN   (7)
#define PW_ARRAY (3)

void printRam(char *start, char *end);

// Un-comment or re-comment as necessary to get the correct memory order 
#define SWAP_ORDER 

// Buffers in global memory are allocated in the order defined. 
#ifdef SWAP_ORDER
char kbBuff    [PW_LEN+1]  = "kbBuff";                                 // in lower memory
char passWords [PW_ARRAY] [PW_LEN+1] = { "one", "two", "three"};  
char otherBuff [PW_LEN+1]  = "Other";                                 // in higher memory 

#else
char otherBuff [PW_LEN+1]  = "Other";                                 // in higher memory 
char passWords [PW_ARRAY] [PW_LEN+1] = { "one", "two", "three"};  
char kbBuff    [PW_LEN+1]  = "kbBuff";                                // in lower memory  
#endif

int main(int argc, char * argv[]) {   
   int i;
   int pwValid =0;
   
 while(1) {
   // Print the current status of memory
    printRam(&kbBuff, &otherBuff);

   // Use gets() to read data from the keyboard into kbBuff
   // and check for a match in the password table.
   // print out the buffer memory block and then the final list
   // of current password.  Use <ctrl-c> to exit
   
   printf("Enter password: ");
       fgets(kbBuff, sizeof(kbBuff), stdin);

  // Ensure kbBuff is null-terminated and does not contain newline character
       kbBuff[strcspn(kbBuff, "\n")] = 0; // Safe use of strcspn to remove newline

   // Check if the entered password matches any stored passwords
   pwValid = 0;
  for (i = 0; i < PW_ARRAY; i++) {
       if (strcmp(kbBuff, passWords[i]) == 0) {
           pwValid = 1; // Set to 1 if password is valid
           break;
       }
  }

   // Print the result of the password check
   printf("Password is %s\n", (pwValid ? "VALID" : "INVALID"));

   // Print valid passwords
       printf("Valid passwords\n");
       for (i = 0; i < PW_ARRAY; i++) {
           printf("PW table [%d]: '%s'\n", i, passWords[i]);
       }
    } 
    return(0);

}
/**************************************************************************
 This function prints the ram address from start to end inclusive.
 
   char *start - Pointer to the start of the ram block
   char *end   - Pointer to the end of the ram block
   
   returns - nothing
   
   errors - none
**************************************************************************/
void printRam(char *start, char *end) {
   // Set the print range
   if (start > end) {
      printf("Error memory range out of order.  un-comment or re-comment \n");
      printf("#define SWAP_ORDER on line 15 as necessary to get the correct order\n"); 
      exit(99);
   }
   
   printf("Ram data %pX to %pX\n", (void *) start, (void *) end);
   while (start <= end) {
       printf("  %8p ", start);
       for (int i = 0; i < PW_LEN+1; i++) {
		// handle special characters
		if      (*start == 0x00) { printf("<nul>"); }
		else if (*start == '\r') { printf("<cr>"); }
		else if (*start == '\n') { printf("<lf>"); } 
		else    { printf("%c", *start); }
        start++;
       }
       printf("\n");
   }
   printf("\n");
   return;
} // printRam