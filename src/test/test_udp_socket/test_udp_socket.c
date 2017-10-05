//
// Created by knig on 2017-10-04.
//



#include <pthread.h>
#include <stdlib.h>
#include <socket_templates.h>
#include <socket_creator.h>

void *client(void* data);

int main(int argc, char*argv[]){

    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));

    char* address = "localhost";
    io_handler *server_listener;
    io_handler *server_com;


    //pthread_create(thread_handle, NULL, client, NULL);

    server_listener = create_udp_server_listener(address, 2000);

    server_com = server_listener->listen(server_listener);
    if(server_com != NULL){
        printf("udp server connected to client\n");
    }
/*
    pdu* ack = parse_header(server_com);

    // receiving and printing of message from client
    printf("\nACK pdu from dummy\n");
    printf("op code: %d\n", ack->type);
    printf("identity nr: %d\n", ack->id_number);
    ack->free_pdu(ack);
*/

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

    return NULL;
}