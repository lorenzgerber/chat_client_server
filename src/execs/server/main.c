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


void * main_loop(void* data);

int main (int argc, char*argv[]){

	pthread_t thread_handle[255];
	communicator com[255];

	// setup variables for listener and communication server
	char* address = "localhost";
	server server;
	for(int i = 0; i < 255; i++){
		server.client_array[i] = NULL;
		com[i].thread_id = i;
		com[i].handler = NULL;
		com[i].com_array = com;
		pthread_create(&thread_handle[i], NULL, main_loop, &com[i]);
	}


	// Main loop in listener
	// listen
	// when new client shows up, find empty communicator slot
	// assign io_handler to empty communicator slot (use locking)
	// broadcast on conditional var
	// signal handling
	// restart





	for(int i = 0; i < 255; i++){
		pthread_join(thread_handle[i], NULL);
	}

	return 0;
}

void * main_loop(void* data){

	communicator *com = data;
	int counter = 0;
	int sfd = 1;
	com->handler = create_tcp_server_communicator(&sfd);


	while(counter < 10){
		printf("thread %d here!\n", com->thread_id);

		if(com->thread_id == 10){
			sleep(1);
			if(com->handler != NULL){
				free_tcp_server_communicator(com->handler);
				com->handler = NULL;
			}

		}

		/*if(com->com_array[10].handler != NULL){
			printf("com 10 is not empty\n");
		} else {
			printf("com 10 is now emtpy\n");
		}*/


		sleep(1);
		counter++;
	}
	if(com->thread_id != 10){
		free_tcp_server_communicator(com->handler);
	}

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




