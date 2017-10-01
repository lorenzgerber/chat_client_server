/*
 * dummy_sockets.c
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */
#include "dummy_sockets.h"

io_handler* dummy_socket_mess(io_handler * dummy_socket){

	pdu_MESS *MESS = create_MESS(8, 99);
	MESS->add_message(MESS, 13, 1505933137, "Test Message.");
	MESS->add_client_identity(MESS, "identity");
	dummy_socket->buffer = MESS->create_message(MESS);
	free_pdu_mess(MESS);

	return dummy_socket;
}

io_handler* dummy_socket_ack(io_handler* dummy_socket){

	pdu_ACK* ACK = create_ACK(4444);

	dummy_socket->buffer = ACK->create_message(ACK);
	free_pdu_ack(ACK);

	return dummy_socket;
}

io_handler* dummy_socket_notreg(io_handler* dummy_socket){
	pdu_NOTREG* NOTREG = create_NOTREG(5555);

	dummy_socket->buffer = NOTREG->create_message(NOTREG);
	free_pdu_notreg(NOTREG);

	return dummy_socket;
}

io_handler* dummy_socket_slist(io_handler* dummy_socket){

	pdu_SLIST *SLIST = create_SLIST(2);
	uint8_t address[4] = {127,0,0,1};
	pdu_server_entry* server1 = create_server_entry(address,2000,4,10);
	server1->add_server_name(server1, "servername");
	SLIST->add_server_entry(SLIST, server1);

	uint8_t  address2[4] = {127,0,0,2};
	pdu_server_entry* server2 = create_server_entry(address2,2001,4,11);
	server2->add_server_name(server2, "servername2");
	SLIST->add_server_entry(SLIST, server2);

	dummy_socket->buffer = SLIST->create_message(SLIST);
	free_pdu_slist(SLIST);

	return dummy_socket;
}
