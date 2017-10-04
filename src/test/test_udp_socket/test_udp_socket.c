//
// Created by knig on 2017-10-04.
//
#include <stdio.h>
#include <arpa/inet.h>
#include "udp_socket.h"
#include "socket_creator.h"

int main(int argc, char*argv[]) {

    printf("\n\n---Socket testing udp---\n");
    /*
    //******Create send io handler*****
    io_handler *udp_client = create_client_udp_socket("localhost", 1337);

    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    pdu *testREG = create_reg((uint8_t) strlen(hostname), 5005);
    testREG->add_server_name(testREG, hostname);
    printf("test reg pdu:\n");
    printf("op: %d\n", testREG->type);
    printf("servername length: %d\n", testREG->server_name_length);
    printf("TCP port: %d\n", testREG->tcp_port);
    printf("servername: %s\n", testREG->server_name);

    udp_client->send_pdu(udp_client, testREG);
    */
    //*******create listen io_handler******
    io_handler* udp_server = create_listen_udp_socket("localhost", 1337);
    char buf[1024];
    struct sockaddr_in from;
    int slen=sizeof(from);


    printf("waiting\n");
    if (recvfrom(udp_server->sfd_listen, buf, 1024, 0, (struct sockaddr *) &from, (socklen_t *) &slen) == -1)
        fprintf(stderr, "receive from");
    printf("Received packet from %s:%d\nData: %s\n\n",
           inet_ntoa(from.sin_addr) , ntohs(1337), buf);


    close(udp_server->sfd_listen);

    return 0;
}  