/*
 * udp_socket.c
 *
 *  Created on: Oct 2, 2017
 *      Author: lgerber
 */
#include "udp_socket.h"

int setup_udp_send_socket(){
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		fprintf(stderr, "socket");
	return sock;
}

struct addrinfo *get_udp_server_address(char* name, uint16_t port){
	struct addrinfo *res;
	struct addrinfo hints;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = 0;
		hints.ai_flags = AI_ADDRCONFIG;

		//Translate server port to char
		char send_port[5];
			sprintf(send_port, "%d", port);

		/* Build the network address of server */
		getaddrinfo(name, send_port, &hints, &res);

	return res;
}

int connect_to_udp_server(int sock, struct addrinfo *res){

	if(connect(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen) < 0)
		return -1;
	return 0;
}
