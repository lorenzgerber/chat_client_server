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
	pdu *test;

	// establish connection
	client = create_tcp_client_communicator(address, 2000);
	client->connect(client, 50);



	// create message, should not be echoed as we are not joined
	// we get kicked out
	test = create_mess(0, 0);
	test->add_message(test, 13, 1505933137, "Test Message.");
	client->send_pdu(client, test);
	test->free_pdu(test);

	// start again
	client = create_tcp_client_communicator(address, 2000);
	client->connect(client, 50);


	// join chat session, should be echoed
	test = create_join(7);
	test->add_identity(test, "client1");
	client->send_pdu(client, test);
	test->free_pdu(test);


	test = NULL;
	// looping until an error happens or we get data
	while(client->status == STATUS_RECEIVE_EMPTY || client->status == 0){
		test = parse_header(client);
	}
	if(client->status != STATUS_RECEIVE_OK){
		printf("something wrong with receive in Client\n");
	}
	client->status = 0;

	test->print(test);
	test->free_pdu(test);
	printf("----\n");



	// send chat message again, should be echoed
	test = create_mess(0, 0);
	test->add_message(test, 13, 1505933137, "Test Message.");
	test->set_checksum(test);
	client->send_pdu(client, test);
	test->free_pdu(test);
	test = NULL;


	// close connection
	client->close(client);
	free_tcp_client_communicator(client);

    return 0;
}
