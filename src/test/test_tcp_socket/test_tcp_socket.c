/*
 * test_socket.c
 *
 *  Created on: Oct 3, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcp_socket.h"
#include "socket_creator.h"
#include "socket_templates.h"
#include "pdu_creator.h"

void *client(void* data);

int main(int argc, char*argv[]){

	pthread_t* thread_handle;
	thread_handle = malloc(sizeof(pthread_t));

	char* address = "hplinuxbox";
	io_handler *server_listener;
	io_handler *server_com;


	pthread_create(thread_handle, NULL, client, NULL);

	server_listener = create_tcp_server_listener(address, 2000);

	server_com = server_listener->listen(server_listener);
	if(server_com != NULL){
		printf("tcp server connected to client\n");
	}

	pthread_join(*thread_handle, NULL);
	free(thread_handle);

	return 0;
}



void * client(void* data){
	char* address = "hplinuxbox";

	io_handler *client;
	pdu *test = create_ack(1234);

	client = create_tcp_client_communicator(address, 2000);
	client->connect(client, 5);
	client->send_pdu(client, test);

	return NULL;
}


