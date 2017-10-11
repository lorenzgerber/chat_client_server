/*
 * udp_socket.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "udp_socket.h"

/**
 * setup_udp_socket
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. Function that sets
 * up a socket for udp communication
 * operations.
 * @return socket file descriptor
 */
int setup_udp_socket(void){
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP)) == -1)
        fprintf(stderr, "socket");
    return sock;
}
int setup_udp_send_socket(void){
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        fprintf(stderr, "socket");
    return sock;
}
/**
 * get_udp_server_address
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. This function manages
 * addrinfo structs to obtain the
 * correct connection parameters
 * for the provided port and host
 * name.
 * @param port number to be queried
 * @param name host name to be queried
 * @return addrinfo struct with the
 * connection parameters
 */
struct addrinfo* get_udp_server_address(int* port, char *name){
    int status;
    struct addrinfo hints;
    struct addrinfo *res;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;

    char send_port[5];
    sprintf(send_port, "%d", *port);

    if ((status = getaddrinfo(name, send_port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    return res;
}

/**
 * connect_to_udp_server
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. This function connects
 * to a udp server using the provided
 * socket and the server address
 * parameters in addrinfo.
 * @param sock local udp send socket
 * @param res addrinfo data container
 * @return status
 */
int connect_to_udp_server(int sock, struct addrinfo *res){

	if(connect(sock, res->ai_addr, res->ai_addrlen) < 0)
		return -1;
	return 0;
}
