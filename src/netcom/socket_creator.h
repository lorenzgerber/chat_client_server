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
#include "udp_socket.h"

io_handler* create_tcp_client_communicator(char* server_name, int port);

int tcp_client_connect(struct io_handler *self, int n_times);

int tcp_client_send_pdu(struct io_handler *self, pdu *pdu);

io_handler* create_tcp_server_listener(char *server_name,  uint16_t port);

io_handler* tcp_server_listen(struct io_handler *self);

io_handler* create_tcp_server_communicator(int *sfd_read_write);

int tcp_server_send_pdu(struct io_handler *self, pdu *pdu);

uint8_t *tcp_server_request_n_word(struct io_handler *self, int n_word);

io_handler* create_client_udp_socket(char* server_name, uint16_t port);

io_handler* create_listen_udp_socket(char *server_name, uint16_t port);

io_handler* udp_server_listen(struct io_handler *self);

int udp_send_pdu(struct io_handler *self, pdu* pdu);

uint8_t* dummy_socket_request_n_word(struct io_handler *self, int n_word);

io_handler* create_dummy_socket(int op_code, int socket_entity);

#endif /* SRC_NETCOM_SOCKET_CREATOR_H_ */
