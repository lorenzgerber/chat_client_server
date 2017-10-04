/*
 * socket_creator.c
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#include "socket_creator.h"


/**
 * create_client_tcp_socket
 *
 * Constructor for a client tcp socket. The
 * function creates an io_handler struct, configures
 * the member variables and registers the relevant
 * functions. The function also performs the
 * address lookup with the provided arguments
 * server_name and port.
 */
io_handler* create_client_tcp_socket(char *server_name, int port){
	io_handler *io = malloc(sizeof(io_handler));

	io->socket_entity = ENTITY_CLIENT;

	// Register functions
	io->send_pdu = tcp_client_send_pdu;
	io->connect = tcp_client_connect;
	io->sfd_read_write = setup_tcp_send_socket();

	// get server address
	io->hints = get_tcp_server_address(&port, server_name);

	return io;
}

io_handler* create_client_udp_socket(char* server_name, uint16_t port){
	
    io_handler *io = malloc(sizeof(io_handler));
    
	io->socket_entity = ENTITY_SERVER;
    io->send_pdu = udp_send_pdu;
    io->socket_entity = setup_udp_send_socket();

    io->hints = get_udp_server_address(server_name, port);
    connect_to_udp_server(io->socket_entity, io->hints);

    return io;
}



/**
 * tcp_client_connect
 *
 * Function to be registered in the client tcp io handler.
 * On calling, the function will try n times to connect
 * with the address information available in the io handler
 * struct.
 */
int tcp_client_connect(struct io_handler *self, int n_times){

	int status = -1;
	int counter = n_times;

	while(status == -1 && counter > 0){
		printf("Trying to connect...\n");
		status = connect_to_tcp_server(self->sfd_read_write, self->hints);
		sleep(1);
		counter--;
	}
	if(status == 0){
		printf("connected...\n");
	}

	return status;
}

/**
 * tcp_client_send_pdu
 *
 * Function to be registered in the tcp client io handler.
 * It takes a pdu struct as argument and tries to send
 * the serialized message on the io handler socket.
 */
int tcp_client_send_pdu(struct io_handler *self, pdu* pdu){

	message_byte_array *MBA = pdu->create_message(pdu);

	if(sendto(self->sfd_read_write, MBA->array, pdu->get_message_length(pdu), 0, (struct sockaddr *)self->hints->ai_addr, self->hints->ai_addrlen)<0){
		fprintf(stderr, "sendto failed\n");
	}

	return 0;
}

io_handler* create_listen_udp_socket(char *server_name, uint16_t port){

	io_handler *io = malloc(sizeof(io_handler));

	io->socket_entity = ENTITY_SERVER;

	// Register functions
	//io->listen = tcp_server_listen;
	setup_listener_socket_udp(&io->sfd_listen, port);

	return io;
}
	
io_handler* create_server_tcp_socket(char *server_name, uint16_t port){
	io_handler *io = malloc(sizeof(io_handler));

	io->socket_entity = ENTITY_SERVER;

	// Register functions
	io->listen = tcp_server_listen;
	setup_listener_socket(&io->sfd_listen, &port);

	return io;
}

int tcp_server_listen(struct io_handler *self){

	listen_obtain_client_socket(&self->sfd_listen, &self->sfd_read_write);

	return 0;
}


int udp_send_pdu(struct io_handler *self, pdu* pdu){
    message_byte_array *MBA = pdu->create_message(pdu);

    if(sendto(self->socket_entity, MBA->array, pdu->get_message_length(pdu), 0, (struct sockaddr *)self->hints->ai_addr, self->hints->ai_addrlen)<0){
        fprintf(stderr, "sendto failed\n");
    }

    return 0;
}




/**
 * Function read from dummy socket
 *
 * This function is registered in create_dummy_socket
 *
 */
uint8_t* dummy_socket_request_n_word(struct io_handler *self, int n_word){

	uint8_t* next_read = &self->buffer->array[self->read_head];
	self->read_head += n_word * 4;

	return next_read;
}

/**
 * create_dummy_socket
 *
 * This method is used for testing. It creates a socket
 * mock to test the parser functionality.
 */
io_handler* create_dummy_socket(int op_code, int socket_entity){
	io_handler *dummy_socket = malloc(sizeof(io_handler));
	dummy_socket->read_head = 0;
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
