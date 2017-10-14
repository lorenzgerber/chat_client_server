/*
 * socket_creator.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "socket_creator.h"

int move_to_process_buffer(struct io_handler *handler, int n_word);

/**
 * create_client_tcp_socket
 *
 * Constructor for a client tcp socket. The
 * function creates an io_handler struct, configures
 * the member variables and registers the relevant
 * functions. The function also performs the
 * address lookup with the provided arguments
 * server_name and port.
 *
 * Creating the tcp_client_communicator will
 * also try to obtain the address details for
 * the server to connect to and setup the
 * communication socket.
 *
 * @param server_name host name of server to connect to
 * @param port number of port to connect to
 * @return io_handler configured as tcp client communicator
 */
io_handler* create_tcp_client_communicator(char *server_name, int port){
    io_handler *io = malloc(sizeof(io_handler));
    io->read_buffer = malloc(sizeof(uint8_t)*131072);
    io->recv_length = 0;
    io->buffer = NULL;
    io->socket_entity = ENTITY_CLIENT;
    io->status = STATUS_RECEIVE_EMPTY;

    // Register functions
    io->connect = tcp_client_connect;
    io->send_pdu = tcp_send_pdu;
    io->request_n_word = tcp_request_n_word;
    io->close = close_tcp_socket_communicator;

    io->sfd_read_write = setup_tcp_send_socket();

    // get server address
    io->hints = get_tcp_server_address(&port, server_name);

    return io;
}

int free_tcp_client_communicator(io_handler* self){
	free(self->read_buffer);
	freeaddrinfo(self->hints);
	if(self->buffer != NULL){
		free_message_byte_array(self->buffer);
	}
	free(self);
	return(0);
}

/**
 * tcp_client_connect
 *
 * Function to be registered in the client tcp io handler.
 * On calling, the function will try n times to connect
 * with the address information available in the io handler
 * struct.
 * @self self reference
 * @n_times how many times should we try to connect.
 * @return int status for connecting attempt
 */
int tcp_client_connect(struct io_handler *self, int n_times){

    int status = -1;
    int counter = n_times;

    while(status == -1 && counter > 0){
        printf("tcp client tries connecting to server...\n");
        status = connect_to_tcp_server(self->sfd_read_write, self->hints);
        sleep(1);
        counter--;
    }
    if(status == 0){
        printf("tcp client connected to server...\n");
    }

    return status;
}


/**
 * tcp_send_pdu
 *
 * Function to be registered in communication io_handler.
 * It takes a pdu struct as argument and tries to send
 * the serialized message on the io handler socket. Prior
 * to use, the client needs to connect.
 * @param self reference to io_handler for accessing member variables
 * @param pdu pointer to pdu to be sent.
 * @return status of send attempt.
 */
int tcp_send_pdu(struct io_handler *self, pdu* pdu){

    message_byte_array *MBA = pdu->create_message(pdu);

    if(send(self->sfd_read_write, MBA->array, pdu->get_message_length(pdu), MSG_NOSIGNAL)<0){
        fprintf(stderr, "send failed\n");
    }
    free_message_byte_array(MBA);

    return 0;
}

/**
 * tcp_request_n_word
 *
 * Function to be registered in communication io_handler.
 * It will obtain n words either directly from existing
 * data in the read buffer, alternatively, it will first
 * try to receive data to the read buffer from the configured
 * communication socket of self.
 * @param self reference to io_handler for accessing member variables
 * @param n_word number of word (32 Bytes) to be obtained.
 * @return int return status
 */
int tcp_request_n_word(struct io_handler *self, int n_word){

	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int got_data;
	self->status = 0;

	FD_ZERO(&readfds);
	FD_SET(self->sfd_read_write, &readfds);
	got_data = select(self->sfd_read_write+1, &readfds, NULL, NULL, &tv);



    int nread = 0;
    if(self->buffer != NULL){
        free_message_byte_array(self->buffer);
        self->buffer = NULL;
    }

    if(self->recv_length >= n_word*4){
        move_to_process_buffer(self, n_word);
        self->status = STATUS_RECEIVE_OK;
        return self->status;
    }

    if(got_data){

    	nread = recv(self->sfd_read_write, self->read_buffer, sizeof(uint8_t)*131072, 0);

    	if(nread == 0 || nread == -1){
			self->status = STATUS_RECEIVE_ERROR;
			return self->status;
		} else if (nread > 0){
			self->recv_length+=nread;
			if(self->recv_length >= n_word*4){
				move_to_process_buffer(self, n_word);
			}

			self->status = STATUS_RECEIVE_OK;
		}

    } else {
    	self->status = STATUS_RECEIVE_EMPTY;
    }

    return self->status;
}

/**
 * create_tcp_server_listener
 *
 * Constructor for a TCP listener io_handler.
 * It will set up a port to listen for incoming traffic
 * on the given hostname.
 * @param self reference to io_handler for accessing member variables
 * @param server_name name of local host
 * @param port number to listen
 * @return io_handler configured TCP listener
 */
