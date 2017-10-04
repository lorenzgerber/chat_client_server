/*
 * udp_socket.h
 *
 *  Created on: Oct 2, 2017
 *      Author: lgerber
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
#include <unistd.h>
#include <sys/select.h>

int setup_udp_send_socket();

struct addrinfo *get_udp_server_address(char* name, uint16_t port);

int connect_to_udp_server(int sock, struct addrinfo *res);

#endif /* SRC_NETCOM_UDP_SOCKET_H_ */
