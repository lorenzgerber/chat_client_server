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
#include "socket_creator.h"

#define NUMBER_HANDLERS 255


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
	pthread_t thread_handle[NUMBER_HANDLERS];
	pthread_t thread_listen;
	communicator com[NUMBER_HANDLERS];

	server server;
	signal(SIGINT, intHandler);


	// start com threads
	for(int i = 0; i < NUMBER_HANDLERS; i++){
		server.client_array[i] = NULL;
		server.com_array = com;
		pthread_mutex_init(&server.com_mutex[i], NULL);
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

	//pthread_cond_broadcast(&cond_var);
	//sleep(2);
	// now we go down
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

void * listen_loop(void* data){
	char* address = "localhost";
	server *server = data;
	server->listener = create_tcp_server_listener(address, 2000);
	io_handler *new_com;


	// listener
	while(keep_running){
		int assigned = 0;
		new_com = NULL;
		new_com = server->listener->listen(server->listener);
		sleep(2);



		// implement here transfer of io_handler to com_array
		if(bail_out != 1){

			for(int i = 0; i < NUMBER_HANDLERS && assigned == 0; i++){
				pthread_mutex_lock(&server->com_mutex[i]);
				if(server->com_array[i].handler == NULL){
					server->com_array[i].handler = new_com;
					assigned = 1;
				}
				pthread_mutex_unlock(&server->com_mutex[i]);
			}

			// if iterated over whole array and still not assigned
			if(assigned == 0){
				printf("no more free client slots on server\n");
			}

			// after transfer - signal all threads
			pthread_cond_broadcast(&cond_var);


		}

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



		if(bail_out == 0){
			// receive and parse
			pdu* pdu;
			pdu = parse_header(com->handler);
			if(com->handler->status == STATUS_RECEIVE_OK){
				pdu->print(pdu);
			}

		}

	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

    return NULL;
}




