/*
 * socket_creator.h
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
#ifndef SRC_NETCOM_SOCKET_CREATOR_H_
#define SRC_NETCOM_SOCKET_CREATOR_H_

#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>

#include "socket_templates.h"
#include "dummy_sockets.h"
#include "tcp_socket.h"
#include "udp_socket.h"
#include "pdu_parser.h"

io_handler* create_tcp_client_communicator(char* server_name, int port);
int free_tcp_client_communicator(io_handler* self);
int tcp_client_connect(struct io_handler *self, int n_times);

io_handler* create_tcp_server_listener(char *server_name,  uint16_t port);
io_handler* tcp_server_listen(struct io_handler *self);
int close_tcp_socket_listener(struct io_handler *self);
int free_tcp_server_listener(io_handler *self);

io_handler* create_tcp_server_communicator(int *sfd_read_write);
int free_tcp_server_communicator(io_handler* self);
int tcp_server_send_pdu(struct io_handler *self, pdu *pdu);
int close_tcp_socket_communicator(struct io_handler *self);

int tcp_request_n_word(struct io_handler *self, int n_word);
int tcp_send_pdu(struct io_handler *self, pdu* pdu);


io_handler* create_udp_communicator(char* server_name, int port);
int udp_connect(struct io_handler *self, int n_times);

int udp_request_n_word(struct io_handler *self, int n_word);
int udp_send_pdu(struct io_handler *self, pdu* pdu);
int close_udp_socket(struct io_handler *self);

int dummy_socket_request_n_word(struct io_handler *self, int n_word);
io_handler* create_dummy_socket(int op_code, int socket_entity);

#endif /* SRC_NETCOM_SOCKET_CREATOR_H_ */
