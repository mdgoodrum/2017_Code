/**
 * CS 2110 - Spring 2017 - Homework #10
 *
 * @author Michael Goodrum
 *
 * dllist.c: Complete the functions!
 */

#include <stdlib.h>
#include <stdio.h>
#include "dllist.h"

/* The node struct. Has a prev pointer, next pointer, and data. */
/* DO NOT DEFINE ANYTHING OTHER THAN WHAT'S GIVEN OR YOU WILL GET A ZERO */
/* Design consideration: Only this file should know about nodes */
/* Only this file should be manipulating nodes */
/* DO NOT TOUCH THIS DECLARATION, DO NOT PUT IT IN OTHER FILES */
typedef struct dnode
{
    struct dnode* prev; /* Pointer to previous node */
    struct dnode* next; /* Pointer to next node */
    void* data; /* User data */
} node;

/* Do not create any global variables here. Your dllist library should obviously
 * work for multiple concurrent dllists */

// This function is declared as static since you should only be calling this
// function from inside this file.
static node* create_node(void* data);


/** create_node
  *
  * Helper function that creates a node by allocating memory for it on the heap.
  * Be sure to set its pointers to NULL.
  *
  * @param data a void pointer to data the user wants to store in the dllist
  * @return a node
  */
static node* create_node(void* data)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/** create_dllist
  *
  * Creates a dllist by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head/tail to NULL.
  *
  * @return an empty dllist
  */
dllist* create_dllist(void)
{
	dllist* list = (dllist*)malloc(sizeof(dllist));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;

}

/** push_front
  *
  * Adds the data to the front of the dllist.
  *
  * @param d a pointer to the dllist structure.
  * @param data pointer to data the user wants to store in the dllist.
  */
void push_front(dllist *d, void *data)
{
	if(d == NULL) {
		return;
	}
	node* newNode = create_node(data);
	if(d->head == NULL) {
		d->head = newNode;
		d->tail = newNode;
		d->size++;
		return;
	}
	newNode->next = d->head; 
	newNode->prev = NULL;
	d->head->prev = newNode;
	d->head = newNode;
	d->size++;
}

/** push_back
  *
  * Adds the data to the back of the dllist.
  *
  * @param d a pointer to the dllist structure.
  * @param data pointer to data the user wants to store in the dllist.
  */
void push_back(dllist *d, void *data)
{
	if(d == NULL) {
		return;
	}
	node* newNode = create_node(data);
	if(d->tail == NULL) {
		d->tail = newNode;
		d->head = newNode;
		d->size++;
		return;
	}
	d->tail->next = newNode;
	newNode->next = NULL;
	newNode->prev = d->tail;
	d->tail = newNode;
	d->size++;
}

/** front
  *
  * Gets the data at the front of the dllist
  * If the dllist is empty return NULL.
  *
  * @param d a pointer to the dllist
  * @return The data at the first node in the dllist or NULL.
  */
void *front(dllist *d)
{
	if(d == NULL) {
		return NULL;
	}
	if(d->head == NULL) {
		return NULL;
	}
	return d->head->data;
}

/** back
  *
  * Gets the data at the "end" of the dllist
  * If the dllist is empty return NULL.
  *
  * @param d a pointer to the dllist structure
  * @return The data at the last node in the dllist or NULL.
  */
void *back(dllist *d)
{
	if(d == NULL) {
		return NULL;
	}
	if(d->tail == NULL) {
		return NULL;
	}
	return d->tail->data;
}

/** get
  *
  * Gets the data at the specified index in the dllist
  *
  * @param d a pointer to the dllist structure
  * @param index 0-based, starting from the head.
  * @return The data from the specified index in the dllist or NULL if index is
  *         out of range of the dllist.
  */
void *get(dllist *d, int index)
{
	if ((index < 0) | (index > (d->size - 1)) | (d == NULL)) {
		return NULL;
	}
	node* pointer = d->head;
	for (int i = 0; i < index; i++) {
		pointer = pointer->next;
	}
	return pointer->data;
}
/** add
  *
  * Adds the data at the specified index in the dllist.
  * Adding at index size should have the same behavior as push_back
  *
  * @param d a pointer to the dllist structure
  * @param index 0-based, starting from the head, to size.
  * @return true if insertion was successful or false if index is
  *         out of range of the dllist.
  */
bool add(dllist *d, void* data, int index)
{
	if(d == NULL) {
		return false;
	}
	if((index < 0) | (index > d->size)) {
		return false;
	}
	if(index == 0) {
		push_front(d, data);
		return true;
	}
	if(index == d->size) {
		push_back(d, data);
		return true;
	}
	node* newNode = create_node(data);
	node* pointer = d->head;
	for (int i = 0; i < index; i++) {
		pointer = pointer->next;
	}
	pointer->next->prev = newNode;
	newNode->next = pointer->next;
	newNode->prev = pointer;
	pointer->next = newNode;
	d->size++;
	return true;
	 
}

/** contains
  *
  * Traverses the dllist, trying to see if the dllist contains some data.
  * Since non-NULL values are considered true, this can be used like a boolean
  *
  * The "data" parameter may not necessarily point to the same address as the
  * equal data you are returning from this function, it's just data which the
  * eq_func says is equal. For instance, if you have a dllist of person structs:
  *   (Andrew, 26), (Nick, 24), (Collin, 23), (Marie, 23)
  * And you want to return any person whose age is 22, you could create a new
  * person struct (<NULL>, 24) with an eq_func that returns age == 24 and pass
  * that into this function as "data". contains() would then return (Nick, 24)
  *
  * If there are multiple pieces of data in the dllist which are equal to the
  * "data" parameter, return any one of them.
  *
  * @param d a pointer to the dllist structure
  * @param data The data, to see if it exists in the dllist
  * @param eq_func A function written by the user that will tell if two pieces
  *                of data are equal. Returns 0 if equal, something else
  *                otherwise. Imagine subtracting two numbers.
  * @return The data contained in the dllist, or NULL if it doesn't exist
  */
