/*
 * server.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#define NUMBER_HANDLERS 255

#include <pthread.h>
#include "linked_list.h"
#include "queue.h"
#include "socket_templates.h"

// shared variables
pthread_mutex_t cond_mutex;
pthread_cond_t cond_var;

/**
 * struct server
 *
 * Main data container for
 * server application.
 */
typedef struct server {
	uint16_t our_id;
	uint16_t our_port;
	char* servername;
	char* our_host;
	uint16_t nameserver_port;
	char* nameserver_host;
	io_handler *client_array[255];
	pthread_mutex_t com_mutex[255];
	pthread_mutex_t client_list_lock;
	struct communicator *com_array;
	io_handler *listener;
	list *client_list;
	int number_of_clients;
	int *bail_out;
}server;

/**
 * struct communicator
 *
 * Data container for each
 * communication thread.
 */
typedef struct communicator {
	int thread_id;
	io_handler * handler;
	pthread_mutex_t *handler_lock;
	struct communicator *com_array;
	list *client_list;
	pthread_mutex_t *client_list_lock;
	char *client_name;
	int *number_of_clients;
	int joined;
	int *bail_out;
} communicator;



#endif /* SRC_SERVER_SERVER_H_ */
