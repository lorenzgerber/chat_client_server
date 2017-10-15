/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "server.h"
#include "pdu_parser.h"
#include "pdu_creator.h"
#include "socket_creator.h"
#include "listener.h"



void intHandler(int dummy) {
    keep_running = 0;
}



void * com_loop(void* data);
void * listen_loop(void* data);

int main (int argc, char*argv[]){

	// setup variables for listener and communication server
	pthread_t thread_handle[NUMBER_HANDLERS];
	pthread_t thread_listen;
	communicator com[NUMBER_HANDLERS];
	int bail_out = 0;

	// Server Struct
	server server;
	server.bail_out = &bail_out;
	server.client_list = list_empty();
	list_set_mem_handler(server.client_list, free);
	signal(SIGINT, intHandler);




	// Start Communication threads
	for(int i = 0; i < NUMBER_HANDLERS; i++){
		server.client_array[i] = NULL;
		server.com_array = com;
		pthread_mutex_init(&server.com_mutex[i], NULL);
		com[i].handler_lock = &server.com_mutex[i];
		com[i].thread_id = i;
		com[i].handler = NULL;
		com[i].com_array = com;
		com[i].bail_out = &bail_out;
		com[i].client_list = server.client_list;
		pthread_create(&thread_handle[i], NULL, com_loop, &com[i]);
	}

	// Start Listener thread
	pthread_create(&thread_listen, NULL, listen_loop, &server);

	// Exit / shutdown condition
	while(keep_running){
		sleep(1);
	}

	// shutting down orderly
	server.listener->close(server.listener);

	bail_out = 1;
	pthread_cond_broadcast(&cond_var);

	for(int i = 0; i < NUMBER_HANDLERS; i++){
		pthread_join(thread_handle[i], NULL);
		pthread_mutex_destroy(&server.com_mutex[i]);
	}
	pthread_join(thread_listen, NULL);
	pthread_mutex_destroy(&cond_mutex);
	pthread_cond_destroy(&cond_var);

	return 0;
}









