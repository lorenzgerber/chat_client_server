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
    pdu* ack = parse_header(dummy_socket_ack);

    printf("\nACK pdu from dummy\n");
    printf("op code: %d\n", ack->type);
    printf("identity nr: %d\n", ack->id_number);

    io_handler* dummy_socket_notreg;
    dummy_socket_notreg = create_dummy_socket(PDU_NOTREG, ENTITY_SERVER);
    pdu* notreg = parse_header(dummy_socket_notreg);

    printf("\nNOTREG pdu from dummy\n");
    printf("op code: %d\n", notreg->type);
    printf("identity nr: %d\n", notreg->id_number);

    io_handler* dummy_socket_slist;
    dummy_socket_slist = create_dummy_socket(PDU_SLIST, ENTITY_CLIENT);
    pdu* slist = parse_header(dummy_socket_slist);

    printf("\nSLIST pdu from dummy\n");
    printf("op code: %d\n", slist->type);
    printf("nr of servers: %d\n", slist->number_servers);
    for(int i = 0;i < slist->number_servers; i++){
        printf("****Server %d****\n", i+1);
        printf("adress: %d,%d,%d,%d\n", slist->current_servers[i]->address[0],
                                        slist->current_servers[i]->address[1],
                                        slist->current_servers[i]->address[2],
                                        slist->current_servers[i]->address[3]);
        printf("port: %d\n", slist->current_servers[i]->port);
        printf("number of clients: %d\n", slist->current_servers[i]->number_clients);
        printf("server name length: %d\n", slist->current_servers[i]->name_length);
        printf("Servername: %s\n", slist->current_servers[i]->name);
    }

    io_handler* dummy_socket_join;
    dummy_socket_join = create_dummy_socket(PDU_JOIN, ENTITY_SERVER);
    pdu* join = parse_header(dummy_socket_join);

    printf("\nJOIN pdu from dummy\n");
    printf("op code: %d\n", join->type);
    printf("identity length: %d\n", join->identity_length);
    printf("identity: %s\n", join->identity);

    io_handler* dummy_socket_participants;
    dummy_socket_participants = create_dummy_socket(PDU_PARTICIPANTS, ENTITY_CLIENT);
    pdu* participants = parse_header(dummy_socket_participants);

    printf("\nPARTICIPANTS pdu from dummy\n");
    printf("op code: %d\n", participants->type);
    printf("nr of identities: %d\n", participants->number_identities);
    printf("identities length: %d\n", participants->length);
    for(int i = 0; i < participants->number_identities; i++){
        printf("Identity %d: %s\n",i+1,participants->identities[i]);
    }

    io_handler* dummy_socket_quit;
    dummy_socket_quit = create_dummy_socket(PDU_QUIT, ENTITY_CLIENT);
    parse_header(dummy_socket_quit);
/*
	io_handler *dummy_socket_mess;
    dummy_socket_mess = create_dummy_socket(PDU_MESS, ENTITY_CLIENT);
	parse_header(dummy_socket_mess);
*/
    io_handler* dummy_socket_pjoin;
    dummy_socket_pjoin = create_dummy_socket(PDU_PJOIN, ENTITY_CLIENT);
    parse_header(dummy_socket_pjoin);

    io_handler* dummy_socket_pleave;
    dummy_socket_pleave = create_dummy_socket(PDU_PLEAVE, ENTITY_CLIENT);
    parse_header(dummy_socket_pleave);



	return 0;

}

