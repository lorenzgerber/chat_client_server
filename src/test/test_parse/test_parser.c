/*
 * test_parser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pdu_creator.h"
#include "message_byte_array.h"
#include "pdu_serializer.h"
#include "pdu_parser.h"



int main(int argc, char*argv[]){

	pdu_MESS *MESS;
	message_byte_array *MBA;
	uint8_t buffer[65535];
	int length;

	//Create PARTICIPANTS and add some identities
	MESS = create_MESS(8, 99);
	MESS->add_message(MESS, 13, 1505933137, "Test Message.");
	MESS->add_client_identity(MESS, "identity");

	// Create PARTICIPANTS byte stream for send
	MBA = MESS->create_message(MESS);
	length = get_length_MESS(MESS);
	for(int i = 0; i < length; i++){
		buffer[i] = MBA->array[i];
	}


	free_pdu_mess(MESS);
	free_message_byte_array(MBA);

	parse_header(buffer);



	return 0;

}

