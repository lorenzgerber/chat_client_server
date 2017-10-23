/*
 * linked_list.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_DATA_STRUCTURES_LINKED_LIST_H_
#define SRC_DATA_STRUCTURES_LINKED_LIST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef void *data;

typedef void memFreeFunc(data);

// element for a double linked list
typedef struct element {
    struct element *previous;
    data data;
    struct element *next;
} element;

typedef element * list_position;

// head for a double linked list
typedef struct list {
    element *head;
    memFreeFunc *freeFunc;
} list;



//create empty list, constructor
list *list_empty(void);


// set mem handler
void list_set_mem_handler(list *l, memFreeFunc *f);


// return first position
list_position list_first(list *l);

// reutrn last position
list_position list_last(list *l);

// get next position
list_position list_next(list_position p);

// get previous position
list_position list_previous(list_position p);

// is list empty
bool list_is_empty(list *l);

// insert list element
list_position list_insert(list_position p,data d);

// inspect list element
data* list_inspect(list_position p);

// remove list element
list_position list_remove(list *l, list_position p);

// free memory
void list_free(list *l);

void list_free_servers(list *l);

// check if end of list
bool list_is_end(list *l, list_position p);


#endif /* SRC_DATA_STRUCTURES_LINKED_LIST_H_ */
