/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <socket_templates.h>
#include <socket_creator.h>

int main (int argc, char*argv[]){

	printf("I'm the super chat client\n");

	io_handler* client = create_tcp_client_communicator("itchy.cs.umu.se", 1337);
	client->connect(client, 5);
	client->status = 0;

	pdu *getlist = create_getlist();

	client->send_pdu(client, getlist);
	getlist->free_pdu(getlist);

	pdu* slist;
	// looping until an error happens or we get data
	while(client->status == STATUS_RECEIVE_EMPTY || client->status == 0){
		slist = parse_header(client);
	}
	if(client->status != STATUS_RECEIVE_OK){
		printf("something wrong with receive in Client\n");
	}else{
		client->status = 0;

		slist->print(slist);
		slist->free_pdu(slist);
	}

}
