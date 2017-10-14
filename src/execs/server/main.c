/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "socket_creator.h"


// shared variables
pthread_mutex_t cond_mutex;
pthread_cond_t cond_var;
int  bail_out = 0;


void * com_loop(void* data);

int main (int argc, char*argv[]){


	// setup variables for listener and communication server
	pthread_t thread_handle[255];
	communicator com[255];
	char* address = "localhost";
	server server;

	int test_sfd = 10;


	// start com threads
	for(int i = 0; i < 255; i++){
		server.client_array[i] = NULL;
		com[i].thread_id = i;
		com[i].handler = NULL;
		com[i].com_array = com;
		pthread_create(&thread_handle[i], NULL, com_loop, &com[i]);
	}


	// Main loop in listener
	// listen
	// when new client shows up, find empty communicator slot
	// assign io_handler to empty communicator slot (use locking)
	// broadcast on conditional var
	// signal handling
	// restart

	// listen loop
	for(int i = 0; i < 10; i++){
		com[i].handler = create_tcp_server_communicator(&test_sfd);

	}

	pthread_cond_broadcast(&cond_var);
	sleep(2);
	// now we go down
	bail_out = 1;
	pthread_cond_broadcast(&cond_var);





	for(int i = 0; i < 255; i++){
		pthread_join(thread_handle[i], NULL);
	}

	return 0;
}

void * com_loop(void* data){

	communicator *com = data;
	//int counter = 0;
	//int sfd = 1;
	//com->handler = create_tcp_server_communicator(&sfd);


	while(bail_out == 0){

		// lock the cond mut
		pthread_mutex_lock(&cond_mutex);

		// check condition
		while(com->handler == NULL && bail_out == 0){
			// if still NULL -> sleep
			pthread_cond_wait(&cond_var, &cond_mutex);
			//printf("thread %d woken up!\n", com->thread_id);

		}
		pthread_mutex_unlock(&cond_mutex);

		printf("thread %d here!\n", com->thread_id);

		sleep(1);
	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

	//free_tcp_server_communicator(com->handler);


	/// Main loop
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