io_handler* create_tcp_server_listener(char *server_name, uint16_t port){
    io_handler *io = malloc(sizeof(io_handler));

    io->socket_entity = ENTITY_SERVER;
    io->status = STATUS_RECEIVE_EMPTY;
    io->buffer = NULL;
    // Register functions
    io->listen = tcp_server_listen;
    io->close = close_tcp_socket_listener;

    setup_listener_socket(&io->sfd_listen, &port);

    return io;
}

int free_tcp_server_listener(io_handler* self){
	free(self);
	return(0);
}

/**
 * tcp_server_listen
 *
 * Function to be registered in the tcp listner io_handler.
 * On invocation, this function starts listening on
 * the determined port. If a client tries to connect,
 * a new io_handler tcp communication struct is constructed
 * and returned.
 * @param self reference to io_handler for acessing member variables
 * @return io_handler configured and connected tcp communicator
 */
io_handler* tcp_server_listen(struct io_handler *self){

    io_handler *com;

    int status = 0;
    status = listen_obtain_client_socket(&self->sfd_listen, &self->sfd_read_write);
    if(status == 0){
        int *sfd_read_write = malloc(sizeof(int));
        *sfd_read_write = self->sfd_read_write;
        com = create_tcp_server_communicator(sfd_read_write);
        free(sfd_read_write);
    }

    return com;
}

/**
 * create_tcp_server_communicator
 *
 * Constructor for tcp communicator io_handler. This
 * struct is constructed from the tcp server listener
 * when a client connects.
 * @param sfd_read_write socket file descriptor to use for communication
 * @return io_handler configured as tcp communicator
 */
io_handler* create_tcp_server_communicator(int *sfd_read_write){
    io_handler *io = malloc(sizeof(io_handler));
    io->socket_entity = ENTITY_SERVER;
    io->read_buffer = malloc(sizeof(uint8_t)*131072);
    io->recv_length = 0;
    io->sfd_read_write = *sfd_read_write;
    io->status = STATUS_RECEIVE_EMPTY;
    io->buffer = NULL;

    io->request_n_word = tcp_request_n_word;
    io->send_pdu = tcp_send_pdu;
    io->close = close_tcp_socket_communicator;

    return io;
}

int free_tcp_server_communicator(io_handler* self){
	free(self->read_buffer);
	free(self);
	return(0);
}

int close_tcp_socket_listener(struct io_handler *self){
	int status = 0;
	status = shutdown(self->sfd_listen, SHUT_RDWR);
	if(status == 0){
		status = close(self->sfd_listen);
	}

	return status;
}


int close_tcp_socket_communicator(struct io_handler *self){

	int status = 0;
	status = shutdown(self->sfd_read_write, SHUT_RDWR);
	if(status == 0){
		status = close(self->sfd_read_write);
	}

	return status;
}

/**
 * move_to_process_buffer
 *
 * Helper function for communicators that
 * moves data from the tcp read buffer to the
 * message byte array where it can be accessed
 * by the parsers.
 * @param io_handler to process
 * @param n_word numbe of words (32 Bytes) to shift
 * @return status
 */
int move_to_process_buffer(struct io_handler *handler, int n_word){
    size_t length = n_word *4;
    handler->buffer = create_message_byte_array(n_word);
    memcpy(handler->buffer->array, handler->read_buffer, length);
    handler->read_head = handler->buffer->array;
    memmove(handler->read_buffer, handler->read_buffer+length, handler->recv_length - length);
    handler->recv_length -= length;

    return 0;
}

/**
 * create_udp_communicator
 *
 * Constructor for a udp socket. The
 * function creates an io_handler struct, configures
 * the member variables and registers the relevant
 * functions. The function also performs the
 * address lookup with the provided arguments
 * server_name and port.
 *
 * Creating the udp_communicator will
 * also try to obtain the address details for
 * the server to connect to and setup the
 * communication socket.
 *
 * @param server_name host name of server to connect to
 * @param port number of port to connect to
 * @return io_handler configured as tcp client communicator
 */
io_handler* create_udp_communicator(char* server_name, int port){

    io_handler *io = malloc(sizeof(io_handler));
    io->read_buffer = malloc(sizeof(uint8_t)*131072);
    io->recv_length = 0;
    io->buffer = NULL;

    io->socket_entity = ENTITY_SERVER;

    // Register functions
    io->sfd_read_write = setup_udp_socket();
    io->connect = udp_connect;
    io->send_pdu = udp_send_pdu;
    io->request_n_word = udp_request_n_word;
    io->close = close_udp_socket;

    // get server address
    io->hints = get_udp_server_address(&port, server_name);

    return io;
}

/**
 * udp_send_pdu
 *
 * Function to be registered in communication io_handler.
 * It takes a pdu struct as argument and tries to send
 * the serialized message on the io handler socket. Prior
 * to use, the client needs to connect.
 *
 * @param self reference to io_handler for accessing member variables
 * @param pdu pointer to pdu to be sent.
 * @return status of send attempt.
 */
int udp_send_pdu(struct io_handler *self, pdu* pdu){
    message_byte_array *MBA = pdu->create_message(pdu);

    if(send(self->sfd_read_write, MBA->array, pdu->get_message_length(pdu), 0)<0){
        fprintf(stderr, "send failed\n");
    }
    free_message_byte_array(MBA);

    return 0;
}

