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


int main(int argc, char*argv[]){

	printf("socket testing\n");
	uint8_t address[4] = {127, 0, 0, 1};

	io_handler *handler = create_client_tcp_socket(address, 2000);


	return 0;
}

