/*
 * udp_socket.c
 *
 *  Created on: Oct 2, 2017
 *      Author: lgerber
 */

#include "udp_socket.h"


int setup_listener_socket_udp(int* sfd, io_handler* self){

    if ((*sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        perror("socket");
    if(bind(*sfd, self->hints->ai_addr,self->hints->ai_addrlen)<0){
        perror("bind");
    }
    return *sfd;
}

int setup_udp_send_socket(){
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1){
		perror("socket");
	}

	return sock;
}

struct addrinfo* get_udp_server_address(int *port, char *name){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    char send_port[5];
    sprintf(send_port, "%d", *port);

    if ((status = getaddrinfo(name, send_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    return servinfo;
}

int connect_to_udp_server(int sock, struct addrinfo *res){

	if(connect(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen) < 0)
		return -1;
	return 0;
}
