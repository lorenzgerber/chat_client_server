/*
 * test_parser.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
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
    dummy_socket_ack = create_dummy_socket(PDU_ACK, ENTITY_DUMMY);
    pdu* ack = parse_header(dummy_socket_ack);

    printf("\nACK pdu from dummy\n");
    ack->print(ack);
    ack->free_pdu(ack);
    free_message_byte_array(dummy_socket_ack->buffer);
    free(dummy_socket_ack);



    io_handler* dummy_socket_notreg;
    dummy_socket_notreg = create_dummy_socket(PDU_NOTREG, ENTITY_DUMMY);
    pdu* notreg = parse_header(dummy_socket_notreg);

    printf("\nNOTREG pdu from dummy\n");
    notreg->print(notreg);
    notreg->free_pdu(notreg);
    free_message_byte_array(dummy_socket_notreg->buffer);
    free(dummy_socket_notreg);




    io_handler* dummy_socket_slist;
    dummy_socket_slist = create_dummy_socket(PDU_SLIST, ENTITY_DUMMY);
    pdu* slist = parse_header(dummy_socket_slist);

    printf("\nSLIST pdu from dummy\n");
    slist->print(slist);
    slist->free_pdu(slist);
    free_message_byte_array(dummy_socket_slist->buffer);
    free(dummy_socket_slist);




    io_handler* dummy_socket_join;
    dummy_socket_join = create_dummy_socket(PDU_JOIN, ENTITY_DUMMY);
    pdu* join = parse_header(dummy_socket_join);

    printf("\nJOIN pdu from dummy\n");
    join->print(join);
    join->free_pdu(join);
    free_message_byte_array(dummy_socket_join->buffer);
    free(dummy_socket_join);



    io_handler* dummy_socket_participants;
    dummy_socket_participants = create_dummy_socket(PDU_PARTICIPANTS, ENTITY_DUMMY);
    pdu* participants = parse_header(dummy_socket_participants);

    printf("\nPARTICIPANTS pdu from dummy\n");
    participants->print(participants);
    participants->free_pdu(participants);
    free_message_byte_array(dummy_socket_participants->buffer);
    free(dummy_socket_participants);




    io_handler* dummy_socket_quit;
    dummy_socket_quit = create_dummy_socket(PDU_QUIT, ENTITY_DUMMY);
    pdu* quit = parse_header(dummy_socket_quit);

    printf("\nQUIT pdu from dummy\n");
    quit->print(quit);
    quit->free_pdu(quit);
    free_message_byte_array(dummy_socket_quit->buffer);
    free(dummy_socket_quit);




	io_handler *dummy_socket_mess;
    dummy_socket_mess = create_dummy_socket(PDU_MESS, ENTITY_DUMMY);
	pdu* mess = parse_header(dummy_socket_mess);

    printf("\nMESS pdu from dummy\n");
    mess->print(mess);
    mess->free_pdu(mess);
    free_message_byte_array(dummy_socket_mess->buffer);
    free(dummy_socket_mess);




    io_handler* dummy_socket_pjoin;
    dummy_socket_pjoin = create_dummy_socket(PDU_PJOIN, ENTITY_DUMMY);
    pdu* pjoin = parse_header(dummy_socket_pjoin);

    printf("\nPJOIN pdu from dummy\n");
    pjoin->print(pjoin);
    pjoin->free_pdu(pjoin);
    free_message_byte_array(dummy_socket_pjoin->buffer);
    free(dummy_socket_pjoin);




    io_handler* dummy_socket_pleave;
    dummy_socket_pleave = create_dummy_socket(PDU_PLEAVE, ENTITY_DUMMY);
    pdu* pleave = parse_header(dummy_socket_pleave);

    printf("\nPJLEAVE pdu from dummy\n");
    pleave->print(pleave);
    pleave->free_pdu(pleave);
    free_message_byte_array(dummy_socket_pleave->buffer);
    free(dummy_socket_pleave);


	return 0;

}

