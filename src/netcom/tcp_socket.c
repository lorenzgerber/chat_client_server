/*
 * tcp_socket.c
 *
 *  Created on: Oct 2, 2017
 *      Author: lgerber
 */
#include "tcp_socket.h"

int setup_listener_socket(int *sfd, uint16_t *port){

    int s;
    int yes = 1;
    struct addrinfo hints, *result, *rp;

    char *listeningPort = calloc(sizeof(char),16);
    sprintf(listeningPort, "%u", *port);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    /* Get addrinfo for the receiving part */
    s = getaddrinfo(NULL, listeningPort, &hints, &result);
    if (s != 0) {
       fprintf(stderr, "Receiver, could not find host: %s\n", gai_strerror(s));
       exit(-1);
    }

    /* Find correct address and try to bind a socket */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        *sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (*sfd == -1)
            continue;
        if (setsockopt(*sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))==-1) {
            fprintf(stderr, "set sock opt");
            exit(-1);
        }
        if (bind(*sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
       close(*sfd);
    }
    if (rp == NULL) {
        fprintf(stderr,"Receiver, could not bind");
       exit(-1);
    }
    freeaddrinfo(result);

    return 0;
}

int listen_obtain_client_socket(int *sfd_listen, int *sfd_read_write){

    struct sockaddr_in client;
    int c;

    listen(*sfd_listen, 3);

    c = sizeof(struct sockaddr_in);
	*sfd_read_write = accept(*sfd_listen,(struct sockaddr *)&client, (socklen_t*)&c);

	if(sfd_read_write < 0){
		perror("Receiver - accept failed");
		exit(-1);
	}

	return 0;

}

int setup_tcp_send_socket(void){
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		perror("socket");
	return sock;
}

struct addrinfo* get_tcp_server_address(int *port, char *name){


	struct addrinfo *res;
	struct addrinfo hints;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;
		hints.ai_flags = AI_ADDRCONFIG;

		//Translate server port to char
		char send_port[5];
			sprintf(send_port, "%d", *port);

		/* Build the network address of server */
		getaddrinfo(name, send_port, &hints, &res);

	return res;


}

int connect_to_tcp_server(int sock, struct addrinfo *res){

	if(connect(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen) < 0)
		return -1;
	return 0;
}




