/*
 * server.h
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#define NUMBER_HANDLERS 255

#include "linked_list.h"
#include "queue.h"
#include "socket_templates.h"

// shared variables
pthread_mutex_t cond_mutex;
pthread_cond_t cond_var;



static volatile int keep_running = 1;



typedef struct server {
	io_handler *client_array[255];
	pthread_mutex_t com_mutex[255];
	struct communicator *com_array;
	io_handler *listener;
	list *client_list;
	int *bail_out;
}server;

typedef struct communicator {
	int thread_id;
	io_handler * handler;
	pthread_mutex_t *handler_lock;
	struct communicator *com_array;
	int joined;
	int *bail_out;
} communicator;






#endif /* SRC_SERVER_SERVER_H_ */
