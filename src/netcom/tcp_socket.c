/*
 * tcp_socket.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "tcp_socket.h"

/**
 * setup_listener_socket
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. This function sets
 * up a listner socket on the provided
 * socket file descriptor and port.
 * @param sfd socket file descriptor
 * @param port to listen to
 * @return status of function
 */
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
    	free(listeningPort);
    	fprintf(stderr, "Receiver, could not find host: %s\n", gai_strerror(s));
    	exit(-1);
    }
    free(listeningPort);

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

/**
 * listen_obtain_client_socket
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. This function accepts incoming
 * connections on the listener socket
 * and establishes a communication session
 * on the provided socket file descriptor.
 * @param sfd_listen the listener file descriptor
 * @param sfd_read_write file descriptor variable
 * to establish the communication on.
 * @return status
 *
 */
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

/**
 * setup_tcp_send_socket
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. this function sets up a
 * socket for sending data.
 * @return socket file descriptor of the established
 * communication socket.
 */
int setup_tcp_send_socket(void){
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		perror("socket");
	return sock;
}

/**
 * get_tcp_server_address
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

/**
 * connection_to_tcp_server
 *
 * Socket function wrapper
 * to be used in socket abstraction
 * objects. This function connects
 * to a provided host for a com
 * session over the provided socket
 * connection.
 * @param our communication socket
 * @param addrinfo struct that contains the
 * data where to connect to.
 */
int connect_to_tcp_server(int sock, struct addrinfo *res){

	if(connect(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen) < 0)
		return -1;
	return 0;
}




