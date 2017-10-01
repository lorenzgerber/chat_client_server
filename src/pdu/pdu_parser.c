/*
 * pdu_parser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#include "pdu_parser.h"

int parse_header(uint8_t* buffer){

	int op_code = buffer[0];

	switch (op_code) {
	case PDU_REG:
		parse_REG(buffer);
		break;
	case PDU_MESS:
		parse_MESS(buffer);
		break;
	default:
		break;
	}
	printf("opcode = %d\n", buffer[0]);
	return 0;
}

int parse_REG(uint8_t *buffer){

	return 0;

}

int parse_MESS(uint8_t *buffer){

	//Check padding
	if (buffer[1]!=0){
		printf("missing padding in potential MESS\n");
	}

	pdu_MESS* pdu = create_MESS(buffer[2], buffer[3]);

	//request_one_word();



	free_pdu_mess(pdu);


	return 0;

}

