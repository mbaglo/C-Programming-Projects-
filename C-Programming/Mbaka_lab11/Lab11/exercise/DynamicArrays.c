/**
 ** DynamicArrays.c  - private implementations - student file
 **
 ** Implementation of a (constant growth) Dynamic Array data type             
 **   Modify struc Data in DynamicArrays.h for payload according application  
 **   To change growth policy modify PushToDArray as needed                   
 **                                                                           
 ** Written by: Greg Semeraro,Ph.D.                                           
 ** Modified: Dr. Juan C. Cockburn (jcck@ieee.org)                            
 ** 02/01/2014 JCCK     
 ** 10/02/2015 R Repka 
 ** 03/03/2017 R Repka
 ** 07/21/2017 R.Repka     - added class errors include file
 ** 09/25/2017 R.Repka     - Minor pseudo code correction
 ** 09/15/2019 R. Repka    - Added example pointer usage
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassErrors.h"
#include "DynamicArrays.h"

/***************************************************************************
 CreateDArray -- Allocates memory and initializes DArray state             
        void CreateDArray(DArray *DArrayHead, unsigned int InitialSize)
 Where:
    DArray *DArrayHead       - Pointer to a storage block which holds the
                               data structure entry 
    unsigned int InitialSize - The initial data block size, could be 0
    returns: void            - nothing is returned.
    errors:                  - This routine will print an error message to 
                               stderror and exit with an error code
 **************************************************************************/
void CreateDArray(DArray *DArrayHead, unsigned int InitialSize) {
   /************************************************************************
   Pseudo code: 
   Initialize array with 0 entries used 
   Initialize capacity (you decide, could be zero)
   Based on capacity allocate memory storage, if necessary 
      If initial capacity is zero, no allocation needed 
      else Allocate storage space for payload according to struc Data
   if memory not allocated print error to stderr and exit with a code 
   
   Access examples:    DArrayHead->Capacity  = something
                       DArrayHead->Payload   = some malloc code
  ************************************************************************/
 DArrayHead->EntriesUsed = 0;
    DArrayHead->Capacity = InitialSize;
    if (InitialSize > 0) {
        DArrayHead->Payload = (Data *)malloc(InitialSize * sizeof(Data));
        if (DArrayHead->Payload == NULL) {
            fprintf(stderr, "Memory allocation for dynamic array failed\n");
            exit(EXIT_FAILURE);
        }
    } else {
        DArrayHead->Payload = NULL;
    } 
  
} /* CreateDArray() */


/***************************************************************************
  PushToDArray -- Adds data at the end of the dynamic array and updates its 
                  state in the array header.  If full, reallocates memory 
                  space according to growth policy                          
        unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)     
Where:
    DArray *DArrayHead      - Pointer to a storage block which holds the
                              data structure entry 
    Data * PayloadPtr       - 
    returns: unsigned int   - The index of the last element inserted
    errors:                 - This routine will print an error message to 
                              stderror and exit with an error code 
 **************************************************************************/
unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)
{
   /************************************************************************
    Pseudo code: 
    Check if the array needs to be made larger 
    If yes, increase the size of the dynamic array by GROWTH_AMOUNT
    Re-allocate storage for the array elements using larger size 
    if memory not re-allocated print error to stderr and exit 
    Copy the Data in PlayloadPtr into the Darray, using the EntriesUsed as 
    an index into the Darray.
    Increment the number of elements used in Darray header 
    Return the array index of last element inserted 
   ************************************************************************/
     
	// Check if the array needs to be made larger 
     if (DArrayHead->EntriesUsed >= DArrayHead->Capacity) {
        // Increase the size of the dynamic array by GROWTH_AMOUNT
        unsigned int newSize = DArrayHead->Capacity + GROWTH_AMOUNT;
        
        // Re-allocate storage for the array elements using the larger size 
        Data *newPayload = (Data *)realloc(DArrayHead->Payload, newSize * sizeof(Data));
        
        // If memory not re-allocated print error to stderr and exit 
        if (newPayload == NULL) {
            fprintf(stderr, "Failed to expand dynamic array\n");
            exit(EXIT_FAILURE);
        }
        
        // Update the dynamic array payload and capacity
        DArrayHead->Payload = newPayload;
        DArrayHead->Capacity = newSize;
    }
    
    // Copy the Data in PayloadPtr into the DArray
    DArrayHead->Payload[DArrayHead->EntriesUsed] = *PayloadPtr;
    
    // Increment the number of elements used in DArray header and return the index
    return DArrayHead->EntriesUsed++;
} /* PushToDArray() */


/*************************************************************************
 DestroyDArray -- Returns the memory back to the heap and updates the   
                  state in the array header      
        void DestroyDArray(DArray *DArrayHead)
  Where:
    DArray *DArrayHead  - Pointer to a storage block which holds the
                          data structure entry 
    returns: void       - nothing is returned.
    errors:             - No errors are reported
*************************************************************************/
void DestroyDArray(DArray *DArrayHead)
{
   /**********************************************************************
    Pseudo code: 
    Set the number of entries used to zero in Darray header 
    Set the capacity to zero in Darray header
    De-allocate the storage for the array elements 
 *************************************************************************/
 
    free(DArrayHead->Payload); // Free the memory for the array items
    DArrayHead->Payload = NULL; // Set the pointer to NULL
    DArrayHead->EntriesUsed = 0; // Reset the number of items to 0
    DArrayHead->Capacity = 0; // Reset the capacity to 0
    
} /* DestroyDArray() */

/**************************************************************************
 Searches the dynamic array for a provided word and returns the
 pointer to the data if found or NULL if not found.

 Where: DArray *DArrayHead - Pointer to the dynamic array to search
        char *String       - The item to search for
 Returns: A pointer to the data found or NULL for not found
 Errors: none
***************************************************************************/
//Data *SearchDArray(DArray *DArrayHead, char *String) {
   
   /*-----  Don't implement this until instructed in Lab 6 ----*/
//for (unsigned int i = 0; i < DArrayHead->EntriesUsed; i++) {
        // Use strncmp for a safer comparison, with a maximum length set to the size of the string in Data
      //  if (strncmp(DArrayHead->Payload[i].String, String, MAX_STR_LEN) == 0) {
     //       return &(DArrayHead->Payload[i]);
 //       }
   // }
   
  // return NULL;
//} /* SearchDArray() */ 