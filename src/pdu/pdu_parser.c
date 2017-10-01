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

	switch(op_code){
		case PDU_MESS:
			parse_MESS(socket, read_position);
			break;
	}

	printf("op_code %d\n", op_code);

	return 0;
}

int parse_REG(struct io_handler* socket){

	return 0;

}

int parse_MESS(struct io_handler* socket, uint8_t* read_position){

	// build step by step a MESS pdu
	uint8_t identity_length = *(read_position+2);
	uint8_t checksum = *(read_position+3);

	printf("identity length %d\n", identity_length);
	printf("checksum %d\n", checksum);

	read_position = socket->request_n_word(socket, 1);

	// this code assembles uint16_t from two uint8_t. Here we could use the
	// htons or whater the shit is called. Then we would only do the bitshift without
	// flipping the positions first, and let htons shit decide if it needs to be flipped
	// on the current platform.
	uint16_t message_length = ((uint16_t)*(read_position+1) << 8) | *read_position;

	printf("message_length: %d\n", message_length);
	//pdu_MESS *MESS = create_MESS(identity_length, checksum);


	//MESS->add_message(MESS, 13, 1505933137, "Test Message.");
	//MESS->add_client_identity(MESS, "identity");



	return 0;

}

