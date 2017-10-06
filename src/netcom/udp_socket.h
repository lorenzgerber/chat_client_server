/*
 * udp_socket.h
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
#ifndef SRC_NETCOM_UDP_SOCKET_H_
#define SRC_NETCOM_UDP_SOCKET_H_

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "socket_templates.h"

#define UDP_PORT 1337

int setup_udp_send_socket(void);

int setup_listener_socket_udp(int* sfd, io_handler* self);

struct addrinfo *get_udp_server_address(uint16_t* port, char *name);

int connect_to_udp_server(int sock, struct addrinfo *res);

#endif /* SRC_NETCOM_UDP_SOCKET_H_ */



