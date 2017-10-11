/*
 * main.c
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#include <stdio.h>
#include "linked_list.h"
#include "queue.h"

int main (int argc, char*args[]){

	char *data1 = "data1";


	list *test_list = list_empty();
	list_is_empty(test_list);
	list_insert(list_last(test_list), data1);
	printf("%s\n", (char*)list_inspect(list_last(test_list)));

	//queue *test_queue = queue_empty();






	return 0;
}
