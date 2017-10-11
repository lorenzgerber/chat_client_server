/*
 * socket_templates.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef SRC_NETCOM_SOCKET_TEMPLATES_H_
#define SRC_NETCOM_SOCKET_TEMPLATES_H_

#define ENTITY_SERVER 0
#define ENTITY_CLIENT 1

#define STATUS_RECEIVE_EMPTY 1
#define STATUS_RECEIVE_ERROR 2
#define STATUS_RECEIVE_OK 3

#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "message_byte_array.h"

/**
 * struct io_handler
 *
 * Container object for network io.
 * It represents the flexible socket
 * abstraction that can be configured
 * by constructor  methods to tcp / udp
 * communicator client / server,
 * or tcp listener.
 * Further, it can also be configured
 * as dummy socket for testing of
 * the parser functionality.
 */
typedef struct io_handler {

	struct message_byte_array *buffer;
	uint8_t *read_buffer;
	uint8_t recv_length;
	int status;

	int sfd_read_write;
	int sfd_listen;

	uint8_t *read_head;
	uint8_t *read_next;
	int socket_entity;
	struct addrinfo *hints;

	// function: connect_socket
	int (*connect)(struct io_handler *self, int n_times);

	// function: request_n_word
	int (*request_n_word)(struct io_handler *self, int n_word);

	// function: send_n_word
	int (*send_pdu)(struct io_handler *self, pdu *pdu);

	// function: listen
	struct io_handler* (*listen)(struct io_handler *self);

	// function free/close
	int (*close)(struct io_handler *self);


} io_handler;


#endif /* SRC_NETCOM_SOCKET_TEMPLATES_H_ */
