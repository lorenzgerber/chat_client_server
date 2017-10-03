/*
 * socket_creator.c
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#include "socket_creator.h"

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

io_handler* create_client_tcp_socket(char *server_name, uint16_t port){
	io_handler *io = malloc(sizeof(io_handler));
	io->socket_entity = ENTITY_CLIENT;
	io->send_n_word = tcp_client_send_n_word;
	socket = setup_tcp_send_socket();
	io->hints = get_tcp_server_address(port, server_name);
	connect_to_tcp_server(io->socket, io->hints);

	return io;
}

int tcp_client_send_n_word(struct io_handler *self, pdu* pdu){

	message_byte_array *MBA = pdu->create_message();
	MBA->array;

	if(sendto(self->socket, MBA->array, n*8, 0, (struct sockaddr *)self->hints->ai_addr, self->hints->ai_addrlen)<0){
		fprintf(stderr, "sendto failed\n");
	}

	return 0;
}


/**
 * Function read from dummy socket
 *
 * This function is registered in creat_dummy_socket
 *
 */
uint8_t* dummy_socket_request_n_word(struct io_handler *self, int n_word){

	uint8_t* next_read = &self->buffer->array[self->read_head];
	self->read_head += n_word * 4;

	return next_read;
}
