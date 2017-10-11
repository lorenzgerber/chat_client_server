/*
 * queue.c
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#include "queue.h"


// create empty queue
queue *queue_empty(void) {

	queue *new_queue =  malloc(sizeof(struct queue));

    if(new_queue == NULL){
        perror("list memory allocation\n");
        exit(EXIT_FAILURE);
    }

    new_queue->first = NULL;

    new_queue->freeFunc = NULL;
    return new_queue;
}


// set mem handler
void queueSetMemHandler(queue *q, memFreeFunc *f){
	q->freeFunc = f;
}


// enqueue
int enqueue(queue *q, data d){
	queue_element *new_element = malloc(sizeof(queue_element));
	if(new_element == NULL){
		perror("element memory allocation\n");
		exit(EXIT_FAILURE);
	}
	new_element->data = d;
	new_element->next = NULL;
	if(q->first == NULL){
		q->first = new_element;
	} else {
		queue_position current_position = q->first;
		while(current_position->next != NULL){
			current_position = current_position->next;
		}
		current_position->next = new_element;
	}


	return 0;
}

// IsEmpty
int is_empty(queue *q){
	if (q->first == NULL){
		return 1;
	} else {
		return 0;
	}
}

// get Front
data* front(queue *q){
	if(q->first!=NULL){
		return q->first->data;
	}
	return NULL;
}

// dequeue
int dequeue(queue *q){
	if(q->freeFunc!=NULL){
		q->freeFunc(q->first->data);
	}
	queue_position to_remove = q->first;
	q->first = q->first->next;
	free(to_remove);
	return 0;
}

