//
// Created by knig on 2017-10-04.
//

#include "socket_creator.h"

int main(int argc, char*argv[]) {

    printf("\n\n---Socket testing udp---\n");

    //*******create listen io_handler******

    io_handler* udp_server = create_listen_udp_socket("localhost", UDP_PORT);
    udp_server->listen(udp_server);

    //******Create send io handler*****
    io_handler *udp_client = create_client_udp_socket("localhost", UDP_PORT);

    //create reg pdu and send it
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

    close(udp_server->sfd_listen);

    return 0;
}  