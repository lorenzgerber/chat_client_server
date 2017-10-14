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
#include "socket_creator.h"


// shared variables
pthread_mutex_t cond_mutex;
pthread_cond_t cond_var;
int  bail_out = 0;
static volatile int keep_running = 1;

void intHandler(int dummy) {
    keep_running = 0;
}


void * com_loop(void* data);
void * listen_loop(void* data);

int main (int argc, char*argv[]){

	// setup variables for listener and communication server
	pthread_t thread_handle[255];
	pthread_t thread_listen;
	communicator com[255];

	server server;
	signal(SIGINT, intHandler);


	// start com threads
	for(int i = 0; i < 255; i++){
		server.client_array[i] = NULL;
		com[i].thread_id = i;
		com[i].handler = NULL;
		com[i].com_array = com;
		pthread_create(&thread_handle[i], NULL, com_loop, &com[i]);
	}

	// start listener
	pthread_create(&thread_listen, NULL, listen_loop, &server);


	// Exit / shutdown condition
	while(keep_running){
		sleep(1);
	}


	// shutting down orderly
	server.listener->close(server.listener);

	pthread_cond_broadcast(&cond_var);
	sleep(2);
	// now we go down
	bail_out = 1;
	pthread_cond_broadcast(&cond_var);

	for(int i = 0; i < 255; i++){
		pthread_join(thread_handle[i], NULL);
	}
	pthread_join(thread_listen, NULL);

	return 0;
}

void * listen_loop(void* data){
	char* address = "localhost";
	server *server = data;
	server->listener = create_tcp_server_listener(address, 2000);
	io_handler *new_com;

	// listener
	while(keep_running){
		new_com = server->listener->listen(server->listener);
		// implement here transfer of io_handler to com_array

		// after transfer - signal all threads

	}

	// clean up in case of shutdown
	free_tcp_server_listener(server->listener);
	free_tcp_server_communicator(new_com);
	printf("stop listening\n");
	return NULL;
}


void * com_loop(void* data){

	communicator *com = data;


	while(bail_out == 0){

		// lock the cond mut
		pthread_mutex_lock(&cond_mutex);

		// check condition
		while(com->handler == NULL && bail_out == 0){
			pthread_cond_wait(&cond_var, &cond_mutex);
		}
		pthread_mutex_unlock(&cond_mutex);

		printf("thread %d here!\n", com->thread_id);

		sleep(1);
	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

	/// com loop
	/// ---------
	/// conditional, if my io_handler is not zero run else hold
	/// receive
	/// if error remove client from list (use locking)
	/// if receive OK, parse
	/// if parse OK, prepare message
	/// send to all clients, by using their sfds (use locking to access sfds)
	/// if send fails, nothing happens
	/// start over

    return NULL;
}




