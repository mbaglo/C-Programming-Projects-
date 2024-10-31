#ifndef _LINKED_LISTS_H_
#define _LINKED_LISTS_H_
/******************************************************************************
* Linked Lists Module public interface - LinkedLists.n
* Revised: 2/27/2015
  03/18/2016 - R. Repka     Added malloc debug macros
  10/10/2016 - R. Repka     Made macros more clever.  Simple macros are ok too
  07/21/2017 - R. Repka     Modified for ClassErrors.h
  12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
  06/14/2019 - R. Repka     Simplified the data structures
******************************************************************************/


/**************  Nothing else in the module needs to be modified *************/
#define MAX_BUFF_LEN    256

/******************************************************************************
* struct for linked list nodes, contains user information and link pointers.
******************************************************************************/
typedef struct LinkedListNodes
  {
  /* Link pointers to other nodes*/
   struct LinkedListNodes *Next;
   struct LinkedListNodes *Previous;
   
  /* The user information field, pointer to the actual data */
    char String[MAX_BUFF_LEN];   /* The data (English word) */
 
  } LinkedListNodes;

/*********************************************************************************
* struct for linked list "header", keeps a counter of the size of the linked list
**********************************************************************************/
typedef struct LinkedLists
  {
   /* Number of elements in the list */
   int NumElements;
   
   /* Pointer to the front of the list of elements, possibly NULL */
   struct LinkedListNodes *FrontPtr;
   
   /* Pointer to the end of the list of elements, possibly NULL */
   struct LinkedListNodes *BackPtr;
  } LinkedLists;

/******************************************************************************
* Initialize the linked list data structure
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr);

/******************************************************************************
* Adds a node to the back of the list.
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, char *DataPtr);

/******************************************************************************
* Removes a node from the front of the list and returns a pointer to the node
* removed. On empty lists should return a NULL pointer.
******************************************************************************/
char *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr);

/******************************************************************************
* De-allocates the linked list and resets the struct fields (in the header) 
* to indicate that the list is empty.
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr);

/*****************************************************************************
* Search's the linked list for a provided word. 
  Returns 1 if found, returns 0 if not found
*****************************************************************************/
int SearchList(LinkedLists *ListPtr, char *String);

#endif /* _LINKED_LISTS_H_ */
