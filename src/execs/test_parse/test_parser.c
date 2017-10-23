/*
 *  main.c
 *
 *	Main file for parser tests
 *
 *	build with: make EXEC=parser_test
 *
 *  Created on: Oct 1, 2017
 *  Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN "\033[32m"

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

    int errors = 0;

    io_handler* dummy_socket_ack;
    dummy_socket_ack = create_dummy_socket(PDU_ACK, ENTITY_DUMMY);
    pdu* ack = parse_header(dummy_socket_ack);

    if(ack->get_message_length(ack) != 4 || ack->type != 1 || ack->id_number != 4444){
        printf("ACK parsing :                  ");
        printf(RED"FAIL\n"RESET);
        errors++;
        ack->print(ack);
    }else{
        printf("ACK parsing :                  ");
        printf(GREEN"OK\n"RESET);
    }
    ack->free_pdu(ack);
    free_message_byte_array(dummy_socket_ack->buffer);
    free(dummy_socket_ack);



    io_handler* dummy_socket_notreg;
    dummy_socket_notreg = create_dummy_socket(PDU_NOTREG, ENTITY_DUMMY);
    pdu* notreg = parse_header(dummy_socket_notreg);

    if(notreg->get_message_length(notreg) != 4 || notreg->type != 100 || notreg->id_number != 5555){
        printf("NOTREG parsing :               ");
        printf(RED"FAIL\n"RESET);
        errors++;
        notreg->print(notreg);
    }else{
        printf("NOTREG parsing :               ");
        printf(GREEN"OK\n"RESET);
    }
    notreg->free_pdu(notreg);
    free_message_byte_array(dummy_socket_notreg->buffer);
    free(dummy_socket_notreg);




    io_handler* dummy_socket_slist;
    dummy_socket_slist = create_dummy_socket(PDU_SLIST, ENTITY_DUMMY);
    pdu* slist = parse_header(dummy_socket_slist);

    if(slist->get_message_length(slist) != 44 ||
            slist->type != 4 ||
            slist->number_servers != 2 ||
            strcmp(slist->current_servers[0]->name, "servername") != 0 ||
            slist->current_servers[0]->number_clients != 4 ||
            slist->current_servers[0]->name_length != 10 ||
            slist->current_servers[0]->port != 2000 ||
            slist->current_servers[0]->address[0] != 127 ||
            slist->current_servers[0]->address[1] != 0 ||
            slist->current_servers[0]->address[2] != 0 ||
            slist->current_servers[0]->address[3] != 1 ||
            strcmp(slist->current_servers[1]->name, "servername2") != 0 ||
            slist->current_servers[1]->number_clients != 5 ||
            slist->current_servers[1]->name_length != 11 ||
            slist->current_servers[1]->port != 2001 ||
            slist->current_servers[1]->address[0] != 127 ||
            slist->current_servers[1]->address[1] != 0 ||
            slist->current_servers[1]->address[2] != 0 ||
            slist->current_servers[1]->address[3] != 2){
        printf("SLIST parsing :                ");
        printf(RED"FAIL\n"RESET);
        errors++;
        slist->print(slist);
    }else{
        printf("SLIST parsing :                ");
        printf(GREEN"OK\n"RESET);
    }
    slist->free_pdu(slist);
    free_message_byte_array(dummy_socket_slist->buffer);
    free(dummy_socket_slist);




    io_handler* dummy_socket_join;
    dummy_socket_join = create_dummy_socket(PDU_JOIN, ENTITY_DUMMY);
    pdu* join = parse_header(dummy_socket_join);

    if(join->get_message_length(join) != 12 ||
            join->type != 12 ||
            join->identity_length != 8 ||
            strcmp(join->identity, "identity") != 0){
        printf("JOIN parsing :                 ");
        printf(RED"FAIL\n"RESET);
        errors++;
        join->print(join);
    }else{
        printf("JOIN parsing :                 ");
        printf(GREEN"OK\n"RESET);
    }
    join->free_pdu(join);
    free_message_byte_array(dummy_socket_join->buffer);
    free(dummy_socket_join);



    io_handler* dummy_socket_participants;
    dummy_socket_participants = create_dummy_socket(PDU_PARTICIPANTS, ENTITY_DUMMY);
    pdu* participants = parse_header(dummy_socket_participants);

    int participants_check = 0;
    char expected_part1[6] = {'p','a','r','t','i','c'};
    char expected_part2[3] = {'i','p','a'};
    char expected_part3[3] = {'n','t','s'};
    char* part_pointer[3];
    part_pointer[0] = expected_part1;
    part_pointer[1] = expected_part2;
    part_pointer[2] = expected_part3;
    for(int i = 0; i < participants->number_identities;i++){
        for(int j = 0; j < strlen(participants->identities[i]); j++){
            if(part_pointer[i][j] != participants->identities[i][j]){
                participants_check = 1;
            }
        }
    }
    if(participants->get_message_length(participants) != 20 ||
            participants->type != 19 ||
            participants->number_identities != 3 ||
            participants_check != 0){
        printf("PARTICIPANTS parsing :         ");
        printf(RED"FAIL\n"RESET);
        errors++;
        participants->print(participants);
    }else{
        printf("PARTICIPANTS parsing :         ");
        printf(GREEN"OK\n"RESET);
    }
    participants->free_pdu(participants);
    free_message_byte_array(dummy_socket_participants->buffer);
    free(dummy_socket_participants);




    io_handler* dummy_socket_quit;
    dummy_socket_quit = create_dummy_socket(PDU_QUIT, ENTITY_DUMMY);
    pdu* quit = parse_header(dummy_socket_quit);

    if(quit->get_message_length(quit) != 4 || quit->type != 11){
        printf("QUIT parsing :                 ");
        printf(RED"FAIL\n"RESET);
        errors++;
        quit->print(quit);
    }else{
        printf("QUIT parsing :                 ");
        printf(GREEN"OK\n"RESET);
    }
    quit->free_pdu(quit);
    free_message_byte_array(dummy_socket_quit->buffer);
    free(dummy_socket_quit);




	io_handler *dummy_socket_mess;
    dummy_socket_mess = create_dummy_socket(PDU_MESS, ENTITY_DUMMY);
	pdu* mess = parse_header(dummy_socket_mess);

    if(mess->get_message_length(mess) != 36 ||
            mess->type != 10 ||
            mess->message_length != 13 ||
            mess->checksum != 99 ||
            mess->time_stamp != 1505933137 ||
            strcmp(mess->message, "Test Message.") != 0 ||
            strcmp(mess->identity, "identity") != 0 ||
            mess->identity_length != 8){
        printf("MESS parsing :                 ");
        printf(RED"FAIL\n"RESET);
        errors++;
        mess->print(mess);
    }else{
        printf("MESS parsing :                 ");
        printf(GREEN"OK\n"RESET);
    }
    mess->free_pdu(mess);
    free_message_byte_array(dummy_socket_mess->buffer);
    free(dummy_socket_mess);




    io_handler* dummy_socket_pjoin;
    dummy_socket_pjoin = create_dummy_socket(PDU_PJOIN, ENTITY_DUMMY);
    pdu* pjoin = parse_header(dummy_socket_pjoin);

    if(pjoin->get_message_length(pjoin) != 16 ||
            pjoin->type != 16 ||
            pjoin->identity_length != 8 ||
            pjoin->time_stamp != 1505933137 ||
            strcmp(pjoin->identity, "identity") != 0){
        printf("PJOIN parsing :                ");
        printf(RED"FAIL\n"RESET);
        errors++;
        pjoin->print(pjoin);
    }else{
        printf("PJOIN parsing :                ");
        printf(GREEN"OK\n"RESET);
    }
    pjoin->free_pdu(pjoin);
    free_message_byte_array(dummy_socket_pjoin->buffer);
    free(dummy_socket_pjoin);




    io_handler* dummy_socket_pleave;
    dummy_socket_pleave = create_dummy_socket(PDU_PLEAVE, ENTITY_DUMMY);
    pdu* pleave = parse_header(dummy_socket_pleave);

    if(pleave->get_message_length(pleave) != 16 ||
            pleave->type != 17 ||
            pleave->identity_length != 8 ||
            pleave->time_stamp != 1505933137 ||
       strcmp(pleave->identity, "identity") != 0){
        printf("PLEAVE parsing :               ");
        printf(RED"FAIL\n"RESET);
        errors++;
        pleave->print(pleave);
    }else{
        printf("PLEAVE parsing :               ");
        printf(GREEN"OK\n"RESET);
    }
    pleave->free_pdu(pleave);
    free_message_byte_array(dummy_socket_pleave->buffer);
    free(dummy_socket_pleave);

    printf("\nParse tests finished with %d errors\n", errors);
	return 0;

}

