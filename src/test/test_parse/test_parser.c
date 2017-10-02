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

    io_handler* dummy_socket_ack;
    dummy_socket_ack = create_dummy_socket(PDU_ACK, ENTITY_SERVER);
    parse_header(dummy_socket_ack);

    io_handler* dummy_socket_notreg;
    dummy_socket_notreg = create_dummy_socket(PDU_NOTREG, ENTITY_SERVER);
    parse_header(dummy_socket_notreg);

    io_handler* dummy_socket_slist;
    dummy_socket_slist = create_dummy_socket(PDU_SLIST, ENTITY_CLIENT);
    parse_header(dummy_socket_slist);

    io_handler* dummy_socket_join;
    dummy_socket_join = create_dummy_socket(PDU_JOIN, ENTITY_SERVER);
    parse_header(dummy_socket_join);

    io_handler* dummy_socket_participants;
    dummy_socket_participants = create_dummy_socket(PDU_PARTICIPANTS, ENTITY_CLIENT);
    parse_header(dummy_socket_participants);

    io_handler* dummy_socket_quit;
    dummy_socket_quit = create_dummy_socket(PDU_QUIT, ENTITY_CLIENT);
    parse_header(dummy_socket_quit);

	io_handler *dummy_socket_mess;
    dummy_socket_mess = create_dummy_socket(PDU_MESS, ENTITY_CLIENT);
	parse_header(dummy_socket_mess);

    io_handler* dummy_socket_pjoin;
    dummy_socket_pjoin = create_dummy_socket(PDU_PJOIN, ENTITY_CLIENT);
    parse_header(dummy_socket_pjoin);

    io_handler* dummy_socket_pleave;
    dummy_socket_pleave = create_dummy_socket(PDU_PLEAVE, ENTITY_CLIENT);
    parse_header(dummy_socket_pleave);



	return 0;

}

