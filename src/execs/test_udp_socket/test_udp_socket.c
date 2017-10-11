/*
 * test_udp_socket.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#define NAME_SERVER "itchy.cs.umu.se"
#define NAME_SERVER_PORT 1337

#include <stdlib.h>

#include "socket_templates.h"
#include "socket_creator.h"

void *client(void* data);

int main(int argc, char*argv[]){

    char* address = NAME_SERVER;
    io_handler *udp_com;

    // create udp communicator
    udp_com = create_udp_communicator(address, NAME_SERVER_PORT);
    udp_com->connect(udp_com, 5);
    udp_com->status = 0;
    //send alive to nameserver (id 10000)
    pdu *alive = create_alive(100, 10000);
    udp_com->send_pdu(udp_com, alive);
    free_alive(alive);

    //receive answer (NOTREG expected)
    pdu* answer_pdu = NULL;
    while(udp_com->status == STATUS_RECEIVE_EMPTY || udp_com->status == 0){
        answer_pdu = parse_header(udp_com);
    }
    if(udp_com->status != STATUS_RECEIVE_OK){
        printf("something wrong with receive\n");
    }
    udp_com->status = 0;

    if(answer_pdu != NULL && answer_pdu->type == 100){
        printf("\nNOTREG received as answer to ALIVE 10000:\n");
        answer_pdu->print(answer_pdu);
        answer_pdu->free_pdu(answer_pdu);
    }else if(answer_pdu != NULL && answer_pdu->type == 1){
        printf("\nACK received as answer to ALIVE 10000:\n");
        answer_pdu->print(answer_pdu);
        answer_pdu->free_pdu(answer_pdu);
    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    //send REG to name server
    pdu* reg = create_reg(13, 2000);
    reg->add_server_name(reg,"ljugarbaenken");
    udp_com->send_pdu(udp_com, reg);
    reg->free_pdu(reg);

    printf("\n----------");
    //receive answer (ACK expected)
    pdu* answer_pdu_2 = NULL;
    while(udp_com->status == STATUS_RECEIVE_EMPTY || udp_com->status == 0){
        answer_pdu_2 = parse_header(udp_com);
    }
    if(udp_com->status != STATUS_RECEIVE_OK){
        printf("something wrong with receive\n");
    }
    udp_com->status = 0;

    int registered;
    if(answer_pdu_2 != NULL && answer_pdu_2->type == 1) {
        printf("\nACK %d received as answer to REG:\n",
               answer_pdu_2->id_number);
        answer_pdu_2->print(answer_pdu_2);
        registered = answer_pdu_2->id_number;
        answer_pdu_2->free_pdu(answer_pdu_2);
    }else if(answer_pdu_2 == NULL){
        printf("sgsdfgsdfgsdfg");

    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }
    printf("\n----------");

    //send ALIVE to name server with the registered id
    pdu *alive2 = create_alive(100, (uint16_t) registered);
    udp_com->send_pdu(udp_com, alive2);
    alive2->free_pdu(alive2);

    //Receive answer
    pdu* answer_pdu_3 = NULL;
    while(udp_com->status == STATUS_RECEIVE_EMPTY || udp_com->status == 0){
        answer_pdu_3 = parse_header(udp_com);
    }
    if(udp_com->status != STATUS_RECEIVE_OK){
        printf("something wrong with receive\n");
    }

    udp_com->status = 0;
    if(answer_pdu_3 != NULL && answer_pdu_3->type == 1){
        printf("\nACK %d received as answer to ALIVE %d:\n",
               answer_pdu_3->id_number,
               registered);
        answer_pdu_3->print(answer_pdu_3);
        answer_pdu_3->free_pdu(answer_pdu_3);
    }else if(answer_pdu_3!= NULL && answer_pdu_3->type == 100){
        printf("\nNOTREG received as answer to ALIVE %d:\n", registered);
        answer_pdu_3->print(answer_pdu_3);
        answer_pdu_3->free_pdu(answer_pdu_3);
    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    udp_com->close(udp_com);

    return 0;
}