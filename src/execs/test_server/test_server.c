/*
 * test_server.c
 *
 *  Created on: Oct 14, 2017
 *      Author: lgerber
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "tcp_socket.h"
#include "socket_creator.h"
#include "socket_templates.h"
#include "pdu_parser.h"
#include "pdu_creator.h"


int main(int argc, char*argv[]){

	char* address = "localhost";

	io_handler *client;
	pdu *test = create_ack(1234);

	client = create_tcp_client_communicator(address, 2000);
	client->connect(client, 50);
	client->send_pdu(client, test);
	test->free_pdu(test);



	pdu* participants = NULL;
	// looping until an error happens or we get data
	while(client->status == STATUS_RECEIVE_EMPTY || client->status == 0){
		participants = parse_header(client);
	}
	if(client->status != STATUS_RECEIVE_OK){
		printf("something wrong with receive in Client\n");
	}
	client->status = 0;

	participants->print(participants);
	free_participants(participants);
	printf("----\n");

	// close connection
	client->close(client);
	free_tcp_client_communicator(client);

    return 0;
}
