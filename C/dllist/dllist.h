/**
 * CS 2110 - Spring 2017 - Homework #10
 *
 * Do not modify this file!
 *
 * dllist.h
 */

#ifndef DLLIST_H
#define DLLIST_H
#include <stdbool.h>
/**************************
 ** Datatype definitions **
 **************************/

/* Forward declaration
 * http://en.wikipedia.org/wiki/Forward_declaration
 */
struct dnode;

/* Given to you for free, however you better know how to do this come time for
 * the final exam!
 * The dllist struct. Has a head & tail pointer.
 */
typedef struct
{
  struct dnode* head; /* Head pointer either points to a node with data or NULL */
  struct dnode* tail; /* Tail pointer either points to a node with data or NULL */
  int size; /* Size of the dllist */
} dllist;

/* Given to you for free, However you better know how to do this come time for
 * the final exam!
 * A function pointer type that points to a function that takes in a void* and
 * returns nothing, call it dllist_op
 */
typedef void (*dllist_op)(void*);

/* A function pointer type that points to a function that takes in a const void*
 * and returns an int, call it dllist_pred
 */
typedef int (*dllist_pred)(const void*);

/* A function pointer type that points to a function that takes in a void* and
 * returns a void*, call it dllist_copy
 */
typedef void* (*dllist_copy)(const void*);

/* A function pointer type that points to a function that takes in two void* and
 * returns an int, call it dllist_eq
 */
typedef int (*dllist_eq)(const void*, const void*);

/*********************************************
** Prototypes for dllist library functions. **
**                                          **
** For more details on their functionality, **
** check dllist.c.                          **
**********************************************/

/* Creating */
dllist *create_dllist(void);
dllist *copy_dllist(dllist *d, dllist_copy copy_func);

/* Adding */
void push_front(dllist *d, void *data);
void push_back(dllist *d, void *data);
bool add(dllist *d, void *data, int index);

/* Querying Dllist */
void *front(dllist *d);
void *back(dllist *d);
void *get(dllist *d, int index);
bool is_empty(dllist *d);
int size(dllist *d);
void *contains(dllist *d, void *data, dllist_eq eq_func);

/* Removing */
void *pop_front(dllist *d);
void *pop_back(dllist *d);
void empty_dllist(dllist *d, dllist_op free_func);

/* Traversal */
void traverse(dllist *d, dllist_op do_func);

/* Modifiers */
void reverse(dllist *d);

#endif
