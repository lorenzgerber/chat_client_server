/*
 * socket_templates.h
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#ifndef SRC_NETCOM_SOCKET_TEMPLATES_H_
#define SRC_NETCOM_SOCKET_TEMPLATES_H_

#define ENTITY_SERVER 0
#define ENTITY_CLIENT 1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include "message_byte_array.h"

typedef struct io_handler {

	struct message_byte_array *buffer;
	int socket;
	int read_head;
	int socket_entity;

	// function: connect_socket

	// function: request_n_word
	uint8_t* (*request_n_word)(struct io_handler *self, int n_word);


	// function: send_n_word

	// function: listen

	// function free/close

} io_handler;


#endif /* SRC_NETCOM_SOCKET_TEMPLATES_H_ */
