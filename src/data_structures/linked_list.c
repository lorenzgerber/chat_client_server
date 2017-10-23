/*
 * linked_list.c
 *
 * Lorenz Gerber
 *
 * Laboration 1, Course 'System-level programming', HT16
 *
 * code file consists an implementation of a double linked list using
 * generic/void data fields.
 *
 */

#include <client.h>
#include "linked_list.h"


//create empty list, constructor
list *list_empty(void) {
    list *new_list=malloc(sizeof(struct list));
    if(new_list == NULL){
        perror("list memory allocation\n");
        exit(EXIT_FAILURE);
    }
    new_list->head=malloc(sizeof(element));
    if(new_list->head == NULL){
        perror("list head memory allocation\n");
        exit(EXIT_FAILURE);
    }
    new_list->head->next=new_list->head;
    new_list->head->previous=new_list->head;
    new_list->freeFunc=NULL;
    return new_list;
}


// set mem handler
void list_set_mem_handler(list *l, memFreeFunc *f){
    l->freeFunc=f;
}


// return first position
list_position list_first(list *l){
    return l->head->next;
}

// return last position
list_position list_last(list *l){
    return l->head->previous;
}

// get next position
list_position list_next(list_position p) {
    return p->next;
};

// get previous position
list_position list_previous(list_position p){
    return p->previous;
};

// is list empty
bool list_is_empty(list *l) {
    return (l->head->next == l->head);
}

// insert list element
list_position list_insert(list_position p,data d) {
    list_position newPosition=malloc(sizeof(element));
    if(newPosition == NULL){
        perror("Memory allocation for new list element\n");
        exit(EXIT_FAILURE);
    }
    newPosition->data=d;
    newPosition->next=p->next;
    (p->next)->previous=newPosition;
    newPosition->previous=p;
    p->next=newPosition;
    return newPosition;
};

// inspect list element
data* list_inspect(list_position p) {
  return p->data;
};

// remove list element
list_position list_remove(list *l, list_position p) {
    list_position before_remove=p->previous;
    p->previous->next=p->next;
    p->next->previous=p->previous;
    if(l->freeFunc!=NULL)
        l->freeFunc(p->data);
    free(p);
    return before_remove;
};

// free memory
void list_free(list *l){
    list_position current = list_last(l);
    while (list_is_empty(l) != true){
        current = list_remove(l, current);
    }
    free(l->head);
    free(l);
};

void list_free_servers(list *l){
    list_position current = list_last(l);
    while (list_is_empty(l) != true){
        free(((chat_server*)list_inspect(current))->address);
        free(((chat_server*)list_inspect(current))->server_name);
        current = list_remove(l, current);
    }
    free(l->head);
    free(l);
};

// check if end of list
bool list_is_end(list *l, list_position p){
  if ( p->next == l->head) {
    return true;
  } else {
    return false;
  }
};
