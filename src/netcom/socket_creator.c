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
io_handler* create_dummy_socket(int op_code){
	io_handler *dummy_socket = malloc(sizeof(io_handler));
	dummy_socket->request_n_word = dummy_socket_request_n_word;


	return dummy_socket;
}


/**
 * Function read from dummy socket
 *
 * This function is registered in creat_dummy_socket
 *
 */
uint8_t* dummy_socket_request_n_word(struct io_handler *self){

	uint8_t* next_read = &self->buffer.array[0];

	return next_read;
}
