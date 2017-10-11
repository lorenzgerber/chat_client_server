/*
 * main.c
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#include <stdio.h>
#include "linked_list.h"
#include "queue.h"
#include <string.h>

int main (int argc, char*args[]){

	char *data1 = malloc(sizeof(char)*10);
	char *data2 = malloc(sizeof(char)*10);
	strcpy(data1, "data1");
	strcpy(data2, "data2");


	list *test_list = list_empty();
	list_is_empty(test_list);
	list_insert(list_last(test_list), data1);
	printf("%s\n", (char*)list_inspect(list_last(test_list)));


	// create new empty queue
	queue *test_queue = queue_empty();

	// check if queue is empty
	if(is_empty(test_queue)){
		printf("this queue is empty\n");
	} else {
		printf("this queue is not empty\n");
	}

	// enqueue data
	enqueue(test_queue, data1);
	enqueue(test_queue, data2);

	// check if queue is no longer empty
	if(is_empty(test_queue)){
		printf("this queue is empty\n");
	} else {
		printf("this queue is not empty\n");
	}

	// get data from front
	printf("%s\n", (char*) front(test_queue));

	// dequeue
	dequeue(test_queue);

	// get data from front
	printf("%s\n", (char*) front(test_queue));

	// dequeue
	dequeue(test_queue);

	// not the queue should be empty again
	if(is_empty(test_queue)){
		printf("this queue is empty\n");
	} else {
		printf("this queue is not empty\n");
	}


	return 0;
}
