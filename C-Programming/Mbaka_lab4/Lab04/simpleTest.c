/******************************************************************************
 * Program to test the LinkedLists module.
 *
 * 08/10/2019 R. Repka     - initial version
 * *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "LinkedLists.h"

#define MAX_BUFF_SIZE    (256)


/******************************************************************************
 Program to test the Data structure by reading the Data file provided on
 the command line into the structure, printing out the first and last five
 elements and total number of elements.

 Where: int argc     - the number of parameters on the command line
        char *argv[] - Pointer to the command line parameters
 Returns: 0 - success, 
          2 - can't open input file
          1 - Invalid number of parameters
******************************************************************************/
int main(int argc, char* argv[]) {
  /* declare local variables */
   int ErrorCode = PGM_SUCCESS;     /* Application error code - 0 is OK */
   LinkedLists TestLinkedList;      /* Linked List for Data */
    
   LinkedListNodes *TempNode = NULL;/* A temporary list node pointer */
   char *TempData = NULL; /* A temporary data pointer */
   int lcv = 0;                     /*  loop control variable */
   int testNum = -1;

   if (2 == argc) {
      /* There must be 1 parameter */
      testNum = atoi(argv[1]);
   }
      
   /* Only run valid tests */
   if ((testNum >= 0) && (testNum <=4)) {
      /* Initialize the linked list */
      InitLinkedList(&TestLinkedList); 
      
      if (testNum > 0) {
         AddToBackOfLinkedList(&TestLinkedList, "First Line");
      }
      if (testNum > 1) {
         AddToBackOfLinkedList(&TestLinkedList, "Second Line"); 
      }
      if (testNum > 2) {
         AddToBackOfLinkedList(&TestLinkedList, "Third Line");
      }
      if (testNum > 3) {
         AddToBackOfLinkedList(&TestLinkedList, "Fourth Line");
      }
      
      /* Print the front entries */
      printf("The list has %d elements\n", TestLinkedList.NumElements);
      printf("From the front:\n");
      TempNode = TestLinkedList.FrontPtr;
      for(lcv = 0; (TempNode != NULL); lcv++) {
          printf("Node %d has value '%s'\n", lcv, TempNode->String);
         TempNode = TempNode->Next;
      }
      
      /* Print the back entries */
      printf("\nFrom the back:\n");
      TempNode = TestLinkedList.BackPtr;
      for(lcv = 0; (TempNode != NULL) && (TempNode->Previous != NULL); lcv++) {
         printf("Node %d has value '%s'\n", lcv, TempNode->String);
         TempNode = TempNode->Previous;
      }
      if (TempNode != NULL) {
         printf("Node %d has value '%s'\n", lcv, TempNode->String);    
      }
      
      /* Remove from front of list, print a message */
      printf("\nRemoving one element from the front\n");
      TempData = RemoveFromFrontOfLinkedList(&TestLinkedList);
      if(NULL == TempData) {
          printf("Attempted to remove front of list, list was empty. \n");
      }
      else {
         /* There was SOME data, it could the last entry or more than one */
         if (TestLinkedList.FrontPtr) {
          printf("\nRemove from front of list, new front is: '%s' \n",
                  TestLinkedList.FrontPtr->String);
         }
         else {
            printf("Nothing left in the list, no front\n");
         }
          printf("Removed value is: %s \n", TempData);
         FREE_DEBUG(TempData);
      }
      
      /* De-allocate the linked list */
      printf("\nDestroying the linked list\n"); 
      DestroyLinkedList(&TestLinkedList);
   }/* if() */
   
   else {
      printf("Tests linked list operations, adding 1 to 4 objects\n");
      printf("to the list and then deletes them\n");
      printf("usage: simpleTest num\n");
      printf("  num - test to run from 0-4\n");
      ErrorCode = PGM_SYNTAX_ERROR;
     } /* if...else() */

   return ErrorCode;
  } /* main() */