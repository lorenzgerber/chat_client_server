/*
 * chat_loop.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#include "client.h"
#include "chat_loop.h"

int chat_loop(current_user *u) {

    io_handler* chat_server_com = create_tcp_client_communicator(u->join_server->address,
                                                                 u->join_server->port);
    chat_server_com->connect(chat_server_com, 5);

    pdu *join = create_join((uint8_t) strlen(u->identity));
    join->add_identity(join, u->identity);

    chat_server_com->send_pdu(chat_server_com, join);
    free_join(join);

    chat_server_com->status = 0;
    pdu* participants = NULL;
    // looping until an error happens or we get data
    while(chat_server_com->status == STATUS_RECEIVE_EMPTY || chat_server_com->status == 0){
        participants = parse_header(chat_server_com);
    }
    if(chat_server_com->status != STATUS_RECEIVE_OK){
        printf("\nsomething wrong with receive in Client\n");
    }
    print_participants(participants);
    participants->free_pdu(participants);

    pdu* quit = create_quit();
    chat_server_com->send_pdu(chat_server_com, quit);
    quit->free_pdu(quit);
    free_tcp_client_communicator(chat_server_com);
    printf("exiting chat server\n");
    return 0;
}
