/*
 * socket_creator.h
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */



#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef SRC_NETCOM_SOCKET_CREATOR_H_
#define SRC_NETCOM_SOCKET_CREATOR_H_
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "socket_templates.h"
#include "dummy_sockets.h"
#include "tcp_socket.h"



io_handler* create_client_tcp_socket(char* server_name, int port);

int tcp_client_connect(struct io_handler *self, int n_times);

int tcp_client_send_pdu(struct io_handler *self, pdu *pdu);

io_handler* create_server_tcp_socket(char *server_name,  uint16_t port);

int tcp_server_listen(struct io_handler *self);

io_handler* create_dummy_socket(int op_code, int socket_entity);

uint8_t* dummy_socket_request_n_word(struct io_handler *self, int n_word);


#endif /* SRC_NETCOM_SOCKET_CREATOR_H_ */