void *contains(dllist *d, void *data, dllist_eq eq_func) 
{
	if ((d != NULL) & (eq_func != NULL)) {
      		node* pointer = d->head;
      		while (pointer) {
        		if (eq_func(pointer->data, data) == 0) {
        			return pointer->data;
        		}
        		pointer = pointer->next;
		}
      		return NULL;
    	}
    	return NULL;
}

/** pop_front
  *
  * Removes the node at the front of the dllist, and returns its data to the user
  *
  * @param d a pointer to the dllist.
  * @return The data in the first node, or NULL if the dllist is NULL or empty
  */
void *pop_front(dllist *d)
{
	if (d && d->head) {
      		node* oldNode = d->head;
      		void* dataPtr = oldNode->data;
      		d->head = d->head->next;
      		d->size--;
		if(d->size == 0) {
			d->head = NULL;
			d->tail = NULL;
		} else {
			d->head->prev = NULL;
		}
      		free(oldNode);
      		return dataPtr;
    	}
    	return NULL;
}

/** pop_back
  *
  * Removes the node at the end of the dllist, and returns its data to the user
  *
  * @param d a pointer to the dllist.
  * @return The data in the first node, or NULL if the dllist is NULL or empty
  */
void *pop_back(dllist *d)
{
	if (d && d->tail) {
      		node* oldNode = d->tail;
      		void* dataPtr = oldNode->data;
      		d->tail = d->tail->prev;
      		d->size--;
		if(d->size == 0) {
			d->head = NULL;
			d->tail = NULL;
		} else {
			d->tail->next = NULL;
		}
      		free(oldNode);
      		return dataPtr;
    	}
    	return NULL;

}

/** copy_dllist
  *
  * Create a new dllist structure, new nodes, and new copies of the data by using
  * the copy function. Its implementation for any test structure must copy
  * EVERYTHING!
  *
  * @param d A pointer to the dllist structure to make a copy of
  * @param copy_func A function pointer to a function that makes a copy of the
  *                  data that's being used in this dllist, allocating space for
  *                  every part of that data on the heap. This is some function
  *                  you must write yourself for testing, tailored specifically
  *                  to whatever context you're using the dllist for in your test
  * @return The dllist structure created by copying the old one, or NULL if the
  *         structure passed in is NULL.
  */
dllist* copy_dllist(dllist *d, dllist_copy copy_func)
{
	if ((d != NULL) & (copy_func != NULL)) {
      		dllist* newList = create_dllist();
      		if (d->head != NULL) {
        		newList->head = (node*) create_node(copy_func(d->head->data));
			newList->head->prev = NULL;
        		node* newNode = newList->head;
        		node* nodePtr = d->head;
			newList->size++;
        		while (nodePtr->next) {
          			newNode->next = (node*) create_node(copy_func(nodePtr->next->data));
				newNode->next->prev = newNode;
          			nodePtr = nodePtr->next;
          			newNode = newNode->next;
				newList->size++;
        		}
      		}
     	 	return newList;
    	}
    	return NULL;
}

/** size
  *
  * Gets the size of the dllist
  *
  * @param d a pointer to the dllist structure
  * @return The size of the dllist
  */
int size(dllist *d)
{
	if(d == NULL) {
		return 0;
	}	
	return d->size;
}

/** is_empty
  *
  * Checks to see if the dllist is empty.
  *
  * @param d a pointer to the dllist structure
  * @return true if the dllist is indeed empty, or false otherwise.
  */
bool is_empty(dllist *d)
{
	return (!d || d->size) ? false : true;
}

/** empty_dllist
  *
  * Empties the dllist. After this is called, the dllist should be empty.
  * This does not free the dllist struct itself, just all nodes and data within.
  *
  * @param d a pointer to the dllist structure
  * @param free_func function used to free the nodes' data.
  */
void empty_dllist(dllist *d, dllist_op free_func)
{
	if ((d != NULL) & (free_func != NULL)) {
    		node* nodePtr = d->head;
   		node* oldPtr = NULL;
    		while (nodePtr) {
      			free_func(nodePtr->data);
      			oldPtr = nodePtr;
      			nodePtr = nodePtr->next;
      			free(oldPtr);
    		}
    		d->size = 0;
		d->head = NULL;
		d->tail = NULL;
  	}
}

/** traverse
  *
  * Traverses the dllist, calling a function on each node's data.
  *
  * @param d a pointer to the dllist structure
  * @param do_func a function that does something to each node's data.
  */
void traverse(dllist *d, dllist_op do_func)
{
	if ((d != NULL) & (do_func != NULL)) {
    		node* pointer = d->head;
    		for(int x = 0; x < (d->size); x++) {
      			do_func(pointer->data);
      			pointer = pointer->next;
    		}
  	}
}

/** reverse
  * Reverses the dllist
  * Should do nothing if d is NULL
  * @param d a pointer to the dllist structure
  */
void reverse(dllist *d) 
{
	if (d != NULL) {
		node *temp = NULL;  
     		node *current = d->head;
     		while (current !=  NULL) {
       			temp = current->prev;
       			current->prev = current->next;
       			current->next = temp;              
       			current = current->prev;
     		}      
      		if(temp != NULL ) {
      		 	d->head = temp->prev;
		}
	}
}	
