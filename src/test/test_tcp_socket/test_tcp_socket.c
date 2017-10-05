/*
 * test_socket.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcp_socket.h"
#include "socket_creator.h"
#include "socket_templates.h"
#include "pdu_parser.h"
#include "pdu_creator.h"

void *client(void* data);

int main(int argc, char*argv[]){

	pthread_t* thread_handle;
	thread_handle = malloc(sizeof(pthread_t));

	char* address = "localhost";
	io_handler *server_listener;
	io_handler *server_com;


	pthread_create(thread_handle, NULL, client, NULL);

	server_listener = create_tcp_server_listener(address, 2000);

	server_com = server_listener->listen(server_listener);
	if(server_com != NULL){
		printf("tcp server connected to client\n");
	}

	pdu* ack = parse_header(server_com);

	// receiving and printing of message from client
	printf("\nACK pdu from dummy\n");
	printf("op code: %d\n", ack->type);
	printf("identity nr: %d\n", ack->id_number);
	ack->free_pdu(ack);


	pdu *participants = create_participants(3, 15);
	participants->add_identities(participants, "partic\0ipa\0nts\0");

	server_com->send_pdu(server_com, participants);

	pthread_join(*thread_handle, NULL);
	free(thread_handle);

	return 0;
}



void * client(void* data){
	char* address = "localhost";

	io_handler *client;
	pdu *test = create_ack(1234);

	client = create_tcp_client_communicator(address, 2000);
	client->connect(client, 5);
	client->send_pdu(client, test);

	pdu *participants = parse_header(client);

	printf("\nPARTICIPANTS pdu from dummy\n");
	printf("op code: %d\n", participants->type);
	printf("nr of identities: %d\n", participants->number_identities);
	printf("identities length: %d\n", participants->length);
	for(int i = 0; i < participants->number_identities; i++){
		printf("Identity %d: %s\n",i+1,participants->identities[i]);
	}
	participants->free_pdu(participants);

	return NULL;
}


