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
		com[i].handler_lock = &server.com_mutex[i];
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
	com->joined = 0;
	pdu* pdu_receive;
	pdu_receive = NULL;

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
			pdu_receive = parse_header(com->handler);
			if(com->handler->status == STATUS_RECEIVE_OK){
				pdu_receive->print(pdu_receive);
				com->handler->status = 0;

				// here we check for JOIN message
				if(pdu_receive->type == 12 && com->joined == 0){

					com->joined = 1;

					// prepare and send participants to new user
					// todo
					// add new client identity to list


					// here we prepare the pjoined message
					pdu* pdu_response = create_pjoin(pdu_receive->identity_length);
					pdu_response->add_client_identity_timestamp(pdu_response, time(NULL), pdu_receive->identity);

					// cycle through all io_handlers
					for(int i = 0; i < NUMBER_HANDLERS; i++){

						// lock handler for access and potential send
						pthread_mutex_lock(com->com_array[i].handler_lock);

						// check if io_handler is in use
						if(com->com_array[i].handler != NULL){

							// Don't send PJOIN to the joining client, though for testing at the moment
							// we send anyway
							//if(i != com->thread_id){
								com->com_array[i].handler->send_pdu(com->com_array[i].handler, pdu_response);
								printf("\nsent on %d\n", i);
							//}

						}
						pthread_mutex_unlock(com->com_array[i].handler_lock);
					}
					pdu_response->free_pdu(pdu_response);

				} else if(pdu_receive->type == 12 && com->joined == 1){
					printf("client sends second join while already joined\n");
				}
				// here we check for QUIT message
				// todo
				// remove client identity from list

				// here we check for MESS message
				// todo

				pdu_receive->free_pdu(pdu_receive);
				pdu_receive = NULL;



			} else if (com->handler->status != STATUS_RECEIVE_EMPTY){
				// here we handle connections that were
				// terminated wihtout notice
				printf("We shoudld probably shut this one down\n");
				com->handler->close(com->handler);
				pthread_mutex_lock(com->handler_lock);
				free_tcp_server_communicator(com->handler);
				com->handler = NULL;
				pthread_mutex_unlock(com->handler_lock);
				com->joined = 0;
			}

		}

	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

    return NULL;
}




