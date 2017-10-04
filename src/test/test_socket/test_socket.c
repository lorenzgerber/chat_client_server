/*
 * test_socket.c
 *
 *  Created on: Oct 3, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include "tcp_socket.h"
#include "socket_creator.h"
#include "socket_templates.h"
#include "pdu_creator.h"


int main(int argc, char*argv[]){

	printf("socket testing\n");

	char* address = "hplinuxbox";
	//io_handler *server;
	io_handler *client;
	pdu *test = create_ack(1234);

    //server = create_server_tcp_socket(address, 2000);
	client = create_client_tcp_socket(address, 2000);

	//server->listen(server);

	client->connect(client, 5);
	client->send_pdu(client, test);
    
    
	return 0;
}

