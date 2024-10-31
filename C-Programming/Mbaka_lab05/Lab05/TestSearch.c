/******************************************************************************
 * Lydia Hays
 * lmh7513@rit.edu
 *
 * Program to time the search for the LinkedLists and DynamicArraymodule. 
 * Takes in the file name and the word that you are searching for. 
 *
 * Revised: Lydia Hays 3/6/2015
 *  9/12/2015 R. Repka     - Added checking
 * 12/24/2017 R. Repka     - use strncpy()
 * 10/08/2019 R. Repka     - removed darray Num parameter
 * *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "LinkedLists.h"
#include "DynamicArrays.h"

#define INITIAL_SIZE (1000)
#define MAX_BUFF_SIZE    (256)

/* Local functions */
void ReadDArrayData(FILE *InputFile, DArray *DynamicArrayPtr);
void ReadLinkedListData(FILE *InputFile, LinkedLists *ListPtr);


/******************************************************************************
*  Program to test the Data structure by reading the Data file provided on
* the command line into the structure, printing out the first and last five
* elements and total number of elements.

 Where: int argc     - the number of parameters on the command line
        char *argv[] - Pointer to the command line parameters
 Returns: 0 - success, 
          1 - can't open input file
          2 - Invalid number of parameters
******************************************************************************/
int main(int argc, char* argv[])
  {
  /* declare local variables */
   int ErrorCode = PGM_SUCCESS;     /* Application error code - 0 is OK */
   LinkedLists TestLinkedList;      /* Linked List for Data */
   DArray TestDynamicArray;         /* Dynamic Array for Data */
   Data *TempDArrayData = NULL;     /* A temporary data pointer */
   FILE *DataFile = NULL;           /* Pointer to Data file name (from cmd line) */ 

   /* One command line argument is required: the file name     */
   if (3 == argc) /* note that argc 2 means one argument given */
     {
      /* Try to open the Data file for input (read mode)     */
      DataFile = fopen(argv[1], "r"); /* argv[1] is a string */

      /* Verify that file was opened correctly */
      if (NULL != DataFile)
        {
         /* Initialize the Linked List */
         InitLinkedList(&TestLinkedList);
         
         /* Read all Data from text file */
         ReadLinkedListData(DataFile, &TestLinkedList);

         /* Initialize the dynamic array */
         CreateDArray(&TestDynamicArray, INITIAL_SIZE);
         rewind(DataFile);

         /* Read all Data from text file */
         ReadDArrayData(DataFile, &TestDynamicArray);

         /* Close the Data file */
         fclose(DataFile);
         
         /* Search in the Linked List structure */
         fprintf(stdout, "Searching for: %s \n", argv[2]);
         if (0 == SearchList(&TestLinkedList, argv[2]))
         {
            fprintf(stdout, "Could not find word in Linked List. \n");
         }
         else
         {
            fprintf(stdout, "Found %s in the Linked List \n",
                argv[2]);
         }
         
         /* Search in the Dynamic Array structure */
         TempDArrayData = SearchDArray(&TestDynamicArray, argv[2]);
         if(NULL == TempDArrayData)
         {
            fprintf(stdout, "Could not find word in DynamicArray. \n");
         }
         else
         {
            fprintf(stdout, "Found %s in the Dynamic Array \n",
                TempDArrayData->String);
         }
         
         /* De-allocate the dynamic array */
         DestroyLinkedList(&TestLinkedList);
         DestroyDArray(&TestDynamicArray);
        } /* if() */
      else
      {
           fprintf(stderr, "main(): Can not open file '%s'.\n", argv[1]);
        fflush(stderr);
        ErrorCode = PGM_FILE_NOT_FOUND;
      } /* if...else() */
     } /* if() */
   else
     {
       fprintf(stderr, "Tests linked list search operations\n");
       fprintf(stderr, "usage: TestSearch <filename> <word>\n");
       fflush(stderr);
       ErrorCode = PGM_SYNTAX_ERROR;
     } /* if...else() */

   return ErrorCode;
  } /* main() */

  
/******************************************************************************
*  Read the data file (assumed to be open) one word at a time placing each
* word into the Linked List data structure.

Where FILE *InputFile      - Open file handle 
      LinkedLists *ListPtr - Pointer to the linked list data structure to 
                             hold the data
Returns: nothing
Errors:  Prints to stderr and continues                          
******************************************************************************/
void ReadLinkedListData(FILE *InputFile, LinkedLists *ListPtr)
  {
   char String[MAX_BUFF_SIZE+2];/* temp variable to hold string */
   char formatStr [32];       /* Used to build the dynamic length */
   
   /* Build a dynamic format string */
   snprintf(formatStr, 32, "%c%d%c", '%', MAX_BUFF_SIZE+1, 's');
   

   while (EOF != fscanf(InputFile, formatStr, String))
     {  
      /* Make sure the data is not too long */
      if (strlen(String) >= MAX_STR_LEN)
      { 
       fprintf(stderr, "Warning: ignoring invalid input\n");
       continue;
      }         
      
      AddToBackOfLinkedList(ListPtr, String);
     } /* while() */
  } /* ReadData() */
  

/******************************************************************************
*  Read the data file (assumed to be open) one word at a time placing each
* word into the Dynamic Array data structure.

Where FILE *InputFile         - Open file handle 
      DArray *DynamicArrayPtr - Pointer to a dynamic array data structure to 
                                hold the data
Returns: nothing
Errors:  Prints to stderr and continues           
******************************************************************************/
void ReadDArrayData(FILE *InputFile, DArray *DynamicArrayPtr)
  {
   Data TempData;             /* temp variable to hold data   */
   char String[MAX_BUFF_SIZE+2]; /* temp variable to hold string */
   char formatStr [32];       /* Used to build the dynamic length */
   
   /* Build a dynamic format string */
   snprintf(formatStr, 32, "%c%d%c", '%', MAX_BUFF_SIZE+1, 's');
   
   while (EOF != fscanf(InputFile, formatStr, String))
     {
      /* Make sure the data is not too long */
      if (strlen(String) >= sizeof(TempData.String))
      { 
       fprintf(stderr, "Warning: ignoring invalid input\n");
       continue;
      }         
      
      strncpy(TempData.String, String, sizeof(TempData.String));
      PushToDArray(DynamicArrayPtr, &TempData);
     } /* while() */
  } /* ReadData() */
