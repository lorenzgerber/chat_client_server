/*
 * udp_socket.c
 *
 *  Created on: Oct 2, 2017
 *      Author: lgerber
 */
#include <arpa/inet.h>
#include "udp_socket.h"
int setup_listener_socket_udp(int *sfd, uint16_t port){
	if ((*sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		fprintf(stderr, "socket");
	}

	struct sockaddr_in addr;//, from;

	//memset(&addr, 0, sizeof(addr));
    memset((char *) &addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(*sfd, (const struct sockaddr *) &addr, sizeof (addr)) == -1) {
		fprintf(stderr, "bind");
	}
}
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

int udp_listen_obtain_client_socket(const int *sfd_listen, int *sfd_read_write){

    char buf[1024];
    struct sockaddr_in from;
    int slen=sizeof(from);

    printf("waiting\n");
    if (recvfrom(*sfd_listen, buf, 1024, 0, (struct sockaddr *) &from, (socklen_t *) &slen) == -1){
        fprintf(stderr, "receive from");
    }

    printf("Received packet from %s:%d\nData: %s\n\n",
           inet_ntoa(from.sin_addr) , UDP_PORT, buf);
    return 0;

}