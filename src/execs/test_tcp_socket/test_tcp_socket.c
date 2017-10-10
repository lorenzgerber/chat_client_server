/*
 * test_tcp_socket.c
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
#include <pthread.h>

#include "tcp_socket.h"
#include "socket_creator.h"
#include "socket_templates.h"
#include "pdu_parser.h"
#include "pdu_creator.h"

void *client(void* data);

int main(int argc, char*argv[]){

    // Setup Thread for client / server operation
    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));

    // setup variables for listener and communication server
    char* address = "localhost";
    io_handler *server_listener;
    io_handler *server_com;

    // start client Thread
    pthread_create(thread_handle, NULL, client, NULL);

    // create listener server
    server_listener = create_tcp_server_listener(address, 2000);

    // start listen and on connection create communication server
    server_com = server_listener->listen(server_listener);
    if(server_com != NULL){
        printf("tcp server connected to client\n");
    }

    // receive and parse pdu
    printf("----\n");
    pdu* ack = parse_header(server_com);
    ack->print(ack);
    ack->free_pdu(ack);
    printf("----\n");


    pdu *participants = create_participants(3, 15);
    participants->add_identities(participants, "partic\0ipa\0nts\0");

    server_com->send_pdu(server_com, participants);

    pthread_join(*thread_handle, NULL);
    free(thread_handle);

    return 0;
}



void * client(void* data){
    char* address = "localhost";

    io_handler *client;
    pdu *test = create_ack(1234);

    client = create_tcp_client_communicator(address, 2000);
    client->connect(client, 5);
    client->send_pdu(client, test);
    test->free_pdu(test);

    pdu *participants = parse_header(client);
    participants->print(participants);
    participants->free_pdu(participants);
    printf("----\n");

    return NULL;
}

