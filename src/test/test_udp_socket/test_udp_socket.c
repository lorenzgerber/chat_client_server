//
// Created by knig on 2017-10-04.
//



#include <pthread.h>
#include <stdlib.h>
#include <socket_templates.h>
#include <socket_creator.h>
#include "pdu_parser.h"

void *client(void* data);


int main(int argc, char*argv[]){

    // Setup Thread for client / server operation
    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));

    char* address = "localhost";

    // start client Thread
    pthread_create(thread_handle, NULL, client, NULL);

    // create listener server
    io_handler* server = create_udp_server(address, 2000);
    server->listen(server);
    pdu* ack = parse_header(server);
    printf("\n***%d***\n", ack->type);

    pthread_join(*thread_handle, NULL);
    free(thread_handle);

    return 0;
}



void * client(void* data){
    char* address = "localhost";

    io_handler *client;
    pdu *test = create_ack(1234);

    client = create_udp_client_communicator(address, 2000);
    client->connect(client, 5);
    client->send_pdu(client, test);

    test = create_ack(1234);

    client = create_udp_client_communicator(address, 2000);
    client->connect(client, 5);
    client->send_pdu(client, test);

    return NULL;
}