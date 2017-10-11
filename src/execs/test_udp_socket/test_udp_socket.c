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

    message_byte_array* MBA;
    // create udp communicator
    udp_com = create_udp_communicator(address, NAME_SERVER_PORT);
    udp_com->connect(udp_com, 5);

    //send alive to nameserver (id 10000)
    pdu *alive = create_alive(100, 10000);
    udp_com->send_pdu(udp_com, alive);
    free_alive(alive);

    //receive answer (NOTREG expected)
    pdu* answer_pdu = parse_header(udp_com);
    if(answer_pdu->type == 100){
        printf("\nNOTREG received as answer to ALIVE 10000:");
        printf("\nOp code: %d", answer_pdu->type);
        printf("\nId: %d\n", answer_pdu->id_number);
        MBA = answer_pdu->create_message(answer_pdu);
        for(int i = 0;i < answer_pdu->get_message_length(answer_pdu);i++){
            printf("%d, ", MBA->array[i]);
        }
        answer_pdu->free_pdu(answer_pdu);
    }else if(answer_pdu->type == 1){
        printf("\nACK received as answer to ALIVE 10000:");
        printf("\nOp code: %d", answer_pdu->type);
        printf("\nId: %d\n", answer_pdu->id_number);
        MBA = answer_pdu->create_message(answer_pdu);
        for(int i = 0;i < answer_pdu->get_message_length(answer_pdu);i++){
            printf("%d, ", MBA->array[i]);
        }
        answer_pdu->free_pdu(answer_pdu);
    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver");
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
    pdu* answer_pdu_2 = parse_header(udp_com);
    int registered;
    if(answer_pdu_2->type == 1){
        printf("\nACK %d received as answer to REG:", answer_pdu_2->id_number);
        printf("\nOp code: %d", answer_pdu_2->type);
        printf("\nId: %d\n", answer_pdu_2->id_number);
        MBA = answer_pdu_2->create_message(answer_pdu_2);
        for(int i = 0;i < answer_pdu_2->get_message_length(answer_pdu_2);i++){
            printf("%d, ", MBA->array[i]);
        }
        registered = answer_pdu_2->id_number;
        answer_pdu_2->free_pdu(answer_pdu_2);
    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver");
        fflush(stderr);
        return EXIT_FAILURE;
    }
    printf("\n----------");

    //send ALIVE to name server with the registered id
    pdu *alive2 = create_alive(100, (uint16_t) registered);
    udp_com->send_pdu(udp_com, alive2);
    alive2->free_pdu(alive2);

    //Receive answer
    pdu* answer_pdu_3 = parse_header(udp_com);
    if(answer_pdu_3->type == 1){
        printf("\nACK %d received as answer to ALIVE %d:",
               answer_pdu_3->id_number,
               registered);
        printf("\nOp code: %d", answer_pdu_3->type);
        printf("\nId: %d\n", answer_pdu_3->id_number);
        MBA = answer_pdu_3->create_message(answer_pdu_2);
        for(int i = 0;i < answer_pdu_3->get_message_length(answer_pdu_3);i++){
            printf("%d, ", MBA->array[i]);
        }
        answer_pdu_3->free_pdu(answer_pdu_3);
    }else if(answer_pdu_3->type == 100){
        printf("\nNOTREG received as answer to ALIVE %d:", registered);
        printf("\nOp code: %d", answer_pdu_3->type);
        printf("\nId: %d\n", answer_pdu_3->id_number);
        MBA = answer_pdu_3->create_message(answer_pdu_3);
        for(int i = 0;i < answer_pdu_3->get_message_length(answer_pdu_3);i++){
            printf("%d, ", MBA->array[i]);
        }
        answer_pdu_3->free_pdu(answer_pdu_3);
    }else{
        fprintf(stderr, "\nReceived unexpected response from nameserver");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    free_message_byte_array(MBA);

    return 0;
}