/**
 * udp_client_connect
 *
 * Function to be registered in the client udp io handler.
 * On calling, the function will try n times to connect
 * with the address information available in the io handler
 * struct.
 *
 * @self self reference
 * @n_times how many times should we try to connect.
 * @return int status for connecting attempt
 */
int udp_connect(struct io_handler *self, int n_times){

    int status = -1;
    int counter = n_times;

    while(status == -1 && counter > 0){
        printf("udp client tries connecting to server...\n");
        status = connect_to_udp_server(self->sfd_read_write, self->hints);
        sleep(1);
        counter--;
    }
    if(status == 0){
        printf("udp client connected to server...\n");
    }

    return status;
}

/**
 * udp_request_n_word
 *
 * Function to be registered in communication io_handler.
 * It will obtain n words either directly from existing
 * data in the read buffer, alternatively, it will first
 * try to receive data to the read buffer from the configured
 * communication socket of self.
 *
 * @param self reference to io_handler for accessing member variables
 * @param n_word number of word (32 Bytes) to be obtained.
 * @return int return status
 */
int udp_request_n_word(struct io_handler *self, int n_word){

    //status holders
    int nread = 0;
    int got_data;
    self->status = 0;

    //select() variables
    struct timeval tv;
    tv.tv_sec = 4;
    tv.tv_usec = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(self->sfd_read_write, &readfds);

    //source address struct
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);

    if(self->buffer != NULL){
        free_message_byte_array(self->buffer);
    }else{

    }

    if(self->recv_length >= n_word*4){
        move_to_process_buffer(self, n_word);
        self->status = STATUS_RECEIVE_OK;
        return self->status;
    }

    //Read from socket using select
    // wait until either socket has data ready to be recv()d (timeout 1 secs)
    got_data = select(self->sfd_read_write + 1, &readfds, NULL, NULL, &tv);

    if (got_data) {

        // one or both of the descriptors have data
        nread = (int) recvfrom(self->sfd_read_write ,
                               self->read_buffer,
                               sizeof(uint8_t)*131072,
                               0,
                               (struct sockaddr *) &si_other,
                               (socklen_t *) &slen);
        if(nread == 0 || nread == -1){
            self->status = STATUS_RECEIVE_ERROR;
            return self->status;
        } else if (nread > 0){
            self->recv_length+=nread;
            if(self->recv_length >= n_word*4){
                move_to_process_buffer(self, n_word);
            }

            self->status = STATUS_RECEIVE_OK;
        }
    }else {
        self->status = STATUS_RECEIVE_EMPTY;
    }

    return self->status;
}

int close_udp_socket(struct io_handler *self){
    int status = 0;
    status = shutdown(self->sfd_read_write, SHUT_RDWR);
    if(status == 0){
        status = close(self->sfd_read_write);
    }

    return status;
}

/**
 * dummy_socket_request_n_word
 *
 * This function is registered in dummy io_handler
 * It is used to serve data to parsers for testing
 * purposes.
 * @param self reference to access memeber variables
 * @param n_word number of words to obtain from data
 * @return status
 *
 */
int dummy_socket_request_n_word(struct io_handler *self, int n_word){

    if(self->read_head == NULL){
        self->read_head = &self->buffer->array[0];
        self->read_next = self->read_head + n_word *4;
    } else {
        self->read_head = self->read_next;
        self->read_next += n_word * 4;
    }
    self->status = STATUS_RECEIVE_OK;

    return self->status;
}

/**
 * create_dummy_socket
 *
 * This method is used for testing. It creates a socket
 * mock to test the parser functionality.
 * @param op_code of which pdu the io_handler shall provide
 * @param socket_entity either SERVER or CLIENT
 * @return dummy_socket io_handler with pdu data corresponding the
 * provided op_code
 */
io_handler* create_dummy_socket(int op_code, int socket_entity){
    io_handler *dummy_socket = malloc(sizeof(io_handler));
    dummy_socket->read_head = NULL;
    dummy_socket->read_next = NULL;
    dummy_socket->request_n_word = dummy_socket_request_n_word;
    dummy_socket->socket_entity = socket_entity;

    switch(op_code){
        case PDU_ACK:
            return dummy_socket_ack(dummy_socket);
        case PDU_NOTREG:
            return dummy_socket_notreg(dummy_socket);
        case PDU_SLIST:
            return dummy_socket_slist(dummy_socket);
        case PDU_JOIN:
            return dummy_socket_join(dummy_socket);
        case PDU_PARTICIPANTS:
            return dummy_socket_participants(dummy_socket);
        case PDU_QUIT:
            return dummy_socket_quit(dummy_socket);
        case PDU_MESS:
            return dummy_socket_mess(dummy_socket);
        case PDU_PJOIN:
            return  dummy_socket_pjoin(dummy_socket);
        case PDU_PLEAVE:
            return dummy_socket_pleave(dummy_socket);
        default:
            break;
    }

    return dummy_socket;
}





