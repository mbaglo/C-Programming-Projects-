/****************************************************************************
 The purpose of this example is to exercise, using pointers
 
 student version 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_c.c -o lab_c
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99 ./lab_c
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*---------------------------------------------------------------------------
  These data structures are used to simulated a very simple linked list
---------------------------------------------------------------------------*/
// Data type for each node in our linked list
typedef struct Node{
    int index;             // The node number
    struct Node *next;     // Pointer to the next node or NULL if last
    char string[40];       // The data payload
} Node;


// This is the master linked list starting point
typedef struct {
    int nterms;            // The total number of active nodes
    Node *front;           // Pointer to the first node
} Wrapper;



// Function prototypes
void printList(Wrapper *wrapper_p);

int main(int argc, char *argv[]) {
   Wrapper *wrapper_p;
   Node *node_p;
   Node *temp_p;
   
// Allocate memory for the wrapper and initialize
   wrapper_p = (Wrapper *)malloc(sizeof(Wrapper));
   wrapper_p->nterms = 0; // Initially no nodes
   wrapper_p->front = NULL; // No front node yet

// Wire the first node in

	// Allocate memory for the first node and initialize
   	node_p = (Node *)malloc(sizeof(Node));
   	node_p->index = 1; // First node index
   	strncpy(node_p->string, "one", 39); // Secure copy string into node
   	node_p->string[39] = '\0'; // Ensure null-termination
   	node_p->next = NULL; // No next node yet

   	// Link the first node to the wrapper
   	wrapper_p->front = node_p;
   	wrapper_p->nterms++; // Increment the number of active nodes
	
	// Print the list with one node
   	printList(wrapper_p);
   
// Wire the second node in 

	// Allocate memory for the second node and initialize (Optional)
   	temp_p = (Node *)malloc(sizeof(Node));
   	temp_p->index = 2; // Second node index
   	strncpy(temp_p->string, "two", 39); // Secure copy string into node
   	temp_p->string[39] = '\0'; // Ensure null-termination
   	temp_p->next = NULL; // No next node yet
	
	// Link the second node to the first node
   	node_p->next = temp_p;
   	wrapper_p->nterms++; // Increment the number of active nodes

	// Print the list with two nodes
   	printList(wrapper_p);
   
// Clean up memory
	
   	// Free the second node
   	free(temp_p);
   	// Free the first node
   	free(node_p);
   	// Free the wrapper
   	free(wrapper_p);
   
   return 0;
}

// Walk the linked list and print the contents 
void printList(Wrapper *wrapper_p) {
    Node *current = wrapper_p->front; // Start with the first node
    while (current != NULL) { // Continue until the end of the list
        printf("Node %d contains the string '%s'\n", current->index, current->string);
        current = current->next; // Move to the next node
    }
}
