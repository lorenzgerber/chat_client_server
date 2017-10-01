/*
 * pdu_parser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#include "pdu_parser.h"

int parse_header(struct io_handler *socket){

	uint8_t op_code;
	uint8_t* read_position;
	read_position = socket->request_n_word(socket, 1);
	op_code = *read_position;
	printf("op_code %d\n", op_code);

	return 0;
}

int parse_REG(struct io_handler* socket){

	return 0;

}

int parse_MESS(struct io_handler* socket){

	return 0;

}

