/******************************************************************************
* TestDarray_hw.c -Program solution - student file

* Program to test the DynamicArrays module.  Reads in a large list of words
* and puts them into the data structure, then prints out the first and last 
* five elements in the data structure as well as the total number of words.
* R. Repka   9/12/2015 - Added checking
* R. Repka   9/27/2017 - Minor comment fix
* R. Repka  12/23/2017 - Safer fscanf() strncpy()
* R. Repka  02/16/2018 - Initialize Darray object
* R. Repka  10/06/2019 - Added get_opt_long features 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "DynamicArrays.h"

/* Initial Size of Dynamic Array */
#define INITIAL_SIZE (1000)



void ReadData(FILE *InputFile, DArray *DynamicArrayPtr);

/******************************************************************************
 This program to test the Data structure by reading the Data file provided on
 the command line into the structure, printing out the first and last N
 elements and total number of elements.  
        int main(int argc, char* argv[])
 Where:
    int argc        - The number of command line parameters
    char *argv[]    - An array of pointers to the parameters
    returns: in     - 0 for success, non-zero for error
    errors:         - Error message printed to stderr and exits with an error 
                      code 
******************************************************************************/
int main(int argc, char* argv[])
  {
  /* declare local variables */
   int ErrorCode = 0;               /* Application error code - 0 is OK */
   DArray TestDynamicArray = { 0, 0, NULL}; /* Dynamic Array for Data */
   FILE *DataFile = NULL;           /* Pointer to Data file name (from cmd line) */
   int lcv;                         /* Loop Control Variable */


   /*-------------------------------------------------------------------------
     These variables are used to control the getOpt_long_only command line 
     parsing utility.  
   --------------------------------------------------------------------------*/
   /* getopt_long stores the option index here. */
   int option_index = 0;
   int rc;
 
   /* Command line parameters */
   int words = 6;
   int help = 0;
   char *inFile = NULL;
char *getoptOptions = "i:w:h" ;
   
   /*-------------------------------------------------------------------------
     add get_opt_long parsing code here
   --------------------------------------------------------------------------*/
   
   

    struct option long_options[] = {
    {"input", required_argument, 0, 'i'},
    {"in",    required_argument, 0, 'i'},
    {"word",  required_argument, 0, 'w'},
    {"help",  no_argument,       0, 'h'},
    {0,       0,                 0,  0 }
};

while ((rc = getopt_long_only(argc, argv, getoptOptions , long_options, &option_index)) != -1) {
        switch (rc) {
            case 'i':
                inFile = optarg;  // Set the input file name
                break;
            case 'w':
                words = optarg ? atoi(optarg) : 6;   // 'words' defaults to 6 if no argument is provided
		break;
            case 'h':
                help = 1;
                break;
         /*    case '?':
           default:
                fprintf(stderr, "Usage: %s -i[n[put]] filename [-w|ord] num] [-h[elp]]\n", argv[0]);
                exit(EXIT_FAILURE);*/
        }
    }

	
   /* One command line argument is required: the file name     */
   if ((1 != help) && (NULL != inFile)) {
      /* Try to open the Data file for input (read mode)     */
      DataFile = fopen(inFile, "r"); 

      /* Verify that file was opened correctly */
      if (NULL != DataFile) {
         /* Initialize the dynamic array */
         CreateDArray(&TestDynamicArray, INITIAL_SIZE);

         /* Read all Data from text file */
         ReadData(DataFile, &TestDynamicArray);

         /* Close the Data file */
         fclose(DataFile);

         /* Print the first and last 5 entries */
         printf("First %d elements:\n", words);
         for (lcv=0; lcv<words; lcv++) {
            printf(" %s\n", TestDynamicArray.Payload[lcv].String);
         } /* for() */

         /* Print the last and first 5 entries */
         printf("Last %d elements:\n", words);
         for (lcv=TestDynamicArray.EntriesUsed-words; lcv<TestDynamicArray.EntriesUsed; lcv++) {
            printf(" %s\n", TestDynamicArray.Payload[lcv].String);
         } /* for() */

         /* Print array size */
         printf("Number of Words Read = %d\n", TestDynamicArray.EntriesUsed);

         /* De-allocate the dynamic array */
         DestroyDArray(&TestDynamicArray);
        } /* if() */
      else
        {
         /* Print error message */
         fprintf(stderr, "Error! File [%s] could not be opened.\n", inFile);
         ErrorCode = 2;
        } /* if...else() */
     } /* if() */
   else
     {
      /* Print usage information */
      fprintf(stderr, "This program reads a text file and prints out the first and last\n");
      fprintf(stderr, "N lines.\n");
      fprintf(stderr, "Usage: %s -i[n[put]] filename [-w|ord] num] [-h[elp]]\n", argv[0]);
      fprintf(stderr, " -i filename - the name of the file to read\n");
      fprintf(stderr, " -word num   - prints the number of words, default is 6\n");
      fprintf(stderr, " -help       - this help, no other action taken\n");
      ErrorCode = 1;
     } /* if...else() */

   return ErrorCode;
  } /* main() */
  
  
/******************************************************************************
 Read the data file (assumed to be open) one word at a time placing each
 word into the data structure.  This routine will verify that the input string
 has the correct length.  Any string that is too long will generate a warning
 to stderr, not be added but processing will continue
        void ReadData(FILE *InputFile, DArray *DynamicArrayPtr)
  Where:
    FILE *InputFile         - Pointer to an open input file
    DArray *DynamicArrayPtr - Pointer to a storage block which holds the
                              data structure entry 
    returns: void           - nothing is returned.
    errors:                 - This routine will print an error message to 
                              stderror and exit with an error code
******************************************************************************/
void ReadData(FILE *InputFile, DArray *DynamicArrayPtr)
  {
   Data TempData;             /* temp variable to hold data   */
   char String[MAX_STR_LEN+2];  /* temp variable to hold string */
   char formatStr [32];       /* Used to build the dynamic length */
   
   /* Build a dynamic format string */
   snprintf(formatStr, 32, "%c%d%c", '%', MAX_STR_LEN+1, 's');

   /* Read the data in from the file into the String buffer */
   while (EOF != fscanf(InputFile, formatStr, String))
     {
      /* Make sure the data is not too long */
      if (strlen(String) >= MAX_STR_LEN)
      { 
       fprintf(stderr, "Warning: ignoring invalid input\n");
       continue;
      }    
      
      strncpy(TempData.String, String, MAX_STR_LEN);
      PushToDArray(DynamicArrayPtr, &TempData);
     } /* while() */
  } /* ReadData() */

