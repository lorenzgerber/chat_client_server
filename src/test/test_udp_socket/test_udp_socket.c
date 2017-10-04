//
// Created by knig on 2017-10-04.
//
#include <stdio.h>
#include "udp_socket.h"
#include "socket_creator.h"

int main(int argc, char*argv[]) {
    printf("\n\n---Socket testing udp---\n");

    io_handler *udphandler = create_client_udp_socket("knig-VirtualBox", 1337);
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

    char testbuf[1024];
    testbuf[1023] = '\0';
    udphandler->send_pdu(udphandler, testREG);

    //recvfrom(udphandler->socket, testbuf, 1024, 0,)
    return 0;
}  