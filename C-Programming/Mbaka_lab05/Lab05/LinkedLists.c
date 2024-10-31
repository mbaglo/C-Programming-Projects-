/******************************************************************************
 An student framework implementation of doubly linked lists that holds 
 elements containing a 256 character string and a sequence number.
 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
 03/12/2018 - R. Repka     Added pseudo code 
 09/17/2019 - R. Repka     fixed minor prototype errors 
 09/26/2019 - R. Repka     Added comments to RemoveFromFrontOfLinkedList()
 10/01/2019 - R. Repka     Changed search function to int
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassErrors.h"
#include "LinkedLists.h"


/******************************************************************************
  Use the following pseudo code to help with all the functions in this module
  
  add 1st node
    LinkedList front = points to node 1,  LinkedList back = points to node 1
      Node 1 next     = NULL  (at the end)
      Node 1 previous = NULL (at the start)
          
  add 2nd node
    LinkedList front = points to node 1,  LinkedList back = points to node 2
      Node 1 next points to node 2             Node 2 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)    Node 2 previous points to node 1

  add 3rd node
  LinkedList front = points to node 1,  LinkedList back = points to node 3
      Node 1 next points to node 2           Node 2 next points to node 3      Node 3 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)  Node 2 previous points to node 1  Node 3 previous points to node 2
      
   Note: It is possible to implement with only 2 special cases 
      
   Remove from front of 3 node list 
    save the pointer to the data and return it at the end
    LinkedList front = points to node 2,  LinkedList back = points to node 3
      Node 2 next points to node 3           Node 3 next   = NULL  (at the end)
      Node 2 previous = NULL (at the start)  Node 3 previous points to node 2
      free Node but NOT the data
******************************************************************************/

/******************************************************************************
 Initialize the linked list data structure.  Points to nothing, no entries.

 Where: LinkedLists *ListPtr - Pointer to the linked list to create
 Returns: nothing
 Errors: none
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr)
{
	ListPtr->NumElements = 0;
    ListPtr->FrontPtr = NULL;
    ListPtr->BackPtr = NULL;
} /* InitLinkedList() */


/******************************************************************************
 Adds a node to the back of the list.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to add to
        char *DataPtr - Pointer to the data to add to the list
 Returns: nothing
 Errors: Prints to stderr and exits
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, char *DataPtr)
{
	LinkedListNodes *newNode = (LinkedListNodes *)malloc(sizeof(LinkedListNodes));
    	if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(-1); // Exit if memory allocation fails
    	}

    	strncpy(newNode->String, DataPtr, MAX_BUFF_LEN);
    	newNode->String[MAX_BUFF_LEN - 1] = '\0'; // Ensure null-termination
    	newNode->Next = NULL; // This will be the new last node, so no next node
    	newNode->Previous = ListPtr->BackPtr; // Previous node is the current back

    	if (ListPtr->BackPtr != NULL) { // List is not empty
        ListPtr->BackPtr->Next = newNode;
    	} else { // List is empty
        ListPtr->FrontPtr = newNode; // This new node is also the front
    	}

    	ListPtr->BackPtr = newNode; // Update the list's back pointer to the new node
    	ListPtr->NumElements++;
} /* AddToBackOfLinkedList */

/******************************************************************************
 Removes a node from the front of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.  
 Note: You will need to malloc a string buffer and copy the data from the
       linked list node before destroying the node.  The calling routine was
       written to free the string so there are no memory leaks

 
   Linked lists contain nodes, which contain data strings
 
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the data removed or NULL for none
 Errors: none
******************************************************************************/
char *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr)
{
	if (ListPtr->FrontPtr == NULL) {
        return NULL; // List is empty
    	}

    	LinkedListNodes *nodeToRemove = ListPtr->FrontPtr;
    	char *removedData = (char *)malloc(MAX_BUFF_LEN * sizeof(char));
    	strncpy(removedData, nodeToRemove->String, MAX_BUFF_LEN);

    	ListPtr->FrontPtr = nodeToRemove->Next; // Update the front pointer
    	if (ListPtr->FrontPtr != NULL) {
        ListPtr->FrontPtr->Previous = NULL;
    	} else {
        ListPtr->BackPtr = NULL; // List became empty
    	}

    	free(nodeToRemove);
    	ListPtr->NumElements--;
    	return removedData;
} /* RemoveFromFrontOfLinkedList() */


/******************************************************************************
 De-allocates the linked list and resets the struct fields (in the header) 
 to indicate that the list is empty.  
 
 If you choose to use the RemoveFromFrontOfLinkedList() function, remember 
 the calling routine must free the returned pointer so there are no memory leaks
       
 Where: LinkedLists *ListPtr    - Pointer to the linked list to destroy
 Returns: nothing
 Errors: none
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr)
{
 	LinkedListNodes *current = ListPtr->FrontPtr;
    	while (current != NULL) {
        LinkedListNodes *next = current->Next;
        free(current);
        current = next;
   	}

    	ListPtr->NumElements = 0;
    	ListPtr->FrontPtr = NULL;
    	ListPtr->BackPtr = NULL;
} /* DestroyLinkedList() */


/******************************************************************************
 Searches the linked list for a provided word. If found, returns 1 otherwise 0
 
 Where: LinkedLists *ListPtr - Pointer to the linked list to search
        char *String         - Pointer to the string to search
 Returns: 1 if found, 0 otherwise
 Errors: none
 * ***************************************************************************/
int SearchList(LinkedLists *ListPtr, char *String)
{
/*-----  Don't implement this until instructed in a future lab ----*/
	LinkedListNodes *current = ListPtr->FrontPtr;
   	while (current != NULL) {
        if (strncmp(current->String, String, MAX_BUFF_LEN) == 0) {
            return 1; // Found
        }
        current = current->Next;
    	}
	return 0;// Not found
} /* SearchList() */




