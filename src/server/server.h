/*
 * server.h
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include "linked_list.h"
#include "queue.h"
#include "socket_templates.h"


typedef struct server {
	io_handler *client_array[255];
	io_handler *listener;
	list *client_list;
	queue *message_list;
}server;

typedef struct communicator {
	int thread_id;
	io_handler * handler;
	struct communicator *com_array;
	queue *message_list;
} communicator;


#endif /* SRC_SERVER_SERVER_H_ */
