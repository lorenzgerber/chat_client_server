/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include "server.h"

int main (int argc, char*argv[]){

	// Check user args

	// Communication Loop with Nameserver
	// needs access to a list of clients
	// loops as long as the server is running
	// implement in a separate thread
	//
	// Send
	// non-blocking receive with a time limit
	// after time limit re-send

	// Communicate with Clients
	// Main loop
	// =========
	// - listen for new clients
	// - listen or new messages from clients
	// - distribute messages
	// needs an array for io_handlers of all clients
	// needs a message queue of messages to be distributed


	printf("I'm the super chat server\n");
	return 0;
}
