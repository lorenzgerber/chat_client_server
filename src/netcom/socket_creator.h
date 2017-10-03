/*
 * socket_creator.h
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#ifndef SRC_NETCOM_SOCKET_CREATOR_H_
#define SRC_NETCOM_SOCKET_CREATOR_H_

#include <stdio.h>
#include <sys/socket.h>
#include "socket_templates.h"
#include "dummy_sockets.h"

io_handler* create_dummy_socket(int op_code, int socket_entity);

io_handler* create_client_tcp_socket(uint8_t address_server[4], uint16_t port);

int tcp_client_send_n_word(uint16_t n, uint8_t word);

uint8_t* dummy_socket_request_n_word(struct io_handler *self, int n_word);


#endif /* SRC_NETCOM_SOCKET_CREATOR_H_ */
