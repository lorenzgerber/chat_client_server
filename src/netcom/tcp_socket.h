/*
 * tcp_socket.h
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
#ifndef SRC_NETCOM_TCP_SOCKET_H_
#define SRC_NETCOM_TCP_SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "socket_templates.h"

int setup_listener_socket(int *sfd, uint16_t *port);

int listen_obtain_client_socket(int *sfd_listen, int *srd_read_write);

int setup_tcp_send_socket(void);

struct addrinfo* get_tcp_server_address(int *port, char *name);

int connect_to_tcp_server(int sock, struct addrinfo *res);


#endif /* SRC_NETCOM_TCP_SOCKET_H_ */
