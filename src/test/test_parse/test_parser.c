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
#include "socket_templates.h"
#include "socket_creator.h"
#include "dummy_sockets.h"



int main(int argc, char*argv[]){

	io_handler *dummy_socket;
	dummy_socket = create_dummy_socket(PDU_MESS, ENTITY_SERVER);
	parse_header(dummy_socket);



	return 0;

}

