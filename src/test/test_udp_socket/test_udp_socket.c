//
// Created by knig on 2017-10-04.
//



#include <pthread.h>
#include <stdlib.h>
#include <socket_templates.h>
#include <socket_creator.h>
#include "pdu_parser.h"

void *client(void* data);

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void diep(char *s) {
    perror(s);
    exit(1);
}
int teststuff(){
    struct sockaddr_in si_me, si_other;
    int s, i, slen = sizeof(si_other);
    char buf[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, &si_me, sizeof(si_me)) == -1)
        diep("bind");

    io_handler* testhandler = malloc(sizeof(io_handler));

    for (i = 0; i < NPACK; i++) {
        if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen) == -1)
            diep("recvfrom()");
        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
        
    }

}

int main(int argc, char*argv[]){

    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));

    char* address = "localhost";
    io_handler *server_listener;
    io_handler *server_com;


    pthread_create(thread_handle, NULL, client, NULL);
    teststuff();
/*
    server_listener = create_udp_server_listener(address, 2000);
    //server_com = udp_server_listen(server_listener);
    server_com = server_listener->listen(server_listener);
    if(server_com != NULL){
        printf("udp server connected to client\n");
    }

    pdu* ack = parse_header(server_com);

    // receiving and printing of message from client
    printf("\nACK pdu from dummy\n");
    printf("op code: %d\n", ack->type);
    printf("identity nr: %d\n", ack->id_number);
    ack->free_pdu(ack);


    pthread_join(*thread_handle, NULL);
    free(thread_handle);
*/
    return 0;
}



void * client(void* data){
    char* address = "localhost";

    io_handler *client;
    pdu *test = create_ack(1234);

    client = create_udp_client_communicator(address, PORT);
    client->connect(client, 5);
    client->send_pdu(client, test);

    test = create_ack(1234);

    client = create_udp_client_communicator(address, PORT);
    client->connect(client, 5);
    client->send_pdu(client, test);

    return NULL;
}