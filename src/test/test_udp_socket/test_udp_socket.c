//
// Created by knig on 2017-10-04.
//



#include <pthread.h>
#include <stdlib.h>
#include <socket_templates.h>
#include <socket_creator.h>

void *client(void* data);


int main(int argc, char*argv[]){

    // Setup Thread for client / server operation
    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));

    char* address = "localhost";
    io_handler *server;
    //io_handler *server_com;
    // start client Thread
    pthread_create(thread_handle, NULL, client, NULL);

    // create listener server
    server = create_udp_server_listener(address, 1337);
    //server_com = server->listen(server);
    pdu *ack = parse_header(server);
    printf("\nOp code: %d", ack->type);
    printf("\nId nr: %d", ack->id_number);

    /*if(server_com != NULL){
        printf("udp server connected to client\n");
    }*/
    //pdu* ack = parse_header(server);
    //printf("\n***%d***\n", ack->type);

    pthread_join(*thread_handle, NULL);
    free(thread_handle);

    return 0;
}



void * client(void* data){
    char* address = "itchy.cs.umu.se";

    io_handler *client;

    pdu *reg = create_reg(9,1337);
    reg->add_server_name(reg,"skitsnack");
    client = create_udp_client_communicator(address, 1337);
    client->connect(client, 5);
    client->send_pdu(client, reg);
    free_reg(reg);
    /*
    pdu *test = create_ack(2324);

    client = create_udp_client_communicator(address, 5000);
    client->connect(client, 5);
    client->send_pdu(client, test);

    free_ack(test);
*/

    return NULL;
}